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

        SealKeys(seal::scheme_type mSchemeType, size_t mPolyModulusDegree, int mPlainModulus);

        bool operator==(const SealKeys &rhs) const;
        bool operator<(const SealKeys &rhs) const;
        [[nodiscard]] std::string ToString() const;

        const seal::scheme_type m_schemeType;
        const std::size_t m_polyModulusDegree;
        const int m_plainModulus;
    };

    struct SealInfo {
    public:

        explicit SealInfo(const SealKeys &mSealKeys = {seal::scheme_type::bfv, 16384, 1024});

        bool operator==(const SealInfo &rhs) const;

        std::unique_ptr<seal::EncryptionParameters> m_encryptionParamsPtr;
        std::unique_ptr<const seal::SEALContext> m_sealContextPtr;
        std::unique_ptr<const seal::KeyGenerator> m_keyGeneratorPtr;
        std::unique_ptr<const seal::Evaluator> m_evaluatorPtr;
        std::unique_ptr<const std::vector<seal::Modulus>> m_coefficientModulusPtr;
        const SealKeys m_sealKeys;
    };

} // yakbas