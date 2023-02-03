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

/*
 *  Windows uses UTF-16 for filenames but can't output it correctly to console?!
 *  I need therapy now
 */

#define VERSION "2023.02.03"

void print_help() {
    std::cout << "How to use:" << std::endl;
    
#ifdef _WIN32
    std::cout << "poddl.exe http://url.to.rss C:\\OutputPath" << std::endl;
#else
    std::cout << "./poddl http://url.to.rss /OutputPath" << std::endl;
#endif

	std::cout << std::endl;
	std::cout << "Optional arguments:" << std::endl;
	std::cout << "-l = Only display list of episodes" << std::endl;
	std::cout << "-n [n] = Download a single episode" << std::endl;
	std::cout << "-n [n-n] = Download a range of episodes" << std::endl;
    std::cout << std::endl;
}

void print_header() {
    std::cout << std::endl << "poddl " << VERSION << std::endl;
    std::cout << "https://www.fredrikblank.com/poddl/" << std::endl;
    std::cout << std::endl;
}

#ifdef _WIN32
int wmain(int argc, const wchar_t *argv[]) {
	SetConsoleOutputCP(CP_UTF8);
#else
int main(int argc, const char *argv[]) {
#endif
    print_header();

	/*
	 * 	-l		List
	 *  -n 1	Download episode 1	
	 *	-n 1-3	Download episode 1-3
	 */

	const auto args = Helper::get_args(argc, argv);

	if (args.size() == 0) {
		print_help();
        return -1;
	}

	const auto options = Helper::get_options(args);

	if (options.url.empty() || ( options.path.empty() && !options.list_only )) {
        std::cout << "Error: Invalid input";
        std::cout << std::endl;
        std::cout << std::endl;
        print_help();
        return -1;
	}

	std::string const url = options.url;

#ifdef _WIN32
	std::wstring const path = Helper::utf8_to_wide_win_string(options.path);
    std::wstring const temp_path = path + L"/tmp";
	std::string const print_path = Helper::wide_win_string_to_utf8(path);
	std::string const print_temp_path = Helper::wide_win_string_to_utf8(temp_path);
#else
	std::string const path = options.path;
    std::string const temp_path = path + "/tmp";
	std::string const print_path = path;
	std::string const print_temp_path = temp_path;
#endif
    std::ostringstream rss_stream;

	if (!options.list_only) {
		if (!FileSystem::create_directory_if_not_exists(path)) {
			std::cout << "Error: Could not create directory " << print_path << std::endl;
			return -1;
		}

		if (!FileSystem::create_directory_if_not_exists(temp_path)) {
			std::cout << "Error: Could not create temp directory " << print_temp_path << std::endl;
			return -1;
		}
	}

 	Client client;
    Parser parser;

    std::cout << "Fetching URL: " << url << std::endl;
    const auto rss_success = client.get_string_stream(url, rss_stream);
    
    if (!rss_success) {
        std::cout << "Error: Invalid response from URL" << std::endl;
        return -1;
    }

    const auto xml = rss_stream.str();
    auto items = parser.get_items(xml);

	if (options.episode_from >= 0) {
		auto temp = Helper::slice_vector(items, options.episode_from, options.episode_to);
		items = temp;
	}

    auto size = items.size();
    auto success = size > 0;

    if (!success) {
        std::cout << "Error: No files found" << std::endl;
        return -1;
    }
        
    std::cout << (options.list_only ? "Listing " : "Downloading ") << size << " files" << std::endl << std::endl;
    int count = 1;

    for (auto const& item : items) {
		if (options.list_only) {
			std::cout << "[" << item.number << "]" << " " << item.title << std::endl;
			count++;
			continue;
		}

#ifdef _WIN32
        std::wstring const file_path = path + L"/" + Helper::utf8_to_wide_win_string(item.title) + L"." + Helper::utf8_to_wide_win_string(item.ext);
        std::wstring const temp_file_path = temp_path + L"/" + Helper::utf8_to_wide_win_string(item.title) + L"." + Helper::utf8_to_wide_win_string(item.ext);
		std::string const print_file_path = Helper::wide_win_string_to_utf8(file_path);
		std::string const print_temp_file_path = Helper::wide_win_string_to_utf8(temp_file_path);
#else
        std::string const file_path = path + "/" + item.title + "." + item.ext;
        std::string const temp_file_path = temp_path + "/" + item.title + "." + item.ext;
		std::string const print_file_path = file_path;
		std::string const print_temp_file_path = temp_file_path;
#endif
        if (FileSystem::file_exists(file_path)) {
            std::cout << "Skipping file " << print_file_path << std::endl;
			count++;
            continue;
        }

        std::ofstream fs(temp_file_path, std::ostream::binary);
        std::cout << "Downloading file " << count << "/" << size << " " << "[" << item.number << "]" << " " << item.title << std::endl;

        if (client.write_file_stream(item.url, fs)) {
            fs.close();

            if (!FileSystem::move_file(temp_file_path, file_path)) {
                std::cout << "Error moving temp file. I'm out. " << print_file_path << std::endl;
                return -1;
            }
        } else {
            std::cout << "Error downloading file " << item.title << std::endl;
        }

        count++;
    }

    if (FileSystem::directory_is_empty(temp_path)) {
        FileSystem::delete_directory(temp_path);
    }

    return 0;
}