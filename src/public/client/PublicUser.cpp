
#include "PublicUser.h"

namespace yakbas::pub {

    PublicUser::PublicUser(const std::string &id, const std::string &firstName, const std::string &lastName,
                           const std::string &email, std::unique_ptr<Address> &addressPtr)
            : BaseUser(id, firstName, lastName, email, addressPtr) {

    }

    PublicUser::~PublicUser() = default;

} // yakbas