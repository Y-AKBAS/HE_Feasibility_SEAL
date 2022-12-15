
#include "PlatformGenerator.h"
#include "Utils.h"
#include "Timer.h"

namespace yakbas::sec {
    using namespace yakbas::util;

    std::unique_ptr<std::vector<communication::sec::Ride>> PlatformGenerator::GenerateRides() {
        
        const auto first = GetUnique<communication::sec::Ride>();
        first->set_from("Leipzig");
        first->set_to("Halle");
        first->set_providerid(GetUUID());

        const auto timestamp = GetUnique<google::protobuf::Timestamp>();
        timestamp->set_nanos(static_cast<std::int32_t>(Timer::GetCurrentTimeNanos()));
        first->set_allocated_starttime(timestamp.get());
        //first->set_coefficient(GetRandomNumber());
        first->set_rideid(GetUUID());

        auto vectorPtr = GetUnique<std::vector<communication::sec::Ride>>();
        vectorPtr->push_back(*first);

        return vectorPtr;
    }

} // yakbas
