#pragma once
#include "chimbuko/ad/ADDefine.hpp"
#include <string>
#include <vector>
#include <list>
#include <stack>
#include <unordered_map>

namespace chimbuko {

// forward declaration
class Event_t;
class ExecData_t;
class CommData_t;

typedef std::stack<CommData_t> CommStack_t;
typedef std::unordered_map<unsigned long, CommStack_t>      CommStackMap_t_t;
typedef std::unordered_map<unsigned long, CommStackMap_t_t> CommStackMap_r_t;
typedef std::unordered_map<unsigned long, CommStackMap_r_t> CommStackMap_p_t;

typedef std::list<ExecData_t> CallList_t;
typedef CallList_t::iterator  CallListIterator_t;
typedef std::unordered_map<unsigned long, CallList_t>      CallListMap_t_t;
typedef std::unordered_map<unsigned long, CallListMap_t_t> CallListMap_r_t;
typedef std::unordered_map<unsigned long, CallListMap_r_t> CallListMap_p_t;

typedef std::stack<CallListIterator_t> CallStack_t;
typedef std::unordered_map<unsigned long, CallStack_t>      CallStackMap_t_t;
typedef std::unordered_map<unsigned long, CallStackMap_t_t> CallStackMap_r_t;
typedef std::unordered_map<unsigned long, CallStackMap_r_t> CallStackMap_p_t;

typedef std::unordered_map<unsigned long, std::vector<CallListIterator_t>> ExecDataMap_t;

class ADEvent {
public:
    ADEvent();
    ~ADEvent();

    void linkEventType(const std::unordered_map<int, std::string>* m) { m_eventType = m; }
    void linkFuncMap(const std::unordered_map<int, std::string>* m) { m_funcMap = m; }
    const std::unordered_map<int, std::string>* getFuncMap() const { return m_funcMap; }
    const std::unordered_map<int, std::string>* getEventType() const { return m_eventType; }
    const ExecDataMap_t* getExecDataMap() const { return &m_execDataMap; }
    const CallListMap_p_t * getCallListMap() const { return &m_callList; }
    CallListMap_p_t& getCallListMap() { return m_callList; }

    void clear();

    EventError addEvent(const Event_t& event);
    EventError addFunc(const Event_t& event);
    EventError addComm(const Event_t& event);

    CallListMap_p_t* trimCallList();
    void show_status(bool verbose=false) const;

private:
    void createCommStack(unsigned long pid, unsigned long rid, unsigned long tid);
    void createCallStack(unsigned long pid, unsigned long rid, unsigned long tid);
    void createCallList(unsigned long pid, unsigned long rid, unsigned long tid);

private:
    const std::unordered_map<int, std::string> *m_funcMap;
    const std::unordered_map<int, std::string> *m_eventType;

    // stack of communication events
    CommStackMap_p_t  m_commStack;
    // call stack of function events
    CallStackMap_p_t  m_callStack;
    // list of function events (container)
    CallListMap_p_t   m_callList;
    // map of execution data (this will be refreshed every frame in trimCallList())
    ExecDataMap_t     m_execDataMap;
};

} // end of AD namespace