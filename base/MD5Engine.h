#ifndef _MD5ENGINE_H_
#define _MD5ENGINE_H_


#include "DigestEngine.h"


namespace base {


class MD5Engine: public DigestEngine
	/// This class implementes the MD5 message digest algorithm,
	/// described in RFC 1321.
{
public:
	enum
	{
		BLOCK_SIZE  = 64,
		DIGEST_SIZE = 16
	};

	MD5Engine();
	~MD5Engine();

	unsigned DigestLength() const;
	void Reset();
	const DigestEngine::Digests& Digest();

protected:
	void updateImpl(const void* data, unsigned length);

private:
	static void transform(uint32_t state[4], const unsigned char block[64]);
	static void encode(unsigned char* output, const uint32_t* input, unsigned int len);
	static void decode(uint32_t* output, const unsigned char* input, unsigned int len);

	struct Context
	{
		uint32_t state[4];          // state (ABCD)
		uint32_t count[2];          // number of bits, modulo 2^64 (lsb first)
		unsigned char buffer[64]; // input buffer
	};

	Context _context;
	DigestEngine::Digests _digest;

	MD5Engine(const MD5Engine&);
	MD5Engine& operator = (const MD5Engine&);
};


} // namespace System


#endif // _MD5ENGINE_H_

