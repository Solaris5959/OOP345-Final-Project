#include <string>
#include <iostream>
#include <iomanip>
#include "Station.h"
#include "Utilities.h"

namespace sdds {
    Station::Station(const std::string& line) {
        Utilities scanner;
        size_t nextPos = 0;
        bool more = true;
        m_id = ++id_generator;

        m_name = scanner.extractToken(line, nextPos, more);
        if (more)
            m_nextSerial = stoi(scanner.extractToken(line, nextPos, more));
        if (more)
            m_quantity = stoi(scanner.extractToken(line, nextPos, more));
        
        if (more) {
            m_widthField = std::max(m_widthField, scanner.getFieldWidth());
            m_description = scanner.extractToken(line, nextPos, more);
        }

        m_name = trim(m_name);
    }
    
    const std::string& Station::getItemName() const {
        return m_name;
    }
    
    size_t Station::getNextSerialNumber() {
        return m_nextSerial++;
    }
    
    size_t Station::getQuantity() {
        return m_quantity;
    }
    
    void Station::updateQuantity() {
        if (m_quantity > 0)
            m_quantity--;
    }
    
    void Station::display(std::ostream& os, bool full) const {
        os << std::right << std::setw(3) << std::setfill('0') << m_id  << " | " << std::left << std::setfill(' ') << std::setw(m_widthField) << m_name
           << "| "<< std::right << std::setw(6) << std::setfill('0') << m_nextSerial << " | ";
        
        if (full) {
            os << std::setfill(' ') << std::setw(4) << m_quantity << " | " << m_description;
        }
        os << std::endl;
    }
}