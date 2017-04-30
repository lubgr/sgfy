
#include <string>
#include <cstring>
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

    template<class S, class ...T> void append(std::ostream& stream, const std::string& fmt,
            const std::string& nextFmt, const S& firstArg, const T&... args)
    {
        std::ptrdiff_t n = getNumFmtSpecifier(fmt);

        stream << fmt;

        shiftOrContinue(stream, n, nextFmt, firstArg, args...);
    }

    std::ptrdiff_t getNumFmtSpecifier(const std::string& fmt)
        /* The regex pattern are constructed following the docs of std::printf. */
    {
        return 0;
    }

    template<class ...T> void splitAndAppend(std::ostream& stream, const std::string& fmt)
    {
        stream << fmt;
    }

    template<class S, class ...T> void splitAndAppend(std::ostream& stream, const std::string& fmt,
            const S& firstArg, const T&... args)
    {
        const size_t pos = fmt.find("%S");;

        if (fmt.length() == 0)
            return;
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
        std::stringstream stream;

        assert(fmt.find("%") != std::string::npos);

        splitAndAppend(stream, fmt, firstArg, args...);

        return stream.str();
    }

    template<class ...T> std::string str(const std::string& termination)
    {
        return termination;
    }
}
