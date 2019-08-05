#include "chimbuko/graph/ADGraph.hpp"

using namespace chimbuko;

int main(int argc, char ** argv)
{
    std::string inputFile = argv[1];

    ADGraph g("mygraph");

    g.buildFromFile(inputFile);

    return 0;
}