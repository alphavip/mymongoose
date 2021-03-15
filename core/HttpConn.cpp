#include "base/mongoose.h"
#include "base/StringTokenizer.h"
#include "base/Url.h"

#include "HttpConn.h"

const static std::string emptystr("");

const std::string& HttpConn::GetParam(const std::string& name) const
{
    auto it = this->m_param.find(name);
    if(it != this->m_param.end())
        return it->second;
    return emptystr;
}

void HttpConn::Parse()
{
    std::string* paramtmp = nullptr;
    std::string method = std::move(std::string(this->m_hm->method.p, this->m_hm->method.len));
    if(method == "GET" || method == "get")
    {
        this->m_method = HttpMethod_Get;
        if(this->m_hm->query_string.p != nullptr)
            this->m_qurrystr = std::move(std::string(this->m_hm->query_string.p, this->m_hm->query_string.len));
        paramtmp = &this->m_qurrystr;
    }
    else if(method == "POST" || method == "post")
    {
        this->m_method = HttpMethod_Post;
        if(this->m_hm->body.p != nullptr)
            this->m_body = std::move(std::string(this->m_hm->body.p, this->m_hm->body.len));
        paramtmp = &this->m_body;
    }
    else if(method == "PUT" || method == "put")
    {
        this->m_method = HttpMethod_Put;
    }
    else if(method == "DELETE" || method == "delete")
    {
        this->m_method = HttpMethod_Delete;
    }
    else  if(method == "PATCH" || method == "patch")
    {
        this->m_method = HttpMethod_Patch;
    }
    else
    {
        this->m_method = HttpMethod_Other;
    }

    if(paramtmp != nullptr)
    {
        std::vector<std::string> out;
        base::StringToken::Split(*paramtmp, "&", out);
        for(auto& str : out)
        {
            std::vector<std::string> out2;
            base::StringToken::Split(str, "=", out2);
            if(out2.size() > 1)
            {
                std::string decodeout;
                UrlDecode(out2[1], decodeout);
                this->m_param[out2[0]] = std::move(decodeout);
            }
        }
    }

}


