/*
 MIT License
 
 Copyright (c) 2023 Fredrik B
 
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

#include "html_coder.hpp"

std::set<std::string> fb::HtmlCoder::get_entities(const std::string& input) {
    std::set<std::string> entities;
	
	size_t const length = input.length();
    size_t ampersand_pos = 0;
    bool ampersand_found = false;

    for (size_t i = 0; i < length; i++) {
        const char c = input[i];

        if (c == '&') {
            ampersand_pos = i;
            ampersand_found = true;
            continue;
        }
        
        if (c == ';' && ampersand_found) {
            const size_t entity_length = i - ampersand_pos + 1;
			ampersand_found = false;
            
            if (entity_length >= 3 && entity_length <= 50) {
                /*  Mom said this should work ok */
                const std::string entity = input.substr(ampersand_pos, entity_length);
                
                if (entities.find(entity) == entities.end()) {
                    entities.insert(entity);
                }
            }
        }
    }

    return entities;
}

std::string fb::HtmlCoder::decode_entity(const size_t& number) {
    try {
        const std::string result = convert.to_bytes(static_cast<char32_t>(number));
        return result;
    } catch(const std::exception& e) { 
        /* wtf */
    }

    return std::string{};
}

std::size_t fb::HtmlCoder::get_entity_number(const std::string& entity, const bool isHex) {
    const size_t start_pos = isHex ? 3 : 2;
    const std::string temp = entity.substr(start_pos, entity.length() - start_pos - 1);
    const size_t number = strtol(temp.c_str(), nullptr, isHex ? 16 : 10);

    return number;
}

void fb::HtmlCoder::replace(std::string& subject, const std::string& search, const std::string& replace) {
    size_t pos = 0;
    
	while ((pos = subject.find(search, pos)) != std::string::npos) {
         subject.replace(pos, search.length(), replace);
         pos += replace.length();
    }
}

std::string fb::HtmlCoder::decode(std::string input) {
    if (input.empty()) {
        return input;
    }

    std::set<std::string> entities = HtmlCoder::get_entities(input);
    
    if (entities.size() == 0) {
        /* No entities found, just return the original string */
        return input;
    }

    for (auto const &item : entities) {
#if DEBUG
        std::cout << "item: " << item << std::endl;
#endif
        const size_t length = item.length();
        const char identifier = item[1];
        const char number_type = item[2];
        std::string decoded;

        if (identifier == '#') {
            /* Unicode number */
            const bool isHex = number_type == 'x' || number_type == 'X';
            const size_t number = HtmlCoder::get_entity_number(item, isHex);

            if (number > 0) {
                decoded = HtmlCoder::decode_entity(number);
            }
        } else {
            /* Named entity. &amp; and things like that */
            auto pos = HtmlCoder::named_html_entities.find(item);
            if (pos != HtmlCoder::named_html_entities.end()) {
                decoded = pos->second;
            }
        }

        if (!decoded.empty()) {
            HtmlCoder::replace(input, item, decoded);
        }
    }

    return input;
}