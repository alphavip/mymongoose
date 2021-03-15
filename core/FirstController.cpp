#include <iostream>
#include <curl/curl.h>
#include <vector>
#include <string>
#include <sstream>

#include "base/CsvParser.h"
#include "base/StringTokenizer.h"

#include "base/tinyxml2.h"

#include "HttpConn.h"

#include "FirstController.h"


static size_t ReadCallBack(void* ptr, size_t size, size_t nmemb, void* userdata)
{
    size_t relSize = nmemb * size;
    std::string& str = *((std::string*)(userdata));
    str.resize(relSize);
    memcpy(&str[0], ptr, relSize);
    return relSize;
}

bool FirstController::CheckDay(uint32_t now)
{
    return ((now - 4800) / 86400) == ((this->lasttime - 4800) / 86400);
}

void StockContent::AsStringLine(std::string& line)
{
    std::ostringstream oss;
    oss << id << "," << this->name << "," << this->area << "," << this->industry << "\r\n";
    line = oss.str();
}


int FirstController::getstock()
{
    uint32_t now = time(NULL);
    if(!CheckDay(now))
    {
        CURL * curl = curl_easy_init();
        if(curl != NULL)
        {
            struct tm _tm;
            time_t ts = now;
            gmtime_r(&ts, &_tm);

            char returl[256] = {0};
            
            //_tm.tm_mday = 26;
            char timestr[64] = {0};
            sprintf(timestr, "%4d-%02d-%02d", _tm.tm_year + 1900, _tm.tm_mon+1, _tm.tm_mday);
            sprintf(returl, "http://xxxxxxxx/best_497_top5/%s.csv", timestr);
            std::cout << returl;

            curl_easy_setopt(curl, CURLOPT_URL, returl);

            curl_easy_setopt(curl, CURLOPT_TIMEOUT, 10);//timeout 10s
    
            std::string rddata;
            curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, ReadCallBack);
            curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void*)(&rddata));
        

            CURLcode res = curl_easy_perform(curl);
            if(res != CURLE_OK)
                fprintf(stderr, "curl_easy_perform() failed: %s\n",
                curl_easy_strerror(res));
            curl_easy_cleanup(curl);
            if(rddata.find("404 Not Found") == std::string::npos)
            {
                this->stockcontent.clear();
                this->stockcontent += std::string(timestr) + std::string("\r\n");
                std::vector<std::string> lines;
                base::StringToken::Split(rddata, "\r\n", lines);
                for(auto& line : lines)
                {
                    std::vector<std::string> stocks;
                    base::StringToken::Split(line, ",", stocks);
                    if(stocks.size() > 3)
                    {
                        size_t pos = stocks[3].find('.');
                        if(pos != std::string::npos)
                        {
                            auto it = this->m_stockinfo.find(base::Convert<uint32_t>(stocks[3].substr(pos+1)));
                            if(it != this->m_stockinfo.end())
                            {
                                std::string stockinfo;
                                it->second.AsStringLine(stockinfo);
                                this->stockcontent = this->stockcontent + stockinfo;
                            }
                            else
                            {
                                this->stockcontent = this->stockcontent + stocks[3] + "\r\n";
                            }
                            
                        }
                    }
                }
                this->lasttime = now;
            }
            std::cout << this->stockcontent;
        }
    }
    return 0;
}

int FirstController::getcharge(std::string& result)
{
    uint32_t now = time(NULL);

    CURL * curl = curl_easy_init();
    if(curl != NULL)
    {
        struct tm _tm;
        time_t ts = now;
        gmtime_r(&ts, &_tm);

        curl_easy_setopt(curl, CURLOPT_URL, "");

        curl_easy_setopt(curl, CURLOPT_TIMEOUT, 10);//timeout 10s

        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, ReadCallBack);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void*)(&result));
    

        CURLcode res = curl_easy_perform(curl);
        if(res != CURLE_OK)
            fprintf(stderr, "curl_easy_perform() failed: %s\n",
            curl_easy_strerror(res));
        curl_easy_cleanup(curl);
    }


    return 0;
}

int FirstController::Init()
{
    getstock();
    RegFunc(FirstController::OnTokenCheckReq, this, "check");
    RegFunc(FirstController::OnTest, this, "test");

    this->m_funcs["stock"] = std::bind(&FirstController::sendstock, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, false);
    this->m_funcs["force"] = std::bind(&FirstController::sendstock, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, true);
    this->m_funcs["charge"] = std::bind(&FirstController::sendcharge, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3);

    return 0;
}

int32_t FirstController::sendstock(HttpConn& conn, const char* fromuser, const char* touser, bool force)
{
    if(force)
        this->lasttime = 0;

     this->getstock();

    if(!this->stockcontent.empty())
    {
        uint32_t now = time(NULL);
        char tmbuf[20] = {0};
        sprintf(tmbuf, "%u", now);
        std::string tmp;
        tmp = "<xml><ToUserName>" + std::string(fromuser) + "</ToUserName>"
        + "<FromUserName>" + std::string(touser) + "</FromUserName>"
        + "<CreateTime>" + std::string(tmbuf) + "</CreateTime>"
        + "<MsgType><![CDATA[text]]></MsgType>"
        + "<Content>" + this->stockcontent + "</Content></xml>";
        conn.SendXml(tmp.c_str(), tmp.length());
    }
    else
    {
        uint32_t now = time(NULL);
        char tmbuf[20] = {0};
        sprintf(tmbuf, "%u", now);
        std::string tmp;
        tmp = "<xml><ToUserName>" + std::string(fromuser) + "</ToUserName>"
        + "<FromUserName>" + std::string(touser) + "</FromUserName>"
        + "<CreateTime>" + std::string(tmbuf) + "</CreateTime>"
        + "<MsgType><![CDATA[text]]></MsgType>"
        + "<Content>再等等吧</Content></xml>";
        std::cout << tmp << std::endl;
        conn.SendXml(tmp.c_str(), tmp.length());
    }

    return 0;
}

void FirstController::LoadStock()
{
    base::CsvParser parser(',', '"');
    parser.Load("stock.csv");
    parser.Skip();
    const base::CsvParser::Row* rowp;
    while ((rowp = parser.Next()) != nullptr)
    {
        auto& row = *rowp;
        this->m_stockinfo[row.As<uint32_t>(2)] = {row.As<uint32_t>(2), row.As<std::string>(3), row.As<std::string>(4), row.As<std::string>(5)};
    }
    
}

int32_t FirstController::sendcharge(HttpConn& conn, const char* fromuser, const char* touser)
{
    std::string result;
    this->getcharge(result);

    if(!result.empty())
    {
        uint32_t now = time(NULL);
        char tmbuf[20] = {0};
        sprintf(tmbuf, "%u", now);
        std::string tmp;
        tmp = "<xml><ToUserName>" + std::string(fromuser) + "</ToUserName>"
        + "<FromUserName>" + std::string(touser) + "</FromUserName>"
        + "<CreateTime>" + std::string(tmbuf) + "</CreateTime>"
        + "<MsgType><![CDATA[text]]></MsgType>"
        + "<Content>" + result + "</Content></xml>";
        conn.SendXml(tmp.c_str(), tmp.length());
    }
    else
    {
        uint32_t now = time(NULL);
        char tmbuf[20] = {0};
        sprintf(tmbuf, "%u", now);
        std::string tmp;
        tmp = "<xml><ToUserName>" + std::string(fromuser) + "</ToUserName>"
        + "<FromUserName>" + std::string(touser) + "</FromUserName>"
        + "<CreateTime>" + std::string(tmbuf) + "</CreateTime>"
        + "<MsgType><![CDATA[text]]></MsgType>"
        + "<Content>出错了</Content></xml>";
        std::cout << tmp << std::endl;
        conn.SendXml(tmp.c_str(), tmp.length());
    }
    return 0;
}

ErrorPtr FirstController::OnTokenCheckReq(HttpConn& conn)
{
    if(conn.GetMethod() == HttpMethod_Get)
    {
        static const char* token = "";

        std::cout << "-----------Get-----------" << std::endl;
        std::cout << "body:" << conn.GetBody() << std::endl;
        std::cout << "qurrystr:" << conn.GetQurryStr() << std::endl;

        const std::string& echostr = conn.GetParam("echostr");
        conn.SendChunk(echostr.c_str(), echostr.length());
        return nullptr;
    }
    else if(conn.GetMethod() == HttpMethod_Post)
    {
        std::cout << "-----------Post-----------" << std::endl;
        std::cout << "body:" << conn.GetBody() << std::endl;
        std::cout << "qurrystr:" << conn.GetQurryStr() << std::endl;
using namespace tinyxml2;

        XMLDocument doc;
        XMLError error = doc.Parse(conn.GetBody().c_str(), conn.GetBody().length());
        if(error != XML_SUCCESS)
        {
            conn.SendChunk("xml error", 9);
            return 0;
        }
        XMLElement* pRoot = doc.RootElement();
        if(pRoot == nullptr)
        {
            conn.SendChunk("xml error", 9);
            return 0;            
        }
        auto* pNode = pRoot->FirstChildElement("ToUserName");
        if(pNode == nullptr)
        {
            conn.SendChunk("xml error", 9);
            return 0;
        }
        const char* touser = pNode->GetText();
        pNode = pNode->NextSiblingElement("FromUserName");
        if(pNode == nullptr)
        {
            conn.SendChunk("xml error", 9);
            return 0;
        }
        const char* fromuser = pNode->GetText();
        pNode = pNode->NextSiblingElement("Content");
        if(pNode == nullptr)
        {
            conn.SendChunk("xml error", 9);
            return 0;
        }
        const char* recvContent = pNode->GetText();
        auto it = this->m_funcs.find(recvContent);
        if(it != this->m_funcs.end())
        {
            it->second(conn, fromuser, touser);
        }
        else
        {
            uint32_t now = time(NULL);
            char tmbuf[20] = {0};
            sprintf(tmbuf, "%u", now);
            std::string tmp;
            tmp = "<xml><ToUserName>" + std::string(fromuser) + "</ToUserName>"
            + "<FromUserName>" + std::string(touser) + "</FromUserName>"
            + "<CreateTime>" + std::string(tmbuf) + "</CreateTime>"
            + "<MsgType><![CDATA[text]]></MsgType>"
            + "<Content>请输入正确指令！</Content></xml>";
            std::cout << tmp << std::endl;
            conn.SendXml(tmp.c_str(), tmp.length());
        }
    }

    return nullptr;
}

ErrorPtr FirstController::OnTest(HttpConn& conn)
{
    std::cout << "body:" << conn.GetBody() << std::endl;
    std::cout << "querry:" << conn.GetQurryStr() << std::endl;

    std::string out(conn.GetMessage().message.p, conn.GetMessage().message.len);
    std::cout << "message:" << out << std::endl;

    conn.SendChunk("hello world!", strlen("hello world!"));


    return nullptr;
}