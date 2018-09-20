//
// Created by HFD on 9/19/2018.
//

#ifndef CPLUS_RES_PATH_H
#define CPLUS_RES_PATH_H

#include <iostream>
#include <string>
#include <SDL.h>

std::string getResourcePath(const std::string &subDir = ""){
#ifdef _WIN32
    const char PATH_SEP = '\\';
#else
    const char PATH_SEP = '/';
#endif

    static std::string baseRes;
    if (baseRes.empty()){
        char *basePath = SDL_GetBasePath();
        if (basePath){
            baseRes = basePath;
            SDL_free(basePath);
        } else{
            std::cerr << "Error getting resource path: "<<SDL_GetError()<<std::endl;
            return "";
        }

//        size_t pos = baseRes.rfind("bin");
        return subDir.empty() ? baseRes:baseRes + subDir + PATH_SEP;
    }
}
#endif //CPLUS_RES_PATH_H
