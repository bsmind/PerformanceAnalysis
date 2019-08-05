#pragma once
#include "chimbuko/ad/ADDefine.hpp"
#include <string>
#include <vector>
#include <unordered_map>
#include <map>

namespace chimbuko {

enum ADGraphError {
    OK = 0,
    InvalidPath = 1
};

class ADNodeData {
public:
    ADNodeData() : m_mean(0.0), m_std(0.0) {}
    ~ADNodeData() {}

    void set(double mean, double std) {
        m_mean = mean;
        m_std = std;
    }

    double mean() const { return m_mean; }
    double std() const { return m_std; }

private:
    double m_mean;
    double m_std;
};

// function key --> function statistics
//typedef std::unordered_map<std::string, ADNodeData> ADNodeDataTable;
//typedef ADNodeDataTable::const_iterator ADNodeDataTableContIter;
//typedef ADNodeDataTable::iterator ADNodeDataTableIter;

// function name --> (function key, count)
// - count: the number of times 'function name' appears in the graph.
typedef std::unordered_map<std::string, std::pair<std::string, int>> ADNodeTable; 

class ADNode {
public:
    ADNode() {}
    ADNode(std::string id) : m_id(id), m_parent(nullptr) {}
    ADNode(std::string id, ADNode* p) : m_id(id), m_parent(p) 
    {

    }
    ADNode(std::string id, ADNode* p, ADNodeData data)
    : m_id(id), m_parent(p), m_data(data)
    {

    }
    ~ADNode() {}

    std::string id() { return m_id; }
    ADNode* parent() { return m_parent; }

    ADNode* seek_child(std::string id)
    {
        if (m_children.count(id) == 0)
            return nullptr;
        return m_children[id];
    }

    // void add_child(ADNode* c)
    // {

    // }

private:
    //ADNodeDataTableContIter m_id;
    //ADNodeDataTableContIter m_parent;
    //std::vector<ADNodeDataTableContIter> m_children;
    std::string          m_id;
    ADNode*              m_parent;
    //std::vector<ADNode*> m_children;
    
    std::unordered_map<std::string, ADNode*> m_children;
    std::vector<std::unordered_map<std::string, ADNode*>::const_iterator> m_children_order;
    ADNodeData           m_data;
};

class ADGraph {
public:
    ADGraph();
    ADGraph(std::string name);
    ~ADGraph();

    // ADGraphError insert(std::string key, ADNodeData value);
    ADGraphError insert(const std::vector<std::string>& path, std::string key, ADNodeData value);

    void buildFromFile(std::string filename);

private:
    void _init();
    void _destroy();

private:
    std::string     m_name;
    ADNode          m_root;
    //ADNodeDataTable m_cache;
    ADNodeTable     m_node_cache;
};


} // end of namespace chimbuko