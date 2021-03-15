#include "CsvParser.h"

#include <memory.h>
namespace base {

CsvParser::CsvParser(char seperator, char quote): data(NULL), sep(seperator), quote(quote), cur(0) { }

CsvParser::~CsvParser() {
    delete[] data;
    for(auto*& r : rows)
        delete r;
}

bool CsvParser::Load(const char* filename) {
    if(data != NULL) { delete[] data; data = NULL; }
    for(auto*& r : rows)
        delete r;
    rows.clear();
    cur = 0;
    FILE* file = fopen(filename, "rb");
    if(file == NULL)
        return false;
    fseek(file, 0, SEEK_END);
    size_t sz = ftell(file);
    if(sz == 0) {
        fclose(file);
        return true;
    }
    fseek(file, 0, SEEK_SET);
    data = new(std::nothrow) char[sz + 1];
    if(data == NULL) {
        fclose(file);
        return false;
    }
    size_t ret = fread(data, 1, sz, file);
    if(ret != sz)
    {
        fclose(file);
        delete []data;
        return false;
    }
    data[sz] = 0;
    fclose(file);

    char* str = data;
    char key1[4] = "\r\n";
    char key2[4] = "\r\n";
    key1[2] = sep; key1[3] = 0;
    key2[2] = quote; key2[3] = 0;

#define CHECK_QUOTE \
    while(*str == quote) { \
        ++ str; \
        if(*str != quote) \
            inQuote = true; \
        else \
            memmove(str, str + 1, strlen(str)); \
    }

    bool inQuote = false;
    CHECK_QUOTE;
    Row* rowp = new(std::nothrow) Row;
    int r = 0;
    while(1) {
        r = r + strcspn(str + r, inQuote ? key2 : key1);
        if(r < 0 || str[r] == 0) {
            if(rowp->fields.empty() && str[0] == 0) {
                delete rowp;
                break;
            }
            rowp->fields.push_back(str);
            rowp->fieldCount = rowp->fields.size();
            rows.push_back(rowp);
            break;
        }
        char c = str[r];
        if(c == '\r' || c == '\n') {
            str[r] = 0;
            rowp->fields.push_back(str);
            rowp->fieldCount = rowp->fields.size();
            rows.push_back(rowp);
            str += r + 1;
            while(*str == '\r' || *str == '\n') ++ str;
            rowp = new(std::nothrow) Row;
            CHECK_QUOTE;
            r = 0;
        }
        else if(c == sep) {
            str[r] = 0;
            rowp->fields.push_back(str);
            str += r + 1;
            CHECK_QUOTE;
            r = 0;
        }
        else if(c == quote) {
            if(str[r + 1] == quote) {
                memmove(str + r + 1, str + r + 2, strlen(str + r + 1));
                ++ r;
                continue;
            }
            inQuote = false;
            str[r] = 0;
            rowp->fields.push_back(str);
            str += r + 1;
            if(*str == '\r' || *str == '\n') {
                rowp->fieldCount = rowp->fields.size();
                rows.push_back(rowp);
                while(*str == '\r' || *str == '\n') ++ str;
                rowp = new(std::nothrow) Row;
                CHECK_QUOTE;
            }
            else if(*str == sep) {
                ++ str;
                CHECK_QUOTE;
            }
            else
                return false;
            r = 0;
        }
    }
    return true;
}

}
