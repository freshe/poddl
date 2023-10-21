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

std::string const enclosure_pattern = "<enclosure.*?url=[\"|'](.+?)[\"|'].*?>";
std::string const title_pattern = "<title>(.+?)</title>";
std::string const cdata_pattern = "<\\!\\[CDATA\\[(.+?)\\]\\]>";

std::string const start_tag = "<item";
std::string const end_tag = "</item>";
std::size_t const end_len = end_tag.length();

std::vector<Podcast> Parser::get_items(const std::string &xml, bool reverse) {
    fb::HtmlCoder html_coder;
    std::vector<Podcast> output;

    std::regex regex_enclosure(enclosure_pattern);
    std::regex regex_title(title_pattern);
    std::regex regex_cdata(cdata_pattern);

    auto start_pos = xml.find(start_tag);
    auto end_pos = xml.find(end_tag);

    while (start_pos >= 0 && end_pos > start_pos) {
        auto length = (end_pos - start_pos) + end_len;
        auto item = xml.substr(start_pos, length);

        std::string url, title, ext;
        std::smatch match_enclosure;
        std::smatch match_title;
        std::smatch match_cdata;
        
        //URL
        if (std::regex_search(item, match_enclosure, regex_enclosure)) {
            url = match_enclosure.str(1);
        }
        
        //Title
        if (std::regex_search(item, match_title, regex_title)) {
            title = match_title.str(1);
        }
        
        if (!url.empty() && !title.empty()) {
            Podcast podcast;

            if (std::regex_search(title, match_cdata, regex_cdata)) {
                title = match_cdata.str(1);
            }

            podcast.url = Helper::url_encode_lazy(html_coder.decode(url));
            podcast.title = Helper::clean_filename(html_coder.decode(title));
            podcast.ext = Helper::get_extension(url);
            
            output.push_back(podcast);
        }

        start_pos = xml.find(start_tag, end_pos);
        end_pos = xml.find(end_tag, start_pos);
    }

    if (reverse) {
        /* oldest episode first (default) */
        std::reverse(output.begin(), output.end());
    }

    const size_t size = output.size();

    for (int i = 0; i < size; i++) {
        auto item = &output[i];
        item->number = (reverse ? i + 1 : size - i);
    }

    return output;
}