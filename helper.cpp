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

Options Helper::get_options(std::vector<std::string> args) {
    fb::HtmlCoder html_coder;
	Options options;

	for (size_t i = 0; i != args.size(); i++) {
		auto const x = args[i];

		if (x == "-n") {
			if ( !(i + 1 <= args.size() - 1) ) {
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
		} else if (x == "-l") {
			options.list_only = true;
		} else if (x == "-s") {
			options.short_names = true;
		} else if (x == "-r") {
			options.newest_first = true;
		} else {
			/* this will hopefully prevent a breaking change */
			if (options.url.empty()) {
				options.url = url_encode_lazy(html_coder.decode(x));
			} else {
				options.path = x;
			}
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
	
	if ( !(number_from >= 1 && number_to >= number_from) ) {
		return std::vector<Podcast> {};
	}

	int index_from = -1;
	int index_to = -1;
	size_t i = 0;

	/* search for subset index based on episode number */
	for (const auto &item : items) {
		if (item.number == number_from) {
			index_from = i;
		}
		if (item.number == number_to) {
			index_to = i;
		}
		i++;
	}

	if (index_from < 0 || index_to < 0) {
		return std::vector<Podcast> {};
	}

	const size_t index_begin = std::min(index_from, index_to);
	const size_t index_end = std::max(index_from, index_to);
	auto a = items.begin() + index_begin;
	auto b = items.begin() + index_end + 1;
	std::vector<Podcast> c(a, b);

	return c;
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