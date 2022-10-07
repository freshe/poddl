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

#include "poddl.hpp"

#define VERSION "2022.10.07"

void print_help() {
    std::cout << "How to use:" << std::endl;
    
#ifdef _WIN32
    std::cout << "poddl.exe http://url.to.rss /OutputPath" << std::endl;
#else
    std::cout << "./poddl http://url.to.rss /OutputPath" << std::endl;
#endif
    std::cout << std::endl;
}

void print_header() {
    std::cout << std::endl << "poddl " << VERSION << std::endl;
    std::cout << "https://www.fredrikblank.com/poddl/" << std::endl;
    std::cout << std::endl;
}

int main(int argc, const char *argv[]) {
    print_header();

    if (argc == 1) {
        print_help();
        return -1;
    }

    if (argc != 3) {
        std::cout << "Error: Invalid input";
        std::cout << std::endl;
        std::cout << std::endl;
        print_help();
        return -1;
    }
    
    std::string const path = argv[2];
    std::string const temp_path = path + "/tmp";
    std::string const url = Helper::url_encode_lazy(argv[1]);
    std::ostringstream rss_stream;

    if (!FileSystem::create_directory_if_not_exists(path)) {
        std::cout << "Error: Could not create directory " << path << std::endl;
        return -1;
    }

    if (!FileSystem::create_directory_if_not_exists(temp_path)) {
        std::cout << "Error: Could not create temp directory " << temp_path << std::endl;
        return -1;
    }

    auto client = Client();
    auto parser = Parser();

    std::cout << "Fetching URL: " << url << std::endl;
    auto rss_success = client.get_string_stream(url, rss_stream);
    
    if (!rss_success) {
        std::cout << "Error: Invalid response from URL" << std::endl;
        return -1;
    }

    auto xml = rss_stream.str();
    auto items = parser.get_items(xml);
    auto size = items.size();
    auto success = size > 0;

    if (!success) {
        std::cout << "Error: No files found" << std::endl;
        return -1;
    }
        
    std::cout << "Downloading " << size << " files" << std::endl << std::endl;
    int count = 1;
            
    for (auto const& item : items) {
        std::string const file_path = path + "/" + item.title + "." + item.ext;
        std::string const temp_file_path = temp_path + "/" + item.title + "." + item.ext;

        if (FileSystem::file_exists(file_path)) {
            std::cout << "Skipping file " << file_path << std::endl;
			count++;
            continue;
        }

        std::ofstream fs(temp_file_path, std::ostream::binary);
        std::cout << "Downloading file " << count << "/" << size << " " << item.title << std::endl;

        if (client.write_file_stream(item.url, fs)) {
            fs.close();

            if (!FileSystem::move_file(temp_file_path, file_path)) {
                std::cout << "Error moving temp file. I'm out. " << file_path << std::endl;
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