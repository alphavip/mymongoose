#include "DigestEngine.h"

namespace base {

DigestEngine::DigestEngine() {
}


DigestEngine::~DigestEngine() {
}


std::string DigestEngine::DigestToHex(const Digests& bytes) {
    static const char digits[] = "0123456789abcdef";
    std::string result;
    result.reserve(bytes.size()*2);
    for (Digests::const_iterator it = bytes.begin(); it != bytes.end(); ++it) {
        uint8_t c = *it;
        result += digits[(c >> 4) & 0xF];
        result += digits[c & 0xF];
    }
    return result;
}

}
