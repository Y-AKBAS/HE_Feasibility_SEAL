
#include "TransporterServiceImpl.h"

namespace yakbas::sec {

    TransporterServiceImpl::TransporterServiceImpl(const SealKeys &sealKeys)
            : m_customSealOperationsPtr(std::make_unique<CustomSealOperations>(sealKeys)),
              m_logger(std::make_unique<log4cplus::Logger>(
                      log4cplus::Logger::getInstance("Secret Transporter Service Impl"))) {}

} // yakbas