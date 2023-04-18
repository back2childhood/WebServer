/* ************************************************************************
> File Name:     httprequest.cpp
> Author:        Qian JiLi
> mail:          193937157@qq.com
> Created Time:  Tue Apr 11 16:25:49 2023
> Description:   
 ************************************************************************/

#include "httprequest.h"
#include <cassert>
#include <regex>
#include <unordered_set>

const std::unordered_set<std::string> HttpRequest::DEFAULT_HTML{
    "/index", "/welcome", "/video", "/picture"
};

void HttpRequest::init(){
    method = path = version = body = "";
    header.clear();
    post.clear();
    state = REQUEST_LINE;
}

bool HttpRequest::isKeepAlive() const{
    if(header.count("Connection") == 1){
        return header.find("Connection") -> second == "keep-alive" && version == "1.1";
    }
    return false;
}

bool HttpRequest::parse(Buffer &buff){
    const char CRLF[] = "\r\n";
    if(buff.readableBytes() <= 0) return false;

    // 自动状态机
    while(buff.readableBytes() && state != FINISH){
        const char* lineEnd = std::search(buff.curReadPtr(), buff.curWritePtrConst(), CRLF, CRLF + 2);
        std::string line(buff.curReadPtr(), lineEnd);
        switch(state){
        case REQUEST_LINE:
            if(!parseRequestLine(line))
                return false;
            parsePath();
            break;
        case HEADERS:
            parseRequestHeader(line);
            if(buff.readableBytes() <= 2)
                state = FINISH;
            break;
        case BODY:
            parseDataBody(line);
                break;
        default:
            break;
        }
        if(lineEnd == buff.curWritePtrConst()) break;
        buff.updateReadPtrUntilEnd(lineEnd + 2);
    }
    return true;
}

void HttpRequest::parsePath(){
    if(path == "/")
        path = "index.html";
    else {
        for(auto &item : DEFAULT_HTML){
            if(item == path){
                path += ".html";
                break;
            }
        }
    }
}

bool HttpRequest::parseRequestLine(const std::string& line){
    std::regex pattern("^([^ ]*) ([^ ]*) HTTP/([^ ]*)$");
    std::smatch subMatch;
    if(std::regex_match(line, subMatch, pattern)){
        method = subMatch[1];
        path = subMatch[2];
        version = subMatch[3];
        state = HEADERS;
        return true;
    }
    return false;
}

void HttpRequest::parseRequestHeader(const std::string& line){
    std::regex pattern("^([^:]*): ?(.*)$");
    std::smatch subMatch;
    if(regex_match(line, subMatch, pattern)){
        header[subMatch[1]] = subMatch[2];
    }else state = BODY;
}

void HttpRequest::parseDataBody(const std::string& line){
    body = line;
    parsePost();
    state = FINISH;
}

int HttpRequest::convertHex(char ch){
    if(ch >= 'a' && ch <= 'f') return ch - 'a' + 10;
    if(ch >= 'A' && ch <= 'F') return ch - 'a' + 10;
    return ch;
}

void HttpRequest::parsePost(){
    if(method == "POST" && header["Content-Type"] == "application/x-www-form-urlencoded"){
        if(body.size() == 0) return;

        std::string key, value;
        int num = 0;
        int n = body.size();
        int i = 0, j = 0;

        for(; i < n; ++ i){
            char ch = body[i];
            switch(ch){
                case '=':
                    key = body.substr(j, i - j);
                    j = i + 1;
                    break;
                case '+':
                    
                default:
                    break;
            }
        }
        assert(j <= i);
        if(post.count(key) == 0 && j < i){
            value = body.substr(j, i - j);
            post[key] = value;
        }
    }
}

std::string HttpRequest::getpath() const{
    return path;
}

std::string& HttpRequest::getpath(){
    return path;
}

std::string HttpRequest::getmethod() const{
    return method;
}

std::string HttpRequest::getversion() const{
    return version;
}

std::string HttpRequest::getPost(const std::string& key) const{
    assert(key != "");
    if(post.count(key) == 1)
        return post.find(key)->second;
    return "";
}

std::string HttpRequest::getPost(const char* key) const{
    assert(key != nullptr);
    if(post.count(key) == 1)
        return post.find(key)->second;
    return "";
}
