#ifndef _CSVPARSER_H_
#define _CSVPARSER_H_

#include "NumberConv.h"
#include "StringTokenizer.h"




namespace base {

class CsvParser {
public:
    class Row {
        friend class CsvParser;
    public:
        inline Row(): fieldCount(0) {}
        inline size_t cols() const { return fields.size(); }
        inline const char* operator[](size_t idx) const {
            if(idx >= fieldCount) return "";
            return fields[idx];
        }
        
        template<typename T>
        T As(size_t idx) const {
            return Convert<T>((*this)[idx]);
        }
        
        template<typename T>
        std::vector<T> AsVector(size_t idx, const std::string& delimiter = "|") const {
            std::vector<T> _vector;
            StringTokenizer st((*this)[idx], delimiter);
            for(auto& tok : st)
                _vector.push_back(Convert<T>(tok));
            return std::move(_vector);
        }

        std::vector<std::pair<uint32_t, uint32_t>> AsItemVec(size_t idx, const std::string& delimiter = "|", const std::string& idelimiter = ",x") const {
            std::vector<std::pair<uint32_t, uint32_t>> _vector;
            StringTokenizer st((*this)[idx], delimiter);
            for(auto& tok : st) {
                StringTokenizer st2(tok, idelimiter);
                if (st2.size() != 2) continue;
                _vector.push_back(std::make_pair(Convert<uint32_t>(st2[0]),Convert<uint32_t>(st2[1])));
            }
            return std::move(_vector);
        }

        template<typename T>
        std::vector<std::vector<T>> AsVector2(size_t idx, const std::string& delimiter = "|", const std::string& idelimiter = ":") const {
            std::vector<std::vector<T>> _vector;
            StringTokenizer st((*this)[idx], delimiter);
            for(auto& tok : st) {
                StringTokenizer st2(tok, idelimiter);
                std::vector<T> _iVector;
                for(auto& itok : st2)
                {
                    _iVector.push_back(Convert<T>(itok));
                }
                if(!_iVector.empty())
                    _vector.push_back(_iVector);
            }
            return std::move(_vector);
        }

        template<typename T>
        std::vector<std::vector<std::vector<T>>> AsVector3(size_t idx, const std::string& delimiter = "|", const std::string& iDelimiter = ":", const std::string &iiDelimiter = ",") const {
            std::vector<std::vector<std::vector<T>>> _vector;
            StringTokenizer st((*this)[idx], delimiter);
            for(auto& tok : st) {
                StringTokenizer st2(tok, iDelimiter);
                std::vector<std::vector<T>> _iVector;
                for(auto& tok2 : st2)
                {
                    std::vector<T> _iiVector;
                    StringTokenizer st3(tok2, iiDelimiter);
                    for(auto &tok3: st3)
                    {
                        _iiVector.push_back(Convert<T>(tok3));
                    }
                    if(!_iiVector.empty())
                        _iVector.push_back(_iiVector);
                }
                if(!_iVector.empty())
                    _vector.push_back(_iVector);
            }
            return std::move(_vector);
        }

 

        std::pair<int32_t, int32_t> AsPair(size_t idx, const std::string& delimiter = "|") const {
            std::pair<int32_t, int32_t> _pair;
            StringTokenizer st((*this)[idx], delimiter);
            if(st.size() != 2)
                return std::move(_pair);
            _pair.first = Convert<int32_t>(st[0]);
            _pair.second = Convert<int32_t>(st[1]);

            return std::move(_pair);
        }

    private:
        std::vector<const char*> fields;
        size_t fieldCount;
    };
public:
    CsvParser(char seperator, char quote);
    ~CsvParser();
    bool Load(const char* filename);
    
    inline void ResetCursor() { cur = 0; }
    inline void Clear() { rows.clear(); cur = 0; }
    inline const Row& GetRow(size_t idx) {
        static const Row empty__;
        if(idx >= rows.size())
            return empty__;
        return *rows[idx];
    }
    inline void Skip(size_t l = 1) {
        cur += l;
    }
    inline const Row* Next() {
        if(cur >= rows.size())
            return nullptr;
        return rows[cur ++];
    }
private:
    char* data;
    std::vector<Row*> rows;
    char sep, quote;
    size_t cur;
};

}

#endif
