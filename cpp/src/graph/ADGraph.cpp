#include "chimbuko/graph/ADGraph.hpp"
#include <fstream>
#include <iostream>

using namespace chimbuko;

ADGraph::ADGraph() : m_name("graph") 
{
    _init();
}

ADGraph::ADGraph(std::string name) : m_name(name)
{
    _init();
}

ADGraph::~ADGraph()
{
    _destroy();
}

void ADGraph::_init()
{
    //m_cache["-1"] = ADNodeData();
    m_node_cache["ROOT"] = {"-1", 1};
    m_root = ADNode("-1");
    //m_root.set_id(m_cache.find("-1"));
}

void ADGraph::_destroy()
{
    std::cout << "start destroy" << std::endl;
    std::cout << "end destroy" << std::endl;
}

// There are too many (or maybe not) conserns because of the nature of the call stack tree
// 1. loop: there might be repeated patterns in the horizontal direction (not a problem)
// 2. if-else: ???
// 3. recursive: there might be repeated pattern in the vertical direction
// 4. [problem] how to handle multiple calls of a function under the same parent????
ADGraphError ADGraph::insert(const std::vector<std::string>& path, std::string key, ADNodeData data)
{
    if (path.size() == 0 || path.front() != "-1")
        return ADGraphError::InvalidPath;

    // search for the parent node
    ADNode* p = &m_root;
    for (int i = 1; i < path.size(); i++) {
        p = p->seek_child(path[i]);
    }
    // end of search

    // p->add_child(new ADNode(key, p, data));

    return ADGraphError::OK;
}

static std::vector<std::string> _tokenize(std::string str, std::string delimiter)
{
    size_t pos;
    std::vector<std::string> tokens;
    while (str.size()) {
        pos = str.find_first_of(delimiter);
        if (pos == std::string::npos) {
            tokens.push_back(str);
            str.erase();
        }
        else {
            tokens.push_back(str.substr(0, pos));
            str = str.substr(pos+1);
        }
    }    
    return tokens;
}

void ADGraph::buildFromFile(std::string filename)
{
    std::ifstream fh;

    fh.open(filename);
    if (!fh.is_open())
        throw std::ios_base::failure(
            std::string("ERROR: Cannot open input graph file: ") + filename
        );

    std::string line;
    std::vector<std::string> path;
    bool bNode = false, bEdge = false;    

    path.push_back(m_node_cache["ROOT"].first);
    while (std::getline(fh, line))
    {
        if (line.size() == 0 || line.at(0) == '#')
            continue;

        if (line.find("BEGIN_NODE") != std::string::npos)
        {
            bNode = true;
            continue;
        }
        else if (line.find("END_NODE") != std::string::npos)
        {
            bNode = false;
            continue;
        }
        else if (line.find("BEGIN_EDGE") != std::string::npos)
        {
            bEdge = true;
            continue;
        }
        else if (line.find("END_EDGE") != std::string::npos)
        {
            bEdge = false;
            continue;
        }
        else 
        {
            std::vector<std::string> tokens = _tokenize(line, " ");
            if (bNode)
            {
                if (tokens.size() != 2)
                {
                    throw std::invalid_argument("ERROR: Invalid node argument!");
                }
                m_node_cache[tokens[1]] = {tokens[0], 0}; 
                //std::cout << "NODE: " << tokens[0] << " --> " << tokens[1] << std::endl;
            } 
            else if (bEdge)
            {
                if (tokens.size() < 2)
                {
                    throw std::invalid_argument("ERROR: Invalid edge argument!");
                }                

                if (m_node_cache.count(tokens[0]) == 0)
                {
                    throw std::invalid_argument(
                        std::string("ERROR: Unknown parent node found: ") + tokens[0] 
                    );
                }

                if (m_node_cache.count(tokens[1]) == 0)
                {
                    throw std::invalid_argument(
                        std::string("ERROR: Unknown node found: ") + tokens[1] 
                    );
                }

                while (!path.empty() && path.back() != m_node_cache[tokens[0]].first)
                {
                    path.pop_back();
                }
                if (path.empty())
                    throw std::invalid_argument("ERROR: Invalid path observed!");

                ADNodeData data;
                if (tokens.size() == 4)
                {
                    data.set(std::stod(tokens[2]), std::stod(tokens[3]));
                }

                // std::cout << "PATH: ";
                // for (auto p: path) std::cout << p << " ==> ";
                // std::cout << m_node_cache[tokens[1]].first << std::endl;
                //insert(path, tokens[1], data);

                path.push_back(m_node_cache[tokens[1]].first);
                
                //std::get<0>(m_node_cache[tokens[0]])
                //insert(tokens[0], tokens[1], data);
                // std::cout << "EDGE: " << tokens[0] << " --> " << tokens[1];
                // if (tokens.size() > 2)
                // {
                //     std::cout << " mean: " << tokens[2] << " std: " << tokens[3];
                // }
                // std::cout << std::endl;
            }
        }
    }
}













