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
#include "helper.hpp"
#include <regex>
#include <map>
#include <algorithm>

std::string const enclosure_pattern = "<enclosure.*?url=[\"|'](.+?)[\"|'].*?>";
std::string const title_pattern = "<title>(.+?)</title>";
std::string const start_tag = "<item>";
std::string const end_tag = "</item>";
std::size_t const end_len = end_tag.length();

std::list<Podcast> Parser::get_items(std::string xml) {
    std::list<Podcast> podcasts;
    auto start_pos = xml.find(start_tag);
    auto end_pos = xml.find(end_tag);
    
    while (start_pos >= 0 && end_pos > start_pos) {
        auto length = (end_pos - start_pos) + end_len;
        auto item = xml.substr(start_pos, length);

        std::string url, title, ext;
        std::regex rgxEnclosure(enclosure_pattern);
        std::regex rgxTitle(title_pattern);
        std::smatch matchEnclosure;
        std::smatch matchTitle;
        
        //URL
        if (std::regex_search(item, matchEnclosure, rgxEnclosure)) {
            url = matchEnclosure.str(1);
        }
        
        //Title
        if (std::regex_search(item, matchTitle, rgxTitle)) {
            title = matchTitle.str(1);
        }
        
        if (!url.empty() && !title.empty()) {
            auto podcast = Podcast();
            podcast.url = Helper::url_encode_lazy(url);
            podcast.title = Helper::clean_filename(Helper::html_decode(title));
            podcast.ext = Helper::get_extension(url);
            podcasts.push_back(podcast);
        }

        xml = xml.replace(start_pos, length, "");
        start_pos = xml.find(start_tag);
        end_pos = xml.find(end_tag);
    }

    return podcasts;
}