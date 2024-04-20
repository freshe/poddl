/*
 MIT License
 
 Copyright (c) 2019 Fredrik B
 
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

#include "poddl.hpp"
#include "html_coder.hpp"

/*
    / \ * ? | <> : "
*/
const std::map<std::string, std::string> bad_filename_characters
{
    {"<", ""},
    {">", ""},
    {"|", ""},
    {"?", ""},
    {"*", ""},
    {":", ""},
    {"/", ""},
    {"\\", ""},
    {"\"", ""}
};

const std::string media_extensions[] =
{ 
    "mp3","m4a","mp4","ogg","oga","aac","flac","wma","wmv","mpg","mpeg",
    "avi","m4v","mov","ac3","pcm","wav","alac","webm" 
};

#ifdef _WIN32
std::vector<std::string> Helper::get_args(int argc, const wchar_t *argv[]) {
    std::vector<std::string> v;
    
    for (int i = 1; i < argc; i++) {
        const wchar_t *arg = argv[i];
        const std::wstring wstr(arg);
        const std::string str = Helper::wide_win_string_to_utf8(wstr);
        v.push_back(str);
    }

    return v;
}
#else
std::vector<std::string> Helper::get_args(int argc, const char *argv[]) {
    std::vector<std::string> v;
    
    for (int i = 1; i < argc; i++) {
        const char *arg = argv[i];
        const std::string str(arg);
        v.push_back(str);
    }

    return v;
}
#endif

int int_try_parse(std::string text) {
    char* endptr;
    int number = strtol(text.c_str(), &endptr, 10);

    if (endptr == text.c_str() + text.size()) {
        return number;
    }

    return 0;
}

void Helper::debug_print_options(const Options &options) 
{
    std::cout << "::: OPTIONS :::" << std::endl;
    std::cout << "url: " << options.url << std::endl;
    std::cout << "path: " << options.path << std::endl;
    std::cout << "list_only: " << options.list_only << std::endl;
    std::cout << "append_episode_nr: " << options.append_episode_nr << std::endl;
    std::cout << "short_names: " << options.short_names << std::endl;
    std::cout << "zero_padded_episode_nr: " << options.zero_padded_episode_nr << std::endl;
    std::cout << "newest_first: " << options.newest_first << std::endl;
    std::cout << "episode_from: " << options.episode_from << std::endl;
    std::cout << "episode_to: " << options.episode_to << std::endl;
    std::cout << "take: " << options.take << std::endl;
    std::cout << "stop_when_file_found -h: " << options.stop_when_file_found << std::endl;
    std::cout << "stop_when_file_found_string -h: " << options.stop_when_file_found_string << std::endl;
}

Options Helper::get_options(const std::vector<std::string> &args) {
    fb::HtmlCoder html_coder;
    Options options;

    const size_t args_size = args.size();
    const size_t last_i = args_size == 0 ? 0 : args_size - 1;

    if (args_size <= 1) {
        return options;
    }

    if (args_size == 2 && args[1][0] != '-') {
        options.url = url_encode_lazy(html_coder.decode(args[0]));
        options.path = args[1];
        return options;
    }

    options.url = url_encode_lazy(html_coder.decode(args[0]));

    for (size_t i = 1; i != args.size(); i++) {
        auto const arg = args[i];

        if (arg == "-l") {
            options.list_only = true;
        } 
        else if (arg == "-s") {
            options.short_names = true;
        }
        else if (arg == "-r") {
            options.newest_first = true;
        }
        else if (arg == "-i") {
            options.append_episode_nr = true;
        }
        else if (arg == "-z") {
            options.zero_padded_episode_nr = 3;
          
            if (i + 1 > last_i) {
                continue;
            }

            auto const z_argument = args[i + 1];

            if (z_argument.length() > 0 && z_argument[0] != '-') {
                const auto z_number = int_try_parse(z_argument);
                if (z_number > 0) {
                    options.zero_padded_episode_nr = z_number;
                }

                i = i + 1;
            }
        }
        else if (arg == "-o") {
            if (i + 1 > last_i) {
                continue;
            }

            options.path = args[i + 1];
            i = i + 1;
        }
        else if (arg == "-h") {
            options.stop_when_file_found = true;
            if (i + 1 > last_i) {
                continue;
            }

            auto const h_argument = args[i + 1];
            
            if (h_argument.length() > 0 && h_argument[0] != '-') {
                options.stop_when_file_found_string = h_argument;
                i = i + 1;
            }
        }
        else if (arg == "-t") {
            if (i + 1 > last_i) {
                continue;
            }

            int take = int_try_parse(args[i + 1]);
            if (take > 0) {
                options.take = take;
            }
        }
        else if (arg == "-n") {
            if (i + 1 > last_i) {
                continue;
            }

            auto const n = args[i + 1];
            auto const d = n.find("-");

            if (d != std::string::npos) {
                auto const a = n.substr(0, d);
                auto const b = n.substr(d + 1);

                options.episode_from = int_try_parse(a);
                options.episode_to = int_try_parse(b);
            } else {
                options.episode_from = int_try_parse(n);
                options.episode_to = options.episode_from;
            }

            i = i + 1;
        }
    }

    return options;
}

void replace_substring(std::string& subject, const std::string& search, const std::string& replace) {
    size_t pos = 0;
    while ((pos = subject.find(search, pos)) != std::string::npos) {
         subject.replace(pos, search.length(), replace);
         pos += replace.length();
    }
}

std::vector<Podcast> Helper::get_subset(
    std::vector<Podcast> &items, int number_from, int number_to) {
    
    if (items.size() <= 1) {
        return items;
    }

    if (number_from > number_to) {
        return std::vector<Podcast>{};
    }
    
    if (number_from <= 0) {
        number_from = 1;
    }
    
    if (number_from > items.size()) {
        return std::vector<Podcast>{};
    }

    if (number_to > items.size()) {
        number_to = items.size();
    }

    const bool reverse = items[0].number > items[1].number;
    
    if (reverse) {
        std::reverse(items.begin(), items.end());
    }

    auto a = items.begin() + number_from - 1;
    auto b = items.begin() + number_to;
    std::vector<Podcast> c(a, b);

    if (reverse) {
        std::reverse(c.begin(), c.end());
    }

    return c;
}

bool Helper::string_exists(const std::string &input, const std::string &search) {
    /* todo non-ascii support : / */
    std::string a = input;
    std::string b = search;

    std::transform(a.begin(), a.end(), a.begin(), [](unsigned char c) { 
        return std::tolower(c); 
    });
    
    std::transform(b.begin(), b.end(), b.begin(), [](unsigned char c) { 
        return std::tolower(c); 
    });

    const size_t found = a.find(b);
    return found != std::string::npos;
}

std::string Helper::clean_filename(std::string input) {
    for (auto const &x : bad_filename_characters) {
        std::size_t found = input.find(x.first);
        if (found != std::string::npos) {
            replace_substring(input, x.first, x.second);
        }
    }
    
    return input;
}

std::string Helper::url_encode_lazy(std::string input) {
    replace_substring(input, " ", "%20");
    return input;
}

std::string Helper::get_extension(std::string input) {
    for (auto const &x : media_extensions) {
        const std::string needle = "." + x;
        bool found = input.find(needle) != std::string::npos;
        if (found) {
            return x;
        }
    }

    //fallback
    return "mp3";
}

std::string Helper::get_zero_padded_number_string(const size_t number, const size_t leading_zeros) {
    std::string number_str = std::to_string(number);
    std::string zero_str = std::string(leading_zeros - std::min(leading_zeros, number_str.length()), '0');
    std::string padded_str = zero_str + number_str;

    return padded_str;
}

time_t Helper::rfc_time_to_timestamp(const std::string &input) {
    std::tm tm = {};
    std::istringstream ss(input);
    ss >> std::get_time(&tm, "%a, %d %b %Y %H:%M:%S");

    if (ss.fail()) {
        //error
    } else {
        std::time_t t = std::mktime(&tm);
        return t;
    }

    return 0;
}

#ifdef _WIN32
std::wstring Helper::utf8_to_wide_win_string(std::string input) {
    int wchars_num = MultiByteToWideChar(CP_UTF8, 0, input.c_str(), -1, NULL ,0);
    if (wchars_num <= 0) {
        return std::wstring();
    }

    wchar_t* wstr = new wchar_t[wchars_num];
    wchars_num = MultiByteToWideChar(CP_UTF8, 0, input.c_str(), -1, wstr, wchars_num);

    std::wstring output(wstr);
    delete[] wstr;

    return output;
}

std::string Helper::wide_win_string_to_utf8(std::wstring input) {
    int chars_num = WideCharToMultiByte(CP_UTF8, 0, input.c_str(), -1, NULL, 0, NULL, NULL);
    if (chars_num <= 0) {
        return std::string();
    }

    char* str = new char[chars_num];
    chars_num = WideCharToMultiByte(CP_UTF8, 0, input.c_str(), -1, str, chars_num, NULL, NULL);
    
    std::string output(str);
    delete[] str;
    
    return output;
}
#endif