
#include "ClientGenerator.h"
#include "Utils.h"
#include "Timer.h"

namespace yakbas::sec {
    using namespace yakbas::util;

    std::unique_ptr<SecretUser> ClientGenerator::GenerateSecretUser(const SealKeys &sealKeys) {

        auto addressPtr = std::make_unique<BaseUser::Address>(
                "My street",
                "My house number",
                "My postal code",
                "My city",
                "My country"
        );

        return std::make_unique<SecretUser>(
                GetUUID(),
                "Yasin",
                "Akbas",
                "my@gmail.com",
                addressPtr,
                std::make_unique<CustomSealOperations>(sealKeys)
        );
    }

    std::unique_ptr<communication::sec::SearchRequest>
    ClientGenerator::GenerateSearchRequest(const std::unique_ptr<std::stringstream> &publicKeyBuffer) {

        auto requestPtr = GetUnique<communication::sec::SearchRequest>();
        const auto startTimePtr = GetUnique<google::protobuf::Timestamp>();
        startTimePtr->set_nanos(static_cast<std::int32_t>(Timer::GetCurrentTimeNanos()));

        requestPtr->set_allocated_starttime(startTimePtr.get());
        requestPtr->set_from("Leipzig");
        requestPtr->set_to("Halle");
        requestPtr->set_publickey(publicKeyBuffer->str());

        return requestPtr;
    }

} // yakbas