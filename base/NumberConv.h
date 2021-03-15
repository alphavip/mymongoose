#ifndef _NUMBERCONV_H_
#define _NUMBERCONV_H_

#include <string>

namespace base {

inline int testRadix(const char*& str) {
    if(str[0] == '0') {
        if(str[1] == 'X' || str[1] == 'x') {
            str += 2;
            return 16;
        }
        str += 1;
    }
    return 10;
}

template<typename T>
struct Converter {
    static unsigned long long Convert(const char* str) {
        const char* s = str;
        int radix = testRadix(s);
        return strtoull(s, nullptr, radix);
    }
};

template<>
struct Converter<bool> {
    static bool Convert(const char* str) {
        return (std::string(str) == "true");
    }
};

template<>
struct Converter<double> {
    static double Convert(const char* str) {
        return atof(str);
    }
};

template<>
struct Converter<std::string> {
    static std::string Convert(const std::string& str) {
        return str;
    }
};

template<>
struct Converter<const char*> {
    static const char* Convert(const char* str) {
        return str;
    }
};

template<typename T>
struct TraitsType {
    typedef int ConverterType;
};

template<>
struct TraitsType<bool> {
    typedef bool ConverterType;
};

template<>
struct TraitsType<float> {
    typedef double ConverterType;
};

template<>
struct TraitsType<double> {
    typedef double ConverterType;
};

template<>
struct TraitsType<std::string> {
    typedef std::string ConverterType;
};

template<>
struct TraitsType<const char*> {
    typedef const char* ConverterType;
};

template<typename T>
T Convert(const char* str) {
    return static_cast<T>(Converter<typename TraitsType<T>::ConverterType>::Convert(str));
}

template<typename T>
T Convert(const std::string& str) {
    return static_cast<T>(Converter<typename TraitsType<T>::ConverterType>::Convert(str.c_str()));
}
    
}

#endif
