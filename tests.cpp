
#include <sstream>
#include <cmath>
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

TEST(Sgfy, singleIntegerFmt)
{
    CHECK_EQUAL("4", str("%d", 4));
}

TEST(Sgfy, exclusivelyStreamOperator)
{
    const std::string result(str("%S", obj));

    CHECK_EQUAL(objStr, result);
}

TEST(Sgfy, streamPlaceholderAtTheEnd)
{
    const std::string& result(str("before %S", obj));

    CHECK_EQUAL("before " + objStr, result);
}

TEST(Sgfy, startWithStreamOperator)
{
    const std::string rest(" some text");
    const std::string result(str("%S" + rest, obj));

    CHECK_EQUAL(objStr + rest, result);
}

TEST(Sgfy, onlyStreamOperator)
{
    const std::string result(str("before %S between %S after", obj, obj));

    CHECK_EQUAL("before " + objStr + " between " + objStr + " after", result);
}

TEST(Sgfy, twoStrings)
{
    const std::string result(str("first %s, second %s, end", "string", "string"));

    CHECK_EQUAL("first string, second string, end", result);
}
TEST(Sgfy, multipleIntegerFmt)
{
    const std::string result(str("one: %d, two: %03d, three: %+03d", 1, 2, 3));
    const std::string expected("one: 1, two: 002, three: +03");

    CHECK_EQUAL(expected, result);
}

TEST(Sgfy, floatAndStringFmt)
{
    const std::string result(str("%s: %0.4f, %0.3E", "floating point", M_PI, M_PI));
    const std::string expected("floating point: 3.1416, 3.142E+00");

    CHECK_EQUAL(expected, result);
}

TEST(Sgfy, mixedFmtSpecifier)
{
    const std::string result(str("%d %d %d %s", 0, 1, 2, "three ", "%d %.1f", 4, 5.6123, " %u", 7));
    const std::string expected("0 1 2 three 4 5.6 7");

    CHECK_EQUAL(expected, result);
}

TEST(Sgfy, mixedFmtSpecifierWithStreamOperator)
{
    const std::string result(str("%d %.2f", -1, 1.234, " %S", obj, " %s", "---", " %S 1234", obj));
    const std::string expected("-1 1.23 " + objStr + " --- " + objStr + " 1234");

    CHECK_EQUAL(expected, result);
}

TEST(Sgfy, fmtWithUnmatchedArgs)
{
    const std::string result(str("%d %s", 10, "ten", 20, obj, obj));
    const std::string expected("10 ten");

    CHECK_EQUAL(expected, result);
}

int main(int argc, char **argv)
{
    return CommandLineTestRunner::RunAllTests(argc, argv);
}
