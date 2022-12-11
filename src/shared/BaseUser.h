
#include <string>
#include <memory>
#include "SharedCommunication.pb.h"

namespace yakbas {

    class BaseUser {
    public:
        struct Address;

        BaseUser(std::string id, std::string firstName, std::string lastName,
                 std::string email, std::unique_ptr<Address> &addressPtr);

        virtual ~BaseUser();

        [[nodiscard]] const std::string &GetId() const;

        [[nodiscard]] const std::string &GetFirstName() const;

        [[nodiscard]] const std::string &GetLastName() const;

        [[nodiscard]] const std::string &GetEmail() const;

        [[nodiscard]] const std::unique_ptr<Address> &GetAddressPtr() const;

        [[nodiscard]] std::unique_ptr<communication::ProtoUser> ToProto() const;

    protected:
        const std::string m_id{};
        const std::string m_firstName{};
        const std::string m_lastName{};
        const std::string m_email{};
        const std::unique_ptr<Address> m_addressPtr{};

    private:

    };

    struct BaseUser::Address {
        const std::string m_street{};
        const std::string m_houseNumber;
        const std::string m_postalCode{};
        const std::string m_city{};
        const std::string m_country{};

        Address() {}

        Address(std::string street, std::string houseNumber, std::string postalCode,
                std::string city, std::string country);
    };

} // yakbas
