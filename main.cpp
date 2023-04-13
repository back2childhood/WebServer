/* ************************************************************************
> File Name:     main.cpp
> Author:        Qian JiLi
> mail:          193937157@qq.com
> Created Time:  Tue Apr 11 09:40:38 2023
> Description:   
 ************************************************************************/

#include "webserver.h"

int main(){
    WebServer server(1316, 3, 60000, false, 4);
    server.start();
}
