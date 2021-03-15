
#include "StringTokenizer.h"

namespace base {

StringTokenizer::StringTokenizer( const std::string& str, const std::string& tok, bool ignore_empty ) {
    (*this)(str, tok, ignore_empty);
    }
    
    
StringTokenizer& StringTokenizer::operator()( const std::string& str, const std::string& tok, bool ignore_empty )
{
    _sepStrings.clear();
    
    if(str.empty())
        return *this;
    
    std::string::const_iterator it1 = str.begin();
    std::string::const_iterator it2;
    std::string::const_iterator it3;
    std::string::const_iterator end = str.end();
    
    while (true)
    {
        it2 = it1;
        while (it2 != end && tok.find(*it2) == std::string::npos) ++it2;
        it3 = it2;
        if (ignore_empty)
        {
            if (it3 != it1)
                _sepStrings.push_back(std::string(it1, it3));
        }
        else
        {
            _sepStrings.push_back(std::string(it1, it3));
        }
        it1 = it2;
        if (it1 == end)
            break;
        ++ it1;
    }
    return *this;
}


void StringToken::Split(const std::string &str, const std::string &tok, std::vector<std::string> &out)
{
    out.clear();

    if(str.empty())
        return;

    std::string::const_iterator it1 = str.begin();
    std::string::const_iterator it2;
    std::string::const_iterator it3;
    std::string::const_iterator end = str.end();

    while (true)
    {
        it2 = it1;
        while (it2 != end && tok.find(*it2) == std::string::npos) ++it2;
        it3 = it2;
        out.push_back(std::string(it1, it3));

        it1 = it2;
        if (it1 == end)
            break;
        ++ it1;
    }
}


}
