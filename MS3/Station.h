#ifndef SDDS_STATION_H
#define SDDS_STATION_H
#include <string>

namespace sdds {
    class Station {
        int m_id;
        std::string m_name;
        std::string m_description;
        unsigned int m_nextSerial;
        unsigned int m_quantity;
        static inline size_t m_widthField = 0;
        static inline int id_generator = 0;
      public:
        Station(const std::string& line);
        const std::string& getItemName() const;
        size_t getNextSerialNumber();
        size_t getQuantity();
        void updateQuantity();
        void display(std::ostream& os, bool full) const;
    };
}

#endif