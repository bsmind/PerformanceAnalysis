#include "chimbuko/AD.hpp"

using namespace chimbuko;

OnlineAD::OnlineAD(int rank)
: m_rank(rank), m_parser(nullptr), m_event(nullptr), m_outlier(nullptr), m_io(nullptr)
{

}

OnlineAD::~OnlineAD()
{
    finalize();
}

void OnlineAD::finalize()
{
    if (m_outlier)
        m_outlier->disconnect_ps();

    if (m_parser) {
        delete m_parser;
        m_parser = nullptr;
    }
    if (m_event) {
        delete m_event;
        m_event = nullptr;
    }
    if (m_outlier) {
        delete m_outlier;
        m_outlier = nullptr;
    }
    if (m_io) {
        delete m_io;
        m_io = nullptr;
    }
}