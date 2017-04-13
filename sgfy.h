
#include <string>

namespace sgfy {
    template<class ...T> std::string str(const char *fmt, const T&... args)
    {
        std::string fmtStr(fmt);

        return fmtStr;
    }
}
