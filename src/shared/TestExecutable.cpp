
#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN

#include "doctest/doctest.h"

TEST_SUITE("Generic Application Run Tester"){

    TEST_CASE("PRINT_MESSAGE"){
        std::cout << "Generic Application Run Tester" << std::endl;
    }
}
