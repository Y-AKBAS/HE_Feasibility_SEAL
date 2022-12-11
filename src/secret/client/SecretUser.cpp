
#include "SecretUser.h"

namespace yakbas::sec {

    SecretUser::SecretUser(const std::string &id, const std::string &firstName,
                           const std::string &lastName, const std::string &email,
                           std::unique_ptr<Address> &addressPtr,
                           std::unique_ptr<CustomSealOperations> &&customSealOperationsPtr)
            : BaseUser(id, firstName, lastName, email, addressPtr),
              m_customSealOperations(std::move(customSealOperationsPtr)) {}

    const std::unique_ptr<CustomSealOperations> &SecretUser::GetCustomSealOperations() const {
        return m_customSealOperations;
    }

    SecretUser::~SecretUser() = default;

} // yakbas