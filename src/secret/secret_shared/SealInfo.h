#pragma once

#include <memory>
#include <vector>
#include <seal/modulus.h>
#include <seal/encryptionparams.h>
#include <seal/context.h>
#include <seal/keygenerator.h>
#include <seal/evaluator.h>

namespace yakbas::sec {

    struct SealKeys {

        SealKeys(seal::scheme_type schemeType, size_t polyModulusDegree, int plainModulus, bool isEncodingEnabled,
                 int scalePower, std::vector<int> &bitSizes);

        SealKeys();

        bool operator==(const SealKeys &rhs) const;

        bool operator<(const SealKeys &rhs) const;

        [[nodiscard]] std::string ToString() const;

        seal::scheme_type m_schemeType = static_cast<const seal::scheme_type>(SEAL_SCHEME_TYPE);
        std::size_t m_polyModulusDegree = SEAL_POLY_MODULUS_DEGREE;
        int m_plainModulus = SEAL_PLAIN_MODULUS_DEGREE;
        bool m_isEncodingEnabled = ENCODING_ENABLED;
        int m_scalePower = CKKS_SCALE_POWER;
        std::vector<int> m_encodingBitSizes{};
        double m_scale = std::pow(2.0, m_scalePower);
    };

    struct SealInfo {
    public:

        explicit SealInfo(const SealKeys &mSealKeys = {});

        bool operator==(const SealInfo &rhs) const;

        std::unique_ptr<seal::EncryptionParameters> m_encryptionParamsPtr{nullptr};
        std::unique_ptr<const seal::SEALContext> m_sealContextPtr{nullptr};
        std::unique_ptr<const seal::Evaluator> m_evaluatorPtr{nullptr};
        std::unique_ptr<const std::vector<seal::Modulus>> m_coefficientModulusPtr{nullptr};
        const SealKeys m_sealKeys{};
    };

} // yakbas