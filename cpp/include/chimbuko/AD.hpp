#pragma once

#include "chimbuko/ad/ADParser.hpp"
#include "chimbuko/ad/ADEvent.hpp"
#include "chimbuko/ad/ADOutlier.hpp"
#include "chimbuko/ad/ADio.hpp"
#include <queue>

typedef std::priority_queue<chimbuko::Event_t, std::vector<chimbuko::Event_t>, std::greater<std::vector<chimbuko::Event_t>::value_type>> PQUEUE;

// need to move some other header
std::string generate_event_id(int rank, int step, size_t idx);
std::string generate_event_id(int rank, int step, size_t idx, unsigned long eid);


namespace chimbuko {

class OnlineAD {
public:
    OnlineAD(int rank);
    ~OnlineAD();

    void init_io();
    void init_parser();
    void init_event();
    void init_outlier();

    void finalize();

private:
    int              m_rank;
    ADParser       * m_parser;
    ADEvent        * m_event;
    ADOutlierSSTD  * m_outlier;
    ADio           * m_io; 
};

} // end of namespace chimbuko