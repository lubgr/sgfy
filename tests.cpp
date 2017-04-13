
#include "sgfy.h"
#include "CppUTest/TestHarness.h"

using namespace sgfy;

TEST_GROUP(Sgfy) {};

TEST(Sgfy, withOutPlaceholder)
{
    const char *plainString = "String without format specifier";

    CHECK_EQUAL(plainString, str(plainString));
}
