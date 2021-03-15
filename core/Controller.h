#ifndef _CONTROLLER_H_ 
#define _CONTROLLER_H_ 


#include <string>
#include <map>
#include <functional>
#include <unordered_map>

#include "base/mongoose.h"
#include "base/Error.h"


#define RegFunc(func, ins, name) \
    RegHandlers(name, std::bind(&func, ins, std::placeholders::_1))

class HttpConn;
class Controller
{
public:
    Controller() {}
    virtual ~Controller() {}

public:
    virtual int Init() = 0;

public:
    typedef std::function<ErrorPtr (HttpConn& conn)> HandleFunc;

    int RegHandlers(const std::string& name, HandleFunc func);
    void Dispath(const std::string& name, HttpConn& conn);

private:
    std::unordered_map<std::string, HandleFunc> m_handlers;
};
#endif //_CONTROLLER_H_