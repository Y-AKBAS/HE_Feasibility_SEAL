
#include "Utils.h"

namespace yakbas::util {

    const auto optionalStreamLambda = std::make_optional([](std::stringstream &stream) {
        stream.exceptions(std::ios::badbit | std::ios::failbit);
    });

    std::shared_ptr<std::stringstream> getSharedStream() {
        return getModifiedShared<std::stringstream>(optionalStreamLambda);
    }

    std::unique_ptr<std::stringstream> getUniqueStream() {
        return getModifiedUnique<std::stringstream>(optionalStreamLambda);
    }

    std::ostream &operator<<(std::ostream &os, std::byte b) {
        return os << std::bitset<8>(std::to_integer<int>(b));
    }

}