#include <iostream>
#include <vector>
#include <map>

using namespace std;

class link{
public:
	void assign(int id1, int id2, double _cost);
	int ends[2];
	double cost;
	double capacity;
	double remaining;
};

class node{
public:
        int id;
        double capacity;
	double remaining;
        node* next;
        double cost;
        vector < int > link_id;
};

class topo{
public:
	topo(vector < string > input);
	topo(string plaintext);
	topo();
	vector < pair < int, double > > shortest_path(int ingress, int egress);
	void dump();	
	void algo();
	map < int, bool > spanning_tree_proc(int ingress, double cost_threshold);
	double getCost(int start, int end);
	double getRemaining(int start, int end);
	void setCost(int start, int end, double cost);
	int getTotalCap();
	void setNodeCostConsider(bool input);
	vector < node > nodes;
	vector < link > links;

private:
	map < int, bool > done;
	map < int, bool > horizon;
	int num_nodes;
	int total_capacity;
	bool enable_node_cost;
	void getDone(int id);
	void getDone(int id, double cost_threshold);
	node* shortest_path_proc(int ingress, int egress);
	vector < node* > spanning_tree_proc(int ingress, map < int, bool > egress);
	void readNodeCap();
};
