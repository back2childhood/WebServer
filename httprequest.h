/* ************************************************************************
> File Name:     httprequest.h
> Author:        Qian JiLi
> mail:          193937157@qq.com
> Created Time:  Tue Apr 11 15:46:25 2023
> Description:   
 ************************************************************************/

#ifndef _HTTPREQUEST_H_
#define _HTTPREQUEST_H_

#include "_cmpublic.h"
#include "buffer.h"
#include <string>
#include <unordered_map>
#include <unordered_set>

class HttpRequest{
public:
    enum PARSE_STATE{
        REQUEST_LINE,
        HEADERS,
        BODY,
        FINISH,
    };

    HttpRequest(){
        init();
    }
    ~HttpRequest() = default;

    void init();
    bool parse(Buffer& buff);

    std::string getpath() const;
    std::string& getpath();
    std::string getmethod() const;
    std::string getversion() const;
    std::string getPost(const std::string& key) const;
    std::string getPost(const char* key) const;

    bool isKeepAlive() const;
private:
    bool parseRequestLine(const std::string& line);
    void parseRequestHeader(const std::string& line);
    void parseDataBody(const std::string& line);

    void parsePath();
    void parsePost();

    static int convertHex(char ch);

    PARSE_STATE state;
    std::string method, path, version, body;
    std::unordered_map<std::string, std::string> header;
    std::unordered_map<std::string, std::string> post;

    static const std::unordered_set<std::string> DEFAULT_HTML;
};
#endif  // _HTTPREQUEST_H_
