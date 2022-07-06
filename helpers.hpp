#include <string>
#include <map>

class Helpers {
public:
    static void replace_substring(std::string& subject, const std::string& search, const std::string& replace); 
    static std::string html_decode(std::string input);
    static std::string url_encode_lazy(std::string input);
    static std::string get_extension(std::string input);
};
