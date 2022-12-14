#pragma once

#include <log4cplus/logger.h>
#include "BaseClientManager.h"
#include "PublicUser.h"
#include "PublicCommunication.grpc.pb.h"
#include "PublicBaseClientManager.h"


namespace yakbas::pub {

    class ClientManager : public PublicBaseClientManager {
    public:

        ClientManager();

        [[nodiscard]] std::unique_ptr<std::vector<std::unique_ptr<communication::Journey>>>
        Search(const std::string &from, const std::string &to, int numberOfJourneys);

        [[nodiscard]] std::unique_ptr<communication::BookingResponse>
        Book(const communication::Journey &journey);

        [[nodiscard]] std::unique_ptr<communication::InvoicingResponse>
        Pay(const communication::BookingResponse &bookingResponse);

        [[nodiscard]] communication::StatusCode
        ReportInvoicing(const communication::InvoicingResponse &invoicingResponse,
                        const communication::BookingResponse &bookingResponse) const;

        static bool IsInitialized();

    private:
        const std::unique_ptr<PublicUser> m_userPtr{nullptr};
        std::unique_ptr<log4cplus::Logger> m_logger{nullptr};

        static std::once_flag m_isInitialized;

        static void MapRideAndSeatNumberMap(google::protobuf::Map<std::string, int32_t> &targetMap,
                                            const google::protobuf::Map<std::string, int32_t> *sourceMapPtr);
    };

} // yakbas
