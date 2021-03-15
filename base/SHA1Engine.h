#ifndef _SHA1ENGINE_H_
#define _SHA1ENGINE_H_


#include "DigestEngine.h"

namespace base {

class SHA1Engine: public DigestEngine {
    /// This class implementes the SHA-1 message digest algorithm.
    /// (FIPS 180-1, see http://www.itl.nist.gov/fipspubs/fip180-1.htm)
public:
    enum {
        BLOCK_SIZE  = 64,
        DIGEST_SIZE = 20
    };

    SHA1Engine();
    ~SHA1Engine();
        
    unsigned DigestLength() const;
    void Reset();
    const DigestEngine::Digests& Digest();

protected:
    void updateImpl(const void* data, unsigned length);

private:
    void transform();
    static void byteReverse(uint32_t* buffer, int byteCount);

    typedef uint8_t BYTE;

    struct Context {
        uint32_t digest[5]; // Message digest
        uint32_t countLo;   // 64-bit bit count
        uint32_t countHi;
        uint32_t data[16];  // SHA data buffer
        uint32_t slop;      // # of bytes saved in data[]
    };

    Context context;
    DigestEngine::Digests digests;

    SHA1Engine(const SHA1Engine&);
    SHA1Engine& operator = (const SHA1Engine&);
};

}

#endif // _SHA1ENGINE_H_
