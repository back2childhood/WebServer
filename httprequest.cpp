/* ************************************************************************
> File Name:     httprequest.cpp
> Author:        Qian JiLi
> mail:          193937157@qq.com
> Created Time:  Tue Apr 11 16:25:49 2023
> Description:   
 ************************************************************************/

#include "httprequest.h"
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

void HttpRequest::parseRequestLine(const std::string& line){

}
