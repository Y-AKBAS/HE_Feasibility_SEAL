
#include "BaseUser.h"

#include <utility>

namespace yakbas {

    BaseUser::BaseUser(std::string id, std::string firstName,
                       std::string lastName, std::string email,
                       std::unique_ptr<Address> &addressPtr)
            : m_id(std::move(id)),
              m_firstName(std::move(firstName)),
              m_lastName(std::move(lastName)),
              m_email(std::move(email)),
              m_addressPtr(std::move(addressPtr)) {}

    BaseUser::~BaseUser() = default;

    const std::string &BaseUser::GetId() const {
        return m_id;
    }

    const std::string &BaseUser::GetFirstName() const {
        return m_firstName;
    }

    const std::string &BaseUser::GetLastName() const {
        return m_lastName;
    }

    const std::string &BaseUser::GetEmail() const {
        return m_email;
    }

    const std::unique_ptr<BaseUser::Address> &BaseUser::GetAddressPtr() const {
        return m_addressPtr;
    }

    std::unique_ptr<communication::ProtoUser> BaseUser::ToProto() const {

        auto protoUserPtr = std::make_unique<communication::ProtoUser>();
        protoUserPtr->set_id(this->m_id);
        protoUserPtr->set_firstname(this->m_firstName);
        protoUserPtr->set_lastname(this->m_lastName);
        protoUserPtr->set_email(this->m_email);

        auto addressPtr = std::make_unique<communication::Address>();
        addressPtr->set_street(this->m_addressPtr->m_street);
        addressPtr->set_housenumber(this->m_addressPtr->m_houseNumber);
        addressPtr->set_postalcode(this->m_addressPtr->m_postalCode);
        addressPtr->set_city(this->m_addressPtr->m_city);
        addressPtr->set_country(this->m_addressPtr->m_country);

        protoUserPtr->set_allocated_address(addressPtr.get());
        return protoUserPtr;
    }

    BaseUser::Address::Address(std::string street, std::string houseNumber, std::string postalCode,
                               std::string city, std::string country)
            : m_street(std::move(street)),
              m_houseNumber(std::move(houseNumber)),
              m_postalCode(std::move(postalCode)),
              m_city(std::move(city)),
              m_country(std::move(country)) {}
} // yakbas