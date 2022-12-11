
#include "BaseUser.h"
#include "CustomSealOperations.h"

namespace yakbas::sec {

    class SecretUser : public BaseUser {
    public:
        SecretUser(const std::string &id, const std::string &firstName,
                   const std::string &lastName, const std::string &email,
                   std::unique_ptr<Address> &addressPtr,
                   std::unique_ptr<CustomSealOperations> &&customSealOperationsPtr);

        ~SecretUser() override;

        [[nodiscard]] const std::unique_ptr<CustomSealOperations> &GetCustomSealOperations() const;

    private:
        const std::unique_ptr<CustomSealOperations> m_customSealOperations;
    };

} // yakbas
