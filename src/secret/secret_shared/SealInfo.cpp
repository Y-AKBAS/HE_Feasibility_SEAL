
#include "SealInfo.h"

namespace yakbas::sec {

    SealKeys::SealKeys(seal::scheme_type schemeType, size_t polyModulusDegree, int plainModulus, bool isEncodingEnabled)
            : m_schemeType(schemeType),
              m_polyModulusDegree(polyModulusDegree),
              m_plainModulus(plainModulus),
              m_isEncodingEnabled(isEncodingEnabled) {}

    bool SealKeys::operator==(const SealKeys &rhs) const {
        return (this->m_polyModulusDegree == rhs.m_polyModulusDegree) &&
               (this->m_schemeType == rhs.m_schemeType) &&
               (this->m_plainModulus == rhs.m_plainModulus) &&
               (this->m_isEncodingEnabled == rhs.m_isEncodingEnabled);
    }

    bool SealKeys::operator<(const SealKeys &rhs) const {
        return (this->m_polyModulusDegree < rhs.m_polyModulusDegree) &&
               (this->m_schemeType < rhs.m_schemeType) &&
               (this->m_plainModulus < rhs.m_plainModulus);
    }

    std::string SealKeys::ToString() const {

        std::string schemeType{};
        switch (m_schemeType) {
            case seal::scheme_type::bfv :
                schemeType = "bfv";
                break;
            case seal::scheme_type::bgv :
                schemeType = "bgv";
                break;
            case seal::scheme_type::ckks :
                schemeType = "ckks";
                break;
            default :
                schemeType = "SchemeType is missing";
                break;
        }

        const auto stream = std::make_unique<std::ostringstream>();
        *stream << "\nSealKeys: "
                << "\nPlainModulus: " << m_plainModulus
                << "\nSchemeType: " << schemeType
                << "\nPolyModulusDegree: " << m_polyModulusDegree;

        return stream->str();
    }

    SealKeys::SealKeys() = default;

    SealInfo::SealInfo(const SealKeys &sealKeys)
            : m_sealKeys(sealKeys) {

        m_encryptionParamsPtr = std::make_unique<seal::EncryptionParameters>(m_sealKeys.m_schemeType);

        if (m_sealKeys.m_schemeType == seal::scheme_type::bfv ||
            m_sealKeys.m_schemeType == seal::scheme_type::bgv) {

            m_coefficientModulusPtr = std::make_unique<std::vector<seal::Modulus>>(
                    seal::CoeffModulus::BFVDefault(m_sealKeys.m_polyModulusDegree));
            if (m_sealKeys.m_isEncodingEnabled) {
                m_encryptionParamsPtr->set_plain_modulus(
                        seal::PlainModulus::Batching(m_sealKeys.m_polyModulusDegree, 20));
            } else {
                m_encryptionParamsPtr->set_plain_modulus(m_sealKeys.m_plainModulus);
            }

        } else if (m_sealKeys.m_schemeType == seal::scheme_type::ckks) {
            if (!m_sealKeys.m_isEncodingEnabled) {
                throw std::invalid_argument("Encoding should be enabled for CKKS Scheme");
            }
            m_coefficientModulusPtr = std::make_unique<std::vector<seal::Modulus>>(
                    seal::CoeffModulus::Create(m_sealKeys.m_polyModulusDegree, {60, 40, 40, 40, 60}));
        } else {
            throw std::invalid_argument("Please specify a valid scheme!");
        }

        m_encryptionParamsPtr->set_poly_modulus_degree(m_sealKeys.m_polyModulusDegree);
        m_encryptionParamsPtr->set_coeff_modulus(*m_coefficientModulusPtr);
        m_sealContextPtr = std::make_unique<seal::SEALContext>(*m_encryptionParamsPtr);
        m_keyGeneratorPtr = std::make_unique<seal::KeyGenerator>(*m_sealContextPtr);
        m_evaluatorPtr = std::make_unique<seal::Evaluator>(*m_sealContextPtr);
    }

    bool SealInfo::operator==(const SealInfo &rhs) const {
        return this->m_sealKeys == rhs.m_sealKeys;
    }

} // yakbas