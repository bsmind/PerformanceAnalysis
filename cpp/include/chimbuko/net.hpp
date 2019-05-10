#pragma once
#include "chimbuko/util/threadPool.hpp"
#include "chimbuko/param.hpp"
#include <string>

namespace chimbuko {

enum class NetThreadLevel {
    THREAD_MULTIPLE
};

// Interface of inter- and intra- process communication
class NetInterface {
public:
    NetInterface();
    virtual ~NetInterface();

    virtual void init(int* argc = nullptr, char*** argv = nullptr, int nt=1) = 0;
    virtual void finalize() = 0;

    virtual void run() = 0;

    virtual std::string name() const = 0;

    void init_parameter(ParamKind kind);

protected:
    void init_thread_pool(int nt);

protected:
    int m_nt;
    threadPool* m_tpool;
    ParamKind m_kind;
    ParamInterface * m_param;
};

namespace DefaultNetInterface
{
    NetInterface& get();
}

} // end of chimbuko namespace