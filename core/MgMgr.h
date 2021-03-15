#ifndef _MGMGR_H_ 
#define _MGMGR_H_

#include "Controller.h"
#include "base/Singleton.h"

class MgMgr
{
public:
    MgMgr() {}
    virtual ~MgMgr() 
    {
        for (auto& pair : this->m_controllers)
            delete pair.second;
        this->m_controllers.clear();
    }

    int Init(const std::string& host);
    void UnInit();
    void Update(uint32_t interval);

public:
    int AddController(const std::string& name, Controller* pcontroller) { this->m_controllers[name] = pcontroller;  return 0; }

public:
    void EventHandle(struct mg_connection *c, int ev, void *p);

private:
    struct mg_mgr m_mgr;
    std::unordered_map<std::string, Controller*> m_controllers;
};


#endif //_MGMGR_H_