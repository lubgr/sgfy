
#include <string>
#include <cstring>
#include <cstdarg>
#include <regex>
#include <sstream>
#include <iostream>
#include <cassert>

namespace sgfy {
    std::ptrdiff_t getNumFmtSpecifier(const std::string& fmt);
    template<class S, class ...T> void splitAndAppend(std::ostream& stream,
            const std::string& nextFmt, const S& firstArg, const T&... args);

    void shiftOrContinue(std::ostream& stream, std::ptrdiff_t& nLeft, const std::string& nextFmt)
    {
        assert(nLeft == 0);

        (void) nLeft;

        stream << nextFmt;
    }

    template<class S, class... T> void shiftOrContinue(std::ostream& stream, std::ptrdiff_t& nLeft,
            const std::string& nextFmt, const S& firstArg, const T&... args)
    {
        if (nLeft > 0)
            shiftOrContinue(stream, --nLeft, nextFmt, args...);
        else
            splitAndAppend(stream, nextFmt, firstArg, args...);
    }

    template<class T> typename
        std::enable_if<std::is_trivial<T>::value, const T&>::type pass(const T& arg)
    {
        return arg;
    }

    template<class T> typename
        std::enable_if<!std::is_trivial<T>::value, std::nullptr_t>::type pass(const T&)
    {
        return nullptr;
    }

    void variadicAppend(std::ostream& stream, const char *fmt, std::va_list args)
    {
        std::va_list argsCopy;
        std::size_t bufferSize;
        int charsWritten;
        char *buffer;

        va_copy(argsCopy, args);

        charsWritten = std::vsnprintf(NULL, 0, fmt, args);

        if (charsWritten < 0) {
            std::cerr << "Couldn't construct message from format specifier" << std::endl;
            bufferSize = 10000;
        } else
            bufferSize = static_cast<size_t>(charsWritten + 1);

        buffer = new char[bufferSize];

        std::vsnprintf(buffer, bufferSize, fmt, argsCopy);

        stream << buffer;

        delete[] buffer;
    }

    void variadicAppend(std::ostream& stream, const char *fmt, ...)
    {
        std::va_list args;

        va_start(args, fmt);

        variadicAppend(stream, fmt, args);

        va_end(args);
    }

    template<class S, class ...T> void append(std::ostream& stream, const std::string& fmt,
            const std::string& nextFmt, const S& firstArg, const T&... args)
    {
        std::ptrdiff_t n = getNumFmtSpecifier(fmt);

        /* Passing a plain argument pack to the type-unsafe variadic append method doesn't compile
         * with clang and is only conditionally implemented in g++, because macro-based variadic
         * function calls for non-trivial argument types are "conditionally-supported with
         * implementation-defined semantics" (C++11, §5.2.2/7). It's possible though to turn
         * non-trivial arguments into null-pointer, which can't discard information because there
         * are no format specifier for non-trivial types and such a call would be illegal anyhow. */
        variadicAppend(stream, fmt.c_str(), firstArg, pass(args)...);

        shiftOrContinue(stream, n, nextFmt, firstArg, args...);
    }

    std::ptrdiff_t count(const std::string& fmt, const std::string& pattern)
    {
        const std::regex regex(pattern);
        const auto start(std::sregex_iterator(fmt.begin(), fmt.end(), regex));

        return std::distance(start, std::sregex_iterator());
    }

    std::ptrdiff_t getNumFmtSpecifier(const std::string& fmt)
        /* The regex pattern are constructed following the docs of std::printf. */
    {
        static const std::string start("%[-+ #0]*");
        static const std::string end("(h|hh|l|ll|L|z|j|t)?[csdioxXufFeEaAgGnp%]");
        const std::ptrdiff_t nFmtSpecifier = count(fmt, start + "[0-9*]*(\\.[0-9*]*)?"  + end);
        const std::ptrdiff_t nAdditionalFirstInt = count(fmt, start + "\\*(\\.[0-9*]*)?" + end);
        const std::ptrdiff_t nAdditionalSecondInt = count(fmt, start + "[0-9*]*\\.\\*" + end);
        const std::ptrdiff_t nPercentFmtSpecifier = count(fmt, "%%");

        return nFmtSpecifier - nPercentFmtSpecifier + nAdditionalFirstInt + nAdditionalSecondInt;
    }

    template<class ...T> void splitAndAppend(std::ostream& stream, const std::string& fmt)
    {
        stream << fmt;
    }

    template<class S, class ...T> void splitAndAppend(std::ostream& stream, const S&, const T&...)
    {
        return;
    }

    template<class S, class ...T> void splitAndAppend(std::ostream& stream, const char *fmt,
            const S& firstArg, const T&... args)
    {
        splitAndAppend(stream, std::string(fmt), firstArg, args...);
    }

    template<class S, class ...T> void splitAndAppend(std::ostream& stream, const std::string& fmt,
            const S& firstArg, const T&... args)
    {
        const size_t pos = fmt.find("%S");;

        if (fmt.length() == 0)
            splitAndAppend(stream, firstArg, args...);
        else if (pos == std::string::npos)
            append(stream, fmt, "", firstArg, args...);
        else if (pos == 0) {
            stream << firstArg;
            splitAndAppend(stream, fmt.substr(2), args...);
        } else
            append(stream, fmt.substr(0, pos), fmt.substr(pos), firstArg, args...);
    }

    template<class S, class ...T> std::string str(const std::string& fmt, const S& firstArg,
            const T&... args)
    {
        std::ostringstream stream;

        splitAndAppend(stream, fmt, firstArg, args...);

        return stream.str();
    }

    template<class ...T> std::string str(const std::string& termination)
    {
        return termination;
    }
}
