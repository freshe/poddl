/*
 MIT License
 
 Copyright (c) 2019 Fredrik Blank
 
 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:
 
 The above copyright notice and this permission notice shall be included in all
 copies or substantial portions of the Software.
 
 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 SOFTWARE.
 */

#include <iostream>
#include "client.hpp"

#ifdef _WIN32
#include "../LibsynDl.vs/wincurl/curl.h"
#pragma comment(lib, "Normaliz.lib")
#pragma comment(lib, "Advapi32.lib")
#pragma comment(lib, "Ws2_32.lib")
#pragma comment(lib, "Wldap32.Lib")
#pragma comment(lib, "Crypt32.Lib")
#pragma comment(lib, "../LibsynDl.vs/wincurl/libcurl_a.lib")
#else
#include <curl/curl.h>
#endif

size_t Client::curl_write(void* buf, size_t size, size_t nmemb, void* up) {
    if (up) {
        std::ostream& os = *static_cast<std::ostream*>(up);
        std::streamsize len = size * nmemb;
        
        if (os.write(static_cast<char*>(buf), len)) {
            return len;
        }
    }
    
    return 0;
};

CURLcode Client::curl_read(const std::string& url, std::ostream& os, long timeout) {
    CURLcode code(CURLE_FAILED_INIT);
    CURL* curl = curl_easy_init();
    
    if (curl) {
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, &Client::curl_write);
        curl_easy_setopt(curl, CURLOPT_NOPROGRESS, 1L);
        curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
        curl_easy_setopt(curl, CURLOPT_FILE, &os);
        curl_easy_setopt(curl, CURLOPT_TIMEOUT, timeout);
        //curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L);
        code = curl_easy_perform(curl);
        curl_easy_cleanup(curl);
    }
    
    return code;
};

Client::Client() {
    curl_global_init(CURL_GLOBAL_ALL);
}

Client::~Client() {
    curl_global_cleanup();
}

bool Client::get_string_stream(std::string url, std::ostringstream &stream) {
    if (CURLE_OK == Client::curl_read(url, stream, 60)) {
        return true;
    }
    
    return false;
};

bool Client::write_file_stream(std::string url, std::ofstream &stream) {
    if (CURLE_OK == Client::curl_read(url, stream, 30)) {
        return true;
    }
    
    return false;
};
