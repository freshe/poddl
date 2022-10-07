#include <stdio.h>
#include <errno.h>
#include <algorithm>
#include <sstream>
#include <string>
#include <list>
#include <iostream>
#include <fstream>
#include <regex>
#include <map>

#ifdef _WIN32
#define CURL_STATICLIB
#include <windows.h>
#include <shlwapi.h>
#include <direct.h>
#include "../LibsynDl.vs/wincurl/curl.h"
#pragma comment(lib, "Shlwapi.lib")
#pragma comment(lib, "Normaliz.lib")
#pragma comment(lib, "Advapi32.lib")
#pragma comment(lib, "Ws2_32.lib")
#pragma comment(lib, "Wldap32.Lib")
#pragma comment(lib, "Crypt32.Lib")
#pragma comment(lib, "../LibsynDl.vs/wincurl/libcurl_a.lib")
#else
#include <unistd.h>
#include <dirent.h>
#include <sys/stat.h>
#include <curl/curl.h>
#endif

struct Podcast {
    std::string url;
    std::string title;
    std::string ext;
};

class Parser {
public:
    std::list<Podcast> get_items(std::string xml);
};

class Helper {
public:
    static std::string html_decode(std::string input);
    static std::string clean_filename(std::string input);
    static std::string url_encode_lazy(std::string input);
    static std::string get_extension(std::string input);
};

class FileSystem {
public:
    static bool directory_exists(std::string path);
    static bool create_directory(std::string path);
    static bool delete_directory(std::string path);
    static bool file_exists(std::string path);
    static bool move_file(std::string from, std::string to);
    static bool directory_is_empty(std::string path);
    static bool create_directory_if_not_exists(std::string path);
};

class Client {
private:
    static size_t curl_write(void* buf, size_t size, size_t nmemb, void* up);
    static CURLcode curl_read(const std::string& url, std::ostream& os);
public:
    Client();
    ~Client();
    static bool get_string_stream(std::string url, std::ostringstream &stream);
    static bool write_file_stream(std::string url, std::ofstream &stream);
};