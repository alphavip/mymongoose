#include "HttpConn.h"
#include "Controller.h"


int Controller::RegHandlers(const std::string& name, HandleFunc func)
{
    assert(this->m_handlers.find(name) == this->m_handlers.end());

    this->m_handlers[name] = func;
    return 0;
}

void Controller::Dispath(const std::string& name, HttpConn& conn)
{
    auto it = this->m_handlers.find(name);
    if (it == this->m_handlers.end())
    {
        conn.Send404();
        return;
    }
    it->second(conn);
}


