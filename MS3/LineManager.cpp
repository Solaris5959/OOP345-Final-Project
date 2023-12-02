#include <iostream>
#include <fstream>
#include <algorithm>
#include "LineManager.h"
#include "Workstation.h"

namespace sdds {
    LineManager::LineManager(const std::string &file, const std::vector<Workstation *> &stations) {
        std::fstream inFile(file);

        if(inFile.is_open()) {
            std::string buffer;

            while(std::getline(inFile, buffer)) {
                if(buffer.find('|') != std::string::npos) {
                    size_t delim = buffer.find('|');
                    std::string stationName, nextName;
                    stationName = buffer.substr(0, delim);
                    nextName = buffer.substr(delim + 1);
                
                    auto it = std::find_if(stations.begin(), stations.end(), [&](const Workstation* station){
                        return station->getItemName() == stationName;
                    });
                
                    if (it != stations.end())
                        m_activeLine.push_back(*it);
                    else
                        std::cerr << "STATION NOT FOUND" << std::endl;
                
                    auto nextIt = std::find_if(stations.begin(), stations.end(), [&](const Workstation* station){
                        return station->getItemName() == nextName;
                    });
                
                    if (it != stations.end())
                        m_activeLine.back()->setNextStation(*nextIt);
                    else
                        std::cerr << "STATION NOT FOUND" << std::endl;
                }
                else {
                    auto it = std::find_if(stations.begin(), stations.end(), [&](const Workstation* station){
                        return station->getItemName() == buffer;
                    });
                
                    if (it != stations.end()) 
                        m_activeLine.push_back(*it);
                    else
                        std::cerr << "STATION NOT FOUND" << std::endl;
                }
            
                auto first = std::find_if(stations.begin(), stations.end(), [&](Workstation* station) {
                    return std::none_of(stations.begin(), stations.end(), [&](Workstation* other) {
                        return other != station && other->getNextStation() == station;
                    });
                });
            
                if (first != stations.end())
                    m_firstStation = *first;
                else
                    std::cerr << "STATION NOT FOUND" << std::endl;
            
                m_cntCustomerOrder = g_pending.size();
           
            }
        }
        else
            throw std::string("File \"" + file + "\" cannot be opened");
    }

    void LineManager::reorderStations()
    {
        std::sort(m_activeLine.begin(), m_activeLine.end(), [&](Workstation *firstWS, Workstation *secondWS) {
            unsigned int dist1 = 0, dist2 = 0;
            Workstation* currentStation = m_firstStation;

            while (currentStation != firstWS) {
                currentStation = currentStation->getNextStation();
                ++dist1;
            }
            
            currentStation = m_firstStation;

            while (currentStation != secondWS) {
                currentStation = currentStation->getNextStation();
                ++dist2;
            }

            return dist1 < dist2; 
        });
    }

    bool LineManager::run(std::ostream &os) {
        static size_t count = 0;
        ++count;

        os << "Line Manager Iteration: " << count << std::endl;
        
        if (g_pending.size() > 0) {
            *m_firstStation += std::move(g_pending.front());
            g_pending.pop_front();
        }

        for_each(m_activeLine.begin(), m_activeLine.end(), [&](Workstation *station) {
            station->fill(os);
        });

        for_each(m_activeLine.begin(), m_activeLine.end(), [&](Workstation *station) { 
            station->attemptToMoveOrder(); 
        });

        return (g_completed.size() + g_incomplete.size()) == m_cntCustomerOrder;
    }

    void LineManager::display(std::ostream &os) const {
        for_each(m_activeLine.begin(), m_activeLine.end(), [&](const Workstation *station) { 
            station->display(os); 
        });
    }
}
