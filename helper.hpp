#include <string>
#include <map>

class Helper {
public:
    static std::string html_decode(std::string input);
    static std::string clean_filename(std::string input);
    static std::string url_encode_lazy(std::string input);
    static std::string get_extension(std::string input);
};
