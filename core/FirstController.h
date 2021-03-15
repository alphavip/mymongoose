#ifndef _FIRSTCONTROLLER_H_ 
#define _FIRSTCONTROLLER_H_ 

#include "Controller.h"

typedef std::function<int32_t (HttpConn& conn, const char* fromuser, const char*)> DealFunc;


struct StockContent
{
    uint32_t id;
    std::string name;
    std::string area;
    std::string industry;

    void AsStringLine(std::string& line);
};

class FirstController :	public Controller
{
public:
    FirstController() {}
    virtual ~FirstController(){}

public:
    virtual int Init();
    void LoadStock();
    ErrorPtr OnTokenCheckReq(HttpConn& conn);
    ErrorPtr OnTest(HttpConn& conn);

private:
    int32_t getstock();
    int32_t getcharge(std::string& result);

    int32_t sendstock(HttpConn& conn, const char* fromuser, const char* touser, bool force);
    int32_t sendcharge(HttpConn& conn, const char* fromuser, const char* touser);

    bool CheckDay(uint32_t now);
    std::string stockcontent;
    uint32_t lasttime = 0;
    std::map<std::string, DealFunc> m_funcs;
    std::unordered_map<uint32_t, StockContent> m_stockinfo;
};

#endif //_FIRSTCONTROLLER_H_

