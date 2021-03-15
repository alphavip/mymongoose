#include <iostream>
#include "base/StringTokenizer.h"

#include "HttpConn.h"

#include "MgMgr.h"

void Handler(struct mg_connection* c, int ev, void* p)
{
    MgMgr* pMgr = reinterpret_cast<MgMgr*>(c->mgr->user_data);
    pMgr->EventHandle(c, ev, p);
}

int MgMgr::Init(const std::string& host)
{
    using namespace std::placeholders;
    mg_mgr_init(&this->m_mgr, this);

    struct mg_connection* conn = mg_bind(&this->m_mgr, host.c_str(), Handler);
    if(conn == nullptr)
    {
        std::cout << "create web sock error!" << std::endl;
        return -1;
    }
    mg_set_protocol_http_websocket(conn);

    return 0;
}

void MgMgr::EventHandle(struct mg_connection *c, int ev, void *p)
{
    switch (ev)
    {
        case MG_EV_HTTP_REQUEST:
        {
            struct http_message* hm = reinterpret_cast<struct http_message*>(p);
            HttpConn conn(c, hm, ev);
            conn.Parse();
            std::vector<std::string> out;
            base::StringToken::Split(std::string(hm->uri.p, hm->uri.len), "/", out);
            if (out.size() < 3)
            {
                conn.Send404();
                return;
            }
            auto it = this->m_controllers.find(out[1]);
            if (it != this->m_controllers.end())
            {
                it->second->Dispath(out[2], conn);
            }
            else
            {
                conn.Send404();
                return;
            }
        }
        break;
        case MG_EV_HTTP_REPLY:
        {
            
        }
        break;
        default:
            break;
    }
}

void MgMgr::UnInit()
{
    mg_mgr_free(&this->m_mgr);
}

void MgMgr::Update(uint32_t interval)
{
    mg_mgr_poll(&this->m_mgr, interval);
}
