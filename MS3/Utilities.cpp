#include <string>
#include "Utilities.h"

#include <iostream>

namespace sdds {
    void Utilities::setFieldWidth(size_t newWidth) {
        m_widthField = newWidth;
    }
    
    size_t Utilities::getFieldWidth() const {
        return m_widthField;
    }
    
    std::string Utilities::extractToken(const std::string& str, size_t& next_pos, bool& more) {
        if (str.at(next_pos) == m_delimiter) {
            more = false;
            throw "Incorrect Format";
        }

        size_t index = str.find_first_of(m_delimiter, next_pos); // find next occurrence of delim (or npos if no more delims)
        std::string substring = str.substr(next_pos, index - next_pos);    // store the substring of starting position to delim
        more = index != str.npos;    // check if there is more text after the index;
        next_pos = index + 1;   // increment next_pos to not be index of delim

        // this block removes leading whitespace
        substring.erase(0, substring.find_first_not_of(' '));

        if (m_widthField < substring.length())
            m_widthField = substring.length();
        
        return substring;
    }
    
    void Utilities::setDelimiter(char newDelimiter) {
        m_delimiter = newDelimiter;
    }
    
    char Utilities::getDelimiter() {
        return m_delimiter;
    }

    std::string trim(const std::string& str) {
        const auto strBegin = str.find_first_not_of(' ');
        if (strBegin == std::string::npos)
            return ""; // no content
        const auto strEnd = str.find_last_not_of(' ');
        const auto strRange = strEnd - strBegin + 1;

        return str.substr(strBegin, strRange);
    }
}