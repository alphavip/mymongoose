#ifndef _ERROR_H_
#define _ERROR_H_

#include <string>
#include <memory>


class Error;

typedef std::shared_ptr<Error> ErrorPtr;

class Error
{
public:
	Error(int c, const std::string& m) : m_code(c), m_msg(m) {}

public:
	static ErrorPtr Create(int c, std::string& msg)
	{
		return ErrorPtr(new Error(c, msg));
	}

public:
	int GetCode()const { return m_code; }
	const std::string& GetMsg()const { return m_msg; }

private:
	int m_code;
	std::string m_msg;
};

#endif // !_ERROR_H_


