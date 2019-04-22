#include "topo.h"

#include <iostream>
#include <fstream>

#include <string.h>
#include <stdlib.h>

#define SHOW_TREE 0
#define SHOW_NODE 25
#define DEBUG 0

#define BETA 0.3

#define ALPHA 1.5
#define OURS 1
#define REDUCE 2
#define ORIGIN 3

class algo{
public:
    algo(int _mode);
    void run();
    void run_random();
    void evaluation();
private:
    vector < int > sources;
    vector < int > dsts;
    map < int, int > flow_cost;
    void readFlow();
    topo myTopo;
    int mode;
};

