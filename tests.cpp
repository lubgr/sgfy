
#include <sstream>
#include <cmath>
#include <iomanip>
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

TEST(Sgfy, startWithNonFmtString)
{
    const char *noFmtStr = "No format string";
    const std::string result(str(noFmtStr, " %d", 10));
    const std::string expected = std::string(noFmtStr) + " 10";

    CHECK_EQUAL(expected, result);
}

TEST(Sgfy, nonLiteralString)
{
    const std::string fmt("%d %s");
    const std::string result(str(fmt, 1, "two"));
    const std::string expected("1 two");

    CHECK_EQUAL(expected, result);
}

TEST(Sgfy, twoNonLiteralStrings)
{
    const std::string fmt[2] = {"%d %s", " %u"};
    const std::string result(str(fmt[0], 1, "two", fmt[1], 3));
    const std::string expected("1 two 3");

    CHECK_EQUAL(expected, result);
}

TEST(Sgfy, streamWithFormatModification)
{
    const std::string expected("3.1 3.1 3.14159 46");
    const std::string result(str("%S%S %S ", std::setprecision(2), M_PI, M_PI,
                "%S%S", std::setprecision(6), M_PI, " %S%S", std::hex, 70));

    CHECK_EQUAL(expected, result);
}

TEST(Sgfy, printfDoc01)
{
    const std::string expected("\t[     Hello]\n\t[Hello     ]\n\t[     Hello]\n\t[Hell      ]\n\t"
            "[Hell      ]\n");
    const char* s = "Hello";
    const std::string result(str("\t[%10s]\n\t[%-10s]\n\t[%*s]\n\t[%-10.*s]\n\t[%-*.*s]\n", s, s,
                10, s, 4, s, 10, 4, s));

    CHECK_EQUAL(expected, result);
}

TEST(Sgfy, printfDoc02)
{
    const std::string expected("\t1 2 000003 0  +4 4294967295\n");
    const std::string result(str("\t%i %d %.6i %i %.0i %+i %u\n", 1, 2, 3, 0, 0, 4, -1));

    CHECK_EQUAL(expected, result);
}

TEST(Sgfy, printfDoc03)
{
    const std::string expected("\t5 a A 0x6\n");
    const std::string result(str("\t%x %x %X %#x\n", 5, 10, 10, 6));

    CHECK_EQUAL(expected, result);
}

TEST(Sgfy, printfDoc04)
{
    const std::string expected("\t12 012 04\n");
    const std::string result(str("\t%o %#o %#o\n", 10, 10, 4));

    CHECK_EQUAL(expected, result);
}

TEST(Sgfy, printfDoc05)
{
    const std::string expected("\t01.50 1.50  1.50\n");
    const std::string result(str("\t%05.2f %.2f %5.2f\n", 1.5, 1.5, 1.5));

    CHECK_EQUAL(expected, result);
}

TEST(Sgfy, printfDoc06)
{
    const std::string expected("\t0x1.8p+0 0X1.8P+0\n");
    const std::string result(str("\t%a %A\n", 1.5, 1.5));

    CHECK_EQUAL(expected, result);
}

TEST(Sgfy, printfDoc07)
{
    const std::string result(str("\t%c %%\n", 65, "'%*c'\n", 5, 'x', "'%*c'\n", -5, 'x'));
    const std::string expected("\tA %\n'    x'\n'x    '\n");

    CHECK_EQUAL(expected, result);
}

int main(int argc, char **argv)
{
    return CommandLineTestRunner::RunAllTests(argc, argv);
}
