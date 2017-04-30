
#include <sstream>
#include "sgfy.h"
#include <CppUTest/TestHarness.h>
#include <CppUTest/CommandLineTestRunner.h>

using namespace sgfy;

const std::string objStr = "[obj]";

struct Class {} obj;

std::ostream& operator << (std::ostream& stream, const Class& rhs)
{
    stream << objStr;

    return stream;
}

TEST_GROUP(Sgfy)
{
};

TEST(Sgfy, emptyString)
{
    const std::string result(str(""));

    CHECK(result.empty());
}

TEST(Sgfy, printfFmtWithoutArgs)
{
    const char *fmt = "%d %s %x";

    CHECK_EQUAL(fmt, str(fmt));
}

TEST(Sgfy, withOutPlaceholder)
{
    const char *plainString = "String without format specifier";

    CHECK_EQUAL(plainString, str(plainString));
}

TEST(Sgfy, exclusivelyStreamOperator)
{
    const std::string result(str("%S", obj));

    CHECK_EQUAL(objStr, result);
}

TEST(Sgfy, onlyStreamOperator)
{
    const std::string result(str("before %S between %S after", obj, obj));

    CHECK_EQUAL("before " + objStr + " between " + objStr + " after", result);
}

int main(int argc, char **argv)
{
    return CommandLineTestRunner::RunAllTests(argc, argv);
}
