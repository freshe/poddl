#include <stdio.h>
#include <errno.h>
#include <algorithm>
#include <sstream>
#include <string>
#include <list>
#include <vector>
#include <iostream>
#include <fstream>
#include <regex>
#include <map>
#include <cstdlib>

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
#include <string.h>
#include <sys/stat.h>
#include <curl/curl.h>
#endif

struct Podcast {
	int number;
    std::string url;
    std::string title;
    std::string ext;
};

struct Options {
	bool list_only = false;
	int episode_from = -1;
	int episode_to = -1;
	std::string url {};
	std::string path {};
};

class Parser {
public:
    std::vector<Podcast> get_items(std::string xml);
};

class Helper {
public:
	static Options get_options(std::vector<std::string> args);
	
	template <typename T>
	static std::vector<T> slice_vector(std::vector<T> &items, int from, int to);

    static std::string html_decode(std::string input);
    static std::string clean_filename(std::string input);
    static std::string url_encode_lazy(std::string input);
    static std::string get_extension(std::string input);
#ifdef _WIN32
    static std::wstring utf8_to_wide_win_string(std::string input);
    static std::string wide_win_string_to_utf8(std::wstring input);
	static std::vector<std::string> get_args(int argc, const wchar_t *argv[]);
#else
	static std::vector<std::string> get_args(int argc, const char *argv[]);
#endif
};

template <typename T>
std::vector<T> Helper::slice_vector(std::vector<T> &items, int from, int to) {
	/* check range */
	if ( !(from >= 1 && to >= from) ) {
		return std::vector<T> {};
	}

	if (to > items.size()) {
		to = items.size();
	}

	auto a = items.begin() + from - 1;
	auto b = items.begin() + to;
	auto c = std::vector<T>(a, b);
	
	return c;
}

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