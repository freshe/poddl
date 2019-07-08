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

#include "parser.hpp"
#include <regex>

std::string const pattern = "\\<enclosure.+url=.+(http.+/(.+\\.mp3|m4a|ogg|aac)).+/\\>";

std::list<Podcast> Parser::get_items(std::string xml) {
    std::regex regex(pattern);
    std::smatch match;
    std::list<Podcast> podcasts;
    
    while (std::regex_search(xml, match, regex)) {
        std::string const file_url = match.str(1);
        std::string const file_name = match.str(2);
        auto item = Podcast();
        item.name = file_name;
        item.url = file_url;
        podcasts.push_back(item);
        xml = match.suffix().str();
    }
    
    return podcasts;
}
