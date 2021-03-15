#ifndef _DIGESTENGINE_H_
#define _DIGESTENGINE_H_

#include <string>
#include <vector>

namespace base {

class DigestEngine {
    /// This class is an abstract base class
    /// for all classes implementing a message
    /// digest algorithm, like MD5Engine
    /// and SHA1Engine.
    /// Call update() repeatedly with data to
    /// compute the digest from. When done,
    /// call digest() to obtain the message
    /// digest.
public:
    typedef std::vector<uint8_t> Digests;

    DigestEngine();
    virtual ~DigestEngine();
        
    void Update(const void* data, unsigned length);
    void Update(char data);
    void Update(const std::string& data);
        /// Updates the digest with the given data.
        
    virtual unsigned DigestLength() const = 0;
        /// Returns the length of the digest in bytes.

    virtual void Reset() = 0;
        /// Resets the engine so that a new
        /// digest can be computed.
        
    virtual const Digests& Digest() = 0;
        /// Finishes the computation of the digest and
        /// returns the message digest. Resets the engine
        /// and can thus only be called once for every digest.
        /// The returned reference is valid until the next
        /// time digest() is called, or the engine object is destroyed.

    static std::string DigestToHex(const Digests& bytes);
        /// Converts a message digest into a string of hexadecimal numbers.

protected:
    virtual void updateImpl(const void* data, unsigned length) = 0;
        /// Updates the digest with the given data. Must be implemented
        /// by subclasses.
        
private:
    DigestEngine(const DigestEngine&);
    DigestEngine& operator = (const DigestEngine&);
};


//
// inlines
//


inline void DigestEngine::Update(const void* data, unsigned length) {
    updateImpl(data, length);
}


inline void DigestEngine::Update(char data) {
    updateImpl(&data, 1);
}


inline void DigestEngine::Update(const std::string& data) {
    updateImpl(data.data(), (unsigned) data.size());    
}

}

#endif // _DIGESTENGINE_H_
