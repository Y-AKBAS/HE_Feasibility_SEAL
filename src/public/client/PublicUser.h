#pragma once

#include "BaseUser.h"

namespace yakbas::pub {

    class PublicUser : public BaseUser {
    public:
        PublicUser(const std::string &id, const std::string &firstName,
                   const std::string &lastName, const std::string &email,
                   std::unique_ptr<Address> &addressPtr);

        ~PublicUser() override;
    };

} // yakbas
