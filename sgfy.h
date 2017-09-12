#ifndef SGFY_H
#define SGFY_H

#include <cassert>
#include <vector>
#include <regex>
#include <iostream>
#include <string>
#include <cstdarg>
#include <sstream>

namespace sgfy {
    inline std::sregex_iterator *getFmtMatches(const std::string& fmt)
    {
        static const std::regex fmtPattern(
                "%"
                "("
                "("
                "[-+ #0]*"
                "("
                "([0-9*]*(\\.[0-9*]*)?)"
                "|"
                "(\\*(\\.[0-9*]*)?)"
                "|"
                "([0-9*]*\\.\\*)"
                ")"
                "(h|hh|l|ll|L|z|j|t)?[csdioxXufFeEaAgGnp]"
                ")"
                "|"
                "S"
                ")"
                );

        return new std::sregex_iterator(fmt.begin(), fmt.end(), fmtPattern);
    }

    typedef std::iterator_traits<std::string::const_iterator>::difference_type difference_type;

    inline difference_type getLength(std::sregex_iterator *match)
    {
        return (*match)->length();
    }

    inline difference_type getPos(std::sregex_iterator *match)
    {
        return (*match)->position();
    }

    inline bool isAtEnd(std::sregex_iterator *match)
    {
        return *match == std::sregex_iterator();
    }

    inline void increment(std::sregex_iterator *match)
    {
        ++(*match);
    }

    inline std::string getStr(std::sregex_iterator *match)
    {
        return (*match)->str();
    }

    inline void append(std::ostream& stream, const char *fmt, std::va_list args)
    {
        std::va_list argsCopy;
        std::size_t bufferSize;
        int charsWritten;
        char *buffer;

        va_copy(argsCopy, args);

        charsWritten = std::vsnprintf(NULL, 0, fmt, args);

        if (charsWritten < 0) {
            std::cerr << "Couldn't build string via format specifier" << std::endl;
            bufferSize = 10000;
        } else
            bufferSize = static_cast<size_t>(charsWritten + 1);

        buffer = new char[bufferSize];

        std::vsnprintf(buffer, bufferSize, fmt, argsCopy);

        stream << buffer;

        delete[] buffer;
    }

    inline void macroAppend(std::ostream& stream, const char *fmt, ...)
    {
        std::va_list args;

        va_start(args, fmt);

        append(stream, fmt, args);

        va_end(args);
    }

    template<class ...T> void append(std::ostream& stream, const std::string& fmt, const T&... args)
    {
        macroAppend(stream, fmt.c_str(), args...);
    }

    class ArgProcessor {
        public:
            ArgProcessor(std::ostream& stream, const std::string& fmt) :
                match(getFmtMatches(fmt)),
                stream(stream),
                fmt(fmt),
                partialFmtLength(0),
                nAsterisks(0),
                pos(0)
        {}

            ~ArgProcessor()
            {
                delete match;
            }

            template<class T> void nextArg(const T& arg)
            {
                if (isAtEnd(match)) {
                    std::cerr << "Argument without format specifier: " << arg << std::endl;
                    return;
                } else
                    return nextArgWithFmt(arg);
            }

        private:

            template<class T> void nextArgWithFmt(const T& arg)
            {
                updateCurrentMatchVars();

                if (getStr(match) == "%S") {
                    append(stream, fmt.substr(pos, partialFmtLength - 2));
                    stream << arg;
                } else if (nAsterisks > 0 && asterisksWidth.size() == nAsterisks)
                    appendVarWidth(arg);
                else if (nAsterisks > 0) {
                    storeVariableFieldWidth(arg);
                    return;
                } else {
                    assert(std::is_trivial<T>::value);
                    append(stream, fmt.substr(pos, partialFmtLength), arg);
                }

                nextMatchOrFinalize();
            }

            void updateCurrentMatchVars()
            {
                partialFmtLength = static_cast<size_t>(getPos(match) + getLength(match)) - pos;
                nAsterisks = numberOfAsterisksInMatch();
            }

            size_t numberOfAsterisksInMatch() const
            {
                const size_t first = getStr(match).find('*');

                if (first == std::string::npos)
                    return 0;
                else if (getStr(match).rfind('*') == first)
                    return 1;
                else
                    return 2;
            }

            template<class T> void appendVarWidth(const T& arg)
            {
                if (nAsterisks == 1)
                    append(stream, fmt.substr(pos, partialFmtLength), asterisksWidth[0], arg);
                else
                    append(stream, fmt.substr(pos, partialFmtLength), asterisksWidth[0],
                            asterisksWidth[1], arg);

                asterisksWidth.clear();
            }

            template<class T> void storeVariableFieldWidth(const T&)
            {
                assert(false);
            }

            void storeVariableFieldWidth(const int& width)
            {
                asterisksWidth.push_back(width);
            }

            void nextMatchOrFinalize()
            {
                pos += partialFmtLength;

                increment(match);

                if (isAtEnd(match) && pos < fmt.length())
                    append(stream, fmt.substr(pos));
            }

            std::vector<int> asterisksWidth;
            std::sregex_iterator *match;
            std::ostream& stream;
            const std::string& fmt;
            size_t partialFmtLength;
            size_t nAsterisks;
            size_t pos;
    };

    template<class S, class ...T> std::string str(const std::string& fmt, const S& firstArg,
            const T&... args)
    {
        std::ostringstream stream;
        ArgProcessor ap(stream, fmt);
        using expander = int[];

        ap.nextArg(firstArg);

        (void) expander{ 0, (ap.nextArg(args), void(), 0)... };

        return stream.str();
    }

    template<class ...T> std::string str(const std::string& termination)
    {
        return termination;
    }
}

#endif
