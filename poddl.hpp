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
#ifdef _WIN32
    static std::wstring utf8_to_wide_win_string(std::string input);
    static std::string wide_win_string_to_utf8(std::wstring input);
#endif
};

class FileSystem {
public:
#ifdef _WIN32
    static bool directory_exists(std::wstring path);
    static bool create_directory(std::wstring path);
    static bool delete_directory(std::wstring path);
    static bool file_exists(std::wstring path);
    static bool move_file(std::wstring from, std::wstring to);
    static bool directory_is_empty(std::wstring path);
    static bool create_directory_if_not_exists(std::wstring path);
#else
    static bool directory_exists(std::string path);
    static bool create_directory(std::string path);
    static bool delete_directory(std::string path);
    static bool file_exists(std::string path);
    static bool move_file(std::string from, std::string to);
    static bool directory_is_empty(std::string path);
    static bool create_directory_if_not_exists(std::string path);
#endif
};

class Client {
private:
    static size_t curl_write(void* buffer, size_t size, size_t nmemb, void* output_ptr);
    static CURLcode curl_read(const std::string& url, std::ostream& output_stream);
public:
    Client();
    ~Client();
    static bool get_string_stream(std::string url, std::ostringstream &output_stream);
    static bool write_file_stream(std::string url, std::ofstream &output_stream);
};