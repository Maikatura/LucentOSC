#pragma once
#include <iostream>

typedef size_t(*curl_write)(char*, size_t, size_t, std::string*);

class Downloader
{
public:

    static std::string Request(std::string word);
   

private:
    static size_t WriteCallback(char* buf, size_t size, size_t nmemb, std::string* data);
};


