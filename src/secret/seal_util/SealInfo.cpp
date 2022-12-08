
#include "SealInfo.h"

namespace yakbas::sec {

    SealKeys::SealKeys(seal::scheme_type mSchemeType, size_t mPolyModulusDegree, int mPlainModulus)
            : m_schemeType(mSchemeType), m_polyModulusDegree(mPolyModulusDegree), m_plainModulus(mPlainModulus) {}

    bool SealKeys::operator==(const SealKeys &rhs) const {
        return (this->m_polyModulusDegree == rhs.m_polyModulusDegree) &&
               (this->m_schemeType == rhs.m_schemeType) &&
               (this->m_plainModulus == rhs.m_plainModulus);
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
        *stream << "SealKeys: "
                << "\nPlainModulus: " << m_plainModulus
                << "\nSchemeType: " << schemeType
                << "\nPolyModulusDegree: " << m_polyModulusDegree;

        return stream->str();
    }

    SealInfo::SealInfo(const SealKeys &sealKeys)
            : m_sealKeys(sealKeys) {

        m_coefficientModulusPtr = std::make_unique<std::vector<seal::Modulus>>(
                seal::CoeffModulus::BFVDefault(m_sealKeys.m_polyModulusDegree));
        m_encryptionParamsPtr = std::make_unique<seal::EncryptionParameters>(m_sealKeys.m_schemeType);

        m_encryptionParamsPtr->set_poly_modulus_degree(m_sealKeys.m_polyModulusDegree);
        m_encryptionParamsPtr->set_coeff_modulus(*m_coefficientModulusPtr);
        m_encryptionParamsPtr->set_plain_modulus(m_sealKeys.m_plainModulus);

        m_sealContextPtr = std::make_unique<seal::SEALContext>(*m_encryptionParamsPtr);
        m_keyGeneratorPtr = std::make_unique<seal::KeyGenerator>(*m_sealContextPtr);
        m_evaluatorPtr = std::make_unique<seal::Evaluator>(*m_sealContextPtr);
    }

    bool SealInfo::operator==(const SealInfo &rhs) const {
        return this->m_sealKeys == rhs.m_sealKeys;
    }

} // yakbas