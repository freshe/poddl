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

#include <stdio.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <list>
#include <errno.h>
#include "fs.hpp"
#include "client.hpp"
#include "parser.hpp"

void print_help() {
    std::cout << "How to use:" << std::endl;
    
#ifdef _WIN32
    std::cout << "LibsynDL.exe http://url.to.rss /OutputPath" << std::endl;
#else
    std::cout << "./LibsynDL http://url.to.rss /OutputPath" << std::endl;
#endif
}

void print_header() {
    std::cout << "LibsynDL" << std::endl;
    std::cout << "https://www.fredrikblank.com/libsyndl/" << std::endl;
    std::cout << std::endl;
}

int main(int argc, const char *argv[]) {
	print_header();

    if (argc != 3) {
        std::cout << "Error: Invalid input";
        std::cout << std::endl;
        std::cout << std::endl;

        print_help();
        return 0;
    }
    
    std::string const url = argv[1];
    std::string const path = argv[2];
    std::ostringstream rss_stream;
    
    if (!FileSystem::directory_exists(path)) {
        if (!FileSystem::create_directory(path)) {
            std::cout << "Error: Could not create directory " << path << std::endl;
            return 0;
        }
    }

    auto client = Client();
    auto parser = Parser();
    auto rss_success = client.get_string_stream(url, rss_stream);
    
    if (rss_success) {
        std::string xml = rss_stream.str();
        auto items = parser.get_items(xml);
        auto size = items.size();
        
        if (size > 0) {
            std::cout << "Downloading " << size << " files" << std::endl;
            int count = 1;
            
            for (auto const& item : items) {
                std::ofstream fs(path + "/" + item.title + "." + item.ext, std::ostream::binary);

                if (client.write_file_stream(item.url, fs)) {
                    std::cout << "Downloaded file " << count << "/" << size << " " << item.title << std::endl;
                } else {
                    std::cout << "Error downloading file " << item.title << std::endl;
                }
                
                count++;
            }
        } else {
            std::cout << "Error: No files found" << std::endl;
        }
    } else {
        std::cout << "Error: Could not fetch URL" << std::endl;
    }

    return 0;
}
