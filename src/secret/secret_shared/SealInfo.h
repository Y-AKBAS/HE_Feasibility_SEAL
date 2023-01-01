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

        SealKeys(seal::scheme_type schemeType, size_t polyModulusDegree, int plainModulus, bool isEncodingEnabled);

        SealKeys();

        bool operator==(const SealKeys &rhs) const;

        bool operator<(const SealKeys &rhs) const;

        [[nodiscard]] std::string ToString() const;

        const seal::scheme_type m_schemeType = static_cast<const seal::scheme_type>(SEAL_SCHEME_TYPE);
        const std::size_t m_polyModulusDegree = SEAL_POLY_MODULUS_DEGREE;
        const int m_plainModulus = SEAL_PLAIN_MODULUS_DEGREE;
        const bool m_isEncodingEnabled = ENCODING_ENABLED;
    };

    struct SealInfo {
    public:

        explicit SealInfo(const SealKeys &mSealKeys = {});

        bool operator==(const SealInfo &rhs) const;

        std::unique_ptr<seal::EncryptionParameters> m_encryptionParamsPtr;
        std::unique_ptr<const seal::SEALContext> m_sealContextPtr;
        std::unique_ptr<seal::KeyGenerator> m_keyGeneratorPtr;
        std::unique_ptr<const seal::Evaluator> m_evaluatorPtr;
        std::unique_ptr<const std::vector<seal::Modulus>> m_coefficientModulusPtr;
        const SealKeys m_sealKeys;
    };

} // yakbas