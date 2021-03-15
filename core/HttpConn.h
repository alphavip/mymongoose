#ifndef _HTTPCONN_H_
#define _HTTPCONN_H_

#include <map>
#include <string>
#include "base/mongoose.h"

enum HttpMethod
{
    HttpMethod_Get = 0,
    HttpMethod_Post = 1,
    HttpMethod_Put = 2,
    HttpMethod_Delete = 3,
    HttpMethod_Patch = 4,
    HttpMethod_Other = 5,
};

class HttpConn
{
public:
    HttpConn(mg_connection* conn, http_message* hm, int ev) : m_conn(conn), m_hm(hm), m_ev(ev){}
    ~HttpConn(){}

public:
    const std::map<std::string, std::string>& GetParam() const { return m_param; }
    const std::string GetUri() { return std::string(m_hm->uri.p, m_hm->uri.p); }
    int GetMethod() const { return m_method; }
    const std::string& GetParam(const std::string& name) const;
    struct http_message& GetMessage() { return *this->m_hm; }

public:
    const std::string& GetBody() { return this->m_body; }
    const std::string& GetQurryStr() { return this->m_qurrystr; }

public:
    void Parse();

public:
    void Send501()
    {
        mg_printf(this->m_conn, "HTTP/1.1 501 Internal Server Error\r\nContent-Length: 0\r\n\r\n");
    }
    void Send404()
    {
        mg_printf(this->m_conn, "HTTP/1.1 404 Not Found Error\r\nContent-Length: 0\r\n\r\n");
    }
    
    void SendXml(const char* data, size_t len)
    {
        mg_printf(this->m_conn, "HTTP/1.1 200 OK\r\nContent-Type: text/xml; charset: utf-8\r\nTransfer-Encoding: chunked\r\n\r\n");
        mg_send_http_chunk(this->m_conn, data, len);
    }

    void SendChunk(const char* data, size_t len)
    {
        mg_printf(this->m_conn, "%s", "HTTP/1.1 200 OK\r\nTransfer-Encoding: chunked\r\n\r\n");
        mg_send_http_chunk(this->m_conn, data, len);
        mg_send_http_chunk(this->m_conn, "", 0);
    }


private:
    int m_ev = MG_EV_HTTP_REQUEST;
    int m_method = 0;
    struct mg_connection* m_conn;
    struct http_message* m_hm;
    std::map<std::string, std::string> m_param;
    std::string m_body;
    std::string m_qurrystr;
};

#endif //_HTTPCONN_H_