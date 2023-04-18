/* ************************************************************************
> File Name:     httpresponse.h
> Author:        Qian JiLi
> mail:          193937157@qq.com
> Created Time:  Fri Apr 14 10:52:53 2023
> Description:   
 ************************************************************************/

#ifndef _HTTPRESPONSE_H_
#define _HTTPRESPONSE_H_

#include "_cmpublic.h"
#include "buffer.h"
#include <sys/_types/_size_t.h>
#include <unordered_map>
#include <sys/mman.h>

class HttpResponse{
public:
    HttpResponse();
    ~HttpResponse();

    void init(const std::string& srcDir, std::string& path, bool isKeepAlive = false, int code = -1);
    void makeResponse(Buffer& buffer);

    void unmapFile();
    char* file();
    size_t fileSize() const;

    void errorContent(Buffer& buffer, std::string message);
    int getcode() const{
        return code;
    }
private:
    void addStateLine(Buffer& buffer);
    void addHeader(Buffer& buffer);
    void addContent(Buffer& buffer);

    void errorHTMl();
    std::string getFileType();

    int code;
    bool isKeepAlive;

    std::string path;
    std::string srcDir;

    char* mmFile;
    struct stat mmFileStat;

    static const std::unordered_map<std::string, std::string> SUFFIX_TYPE;
    static const std::unordered_map<int, std::string> CODE_STATUS;
    static const std::unordered_map<int, std::string> CODE_PATH;
};

#endif  // _HTTPRESPONSE_H_
