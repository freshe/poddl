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

std::string const enclosure_pattern = "<enclosure.+url=.+(http.+/(.+\\.(mp3|m4a|mp4|ogg|oga|aac|flac))).+/\\>";
std::string const title_pattern = "<title>(.+)</title>";
std::string const start_tag = "<item>";
std::string const end_tag = "</item>";
std::size_t const end_len = end_tag.length();

std::list<Podcast> Parser::get_items(std::string xml) {
    std::list<Podcast> podcasts;
    auto spos = xml.find(start_tag);
    auto epos = xml.find(end_tag);
    
    while (spos >= 0 && epos > spos) {
        auto len = (epos - spos) + end_len;
        auto item = xml.substr(spos, len);

        std::string url, name, title, ext;
        std::regex rgxEnclosure(enclosure_pattern);
        std::regex rgxTitle(title_pattern);
        std::smatch matchEnclosure;
        std::smatch matchTitle;
        
        //URL, Name, Extension
        if (std::regex_search(item, matchEnclosure, rgxEnclosure)) {
            url = matchEnclosure.str(1);
            name = matchEnclosure.str(2);
            ext = matchEnclosure.str(3);
        }
        
        //Title
        if (std::regex_search(item, matchTitle, rgxTitle)) {
            title = matchTitle.str(1);
        }
        
        if (!url.empty() && !name.empty() && !ext.empty() && !title.empty()) {
            auto podcast = Podcast();
            podcast.url = url;
            podcast.name = name;
            podcast.title = title;
            podcast.ext = ext;
            podcasts.push_back(podcast);
        }
        
        xml = xml.replace(spos, len, "");
        spos = xml.find(start_tag);
        epos = xml.find(end_tag);
    }

    return podcasts;
}
