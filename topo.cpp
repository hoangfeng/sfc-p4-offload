#include "topo.h"
#include <fstream>

#include <string.h>
#include <stdlib.h>

void link::assign(int id1, int id2, double _cost){
	ends[0] = id1;
	ends[1] = id2;
	capacity = _cost;
	remaining = _cost;
	cost = 1.0 / remaining;
}

topo::topo(){
	topo("output.txt");
}

topo::topo(vector < string > input){
	num_nodes = 0;
	enable_node_cost = true;

	for(int i = 0; i < input.size(); i++){
		int dd;
                int id1;
                int id2;
                double cost;
		sscanf(input[i].c_str(), "%d %d %d %lf\n", &dd, &id1, &id2, &cost);

		if(id1 > num_nodes) num_nodes = id1;
		if(id2 > num_nodes) num_nodes = id2;
	}

	num_nodes++;

	for(int i = 0; i < num_nodes; i++){
                node newNode;
                nodes.push_back(newNode);
        }

	for(int i = 0; i < input.size(); i++){
                int dd;
                int id1;
                int id2;
                double cost;
		int link_id = links.size();
		link newLink;
                sscanf(input[i].c_str(), "%d %d %d %lf\n", &dd, &id1, &id2, &cost);
		newLink.assign(id1, id2, cost);

		nodes[id1].cost = 0.0;
	       	nodes[id1].next = NULL;
        	nodes[id1].id = id1;


		nodes[id1].link_id.push_back(link_id);

                nodes[id2].cost = 0.0;
                nodes[id2].next = NULL;
                nodes[id2].id = id2;
		
                nodes[id2].link_id.push_back(link_id);



		links.push_back(newLink);
                //printf("%d %d %f\n", id1, id2, cost);
        }

	//readNodeCap();
}

topo::topo(string plaintext){
	FILE* input_file = fopen(plaintext.c_str(), "r");
	num_nodes = 0;
        if(!input_file) return;
	while(!feof(input_file)){
		int dd;
                int id1;
                int id2;
                double cost;
		fscanf(input_file, "%d %d %d %lf\n", &dd, &id1, &id2, &cost);

		if(id1 > num_nodes) num_nodes = id1;
		if(id2 > num_nodes) num_nodes = id2;
	}

	num_nodes++;

	for(int i = 0; i < num_nodes; i++){
                node newNode;
                nodes.push_back(newNode);
        }

	fseek(input_file, 0, SEEK_SET);

        while(!feof(input_file)){
                int dd;
                int id1;
                int id2;
                double cost;
		int link_id = links.size();
		link newLink;
                fscanf(input_file, "%d %d %d %lf\n", &dd, &id1, &id2, &cost);
		newLink.assign(id1, id2, cost);

		nodes[id1].cost = 0.0;
	       	nodes[id1].next = NULL;
        	nodes[id1].id = id1;


		nodes[id1].link_id.push_back(link_id);

                nodes[id2].cost = 0.0;
                nodes[id2].next = NULL;
                nodes[id2].id = id2;
		
                nodes[id2].link_id.push_back(link_id);



		links.push_back(newLink);
                //printf("%d %d %f\n", id1, id2, cost);
        }
        fclose(input_file);
}

void topo::setNodeCostConsider(bool input){
	enable_node_cost = input;
}

vector < pair < int, double > > topo::shortest_path(int ingress, int egress){
	node* path = shortest_path_proc(ingress, egress);
	vector < pair < int, double > > ret;

	while(path){
		//cout << "check 2  id = " << path->id << endl;
		pair < int, double > newNode;
		newNode.first = path->id;
		newNode.second = path->cost;

		ret.push_back(newNode);
		path = path->next;
	}

	return ret;
}

vector < node* > topo::spanning_tree_proc(int ingress, map < int, bool > egress){
	nodes[ingress].next = NULL;
	nodes[ingress].cost = 0.0;

	vector < node* > ret;

	done.clear();
	horizon.clear();

	int processID = ingress;
	int count = 0;
	do{
		//cout << "check 1 id = " << processID << endl;

		getDone(processID);
		double minCost = -1.0;
		for(map< int , bool >::iterator it=horizon.begin(); it!=horizon.end(); ++it){
			if(minCost == -1 || minCost > nodes[it->first].cost){
				minCost = nodes[it->first].cost;
				processID = it->first;
			}
			
		}

		if(egress.find(processID) != egress.end()){
			ret.push_back(&nodes[processID]);
			egress.erase(processID);
		}


		count++;
	} while(horizon.size() != 0 && egress.size() > 0);
	return ret;
}

map < int, bool > topo::spanning_tree_proc(int ingress, double cost_threshold){
	nodes[ingress].next = NULL;
	nodes[ingress].cost = 0.0;

	vector < node* > ret;

	done.clear();
	horizon.clear();

	int processID = ingress;
	int count = 0;
	do{
		//cout << "check 1 id = " << processID << endl;

		getDone(processID, cost_threshold);
		double minCost = -1.0;
		for(map< int , bool >::iterator it=horizon.begin(); it!=horizon.end(); ++it){
			if(minCost == -1 || minCost > nodes[it->first].cost){
				minCost = nodes[it->first].cost;
				processID = it->first;
			}
			
		}

		/*
		if(egress.find(processID) != egress.end()){
			ret.push_back(&nodes[processID]);
			egress.erase(processID);
		}
		*/

		count++;
	} while(horizon.size() != 0);
	return done;
}

node* topo::shortest_path_proc(int ingress, int egress){

	nodes[ingress].next = NULL;
	nodes[ingress].cost = 0.0;

	done.clear();
	horizon.clear();

	int processID = ingress;
	int count = 0;
	do{
		getDone(processID);
		float minCost = -1.0;
		for(map< int , bool >::iterator it=horizon.begin(); it!=horizon.end(); ++it){
			if(minCost == -1 || minCost > nodes[it->first].cost){
				minCost = nodes[it->first].cost;
				processID = it->first;
			}
			
		}
		if(processID == egress){
			return &nodes[processID];
		}


		count++;
	} while(horizon.size() != 0);
	return NULL;
}

void topo::getDone(int id){
	horizon.erase(id);
	done[id] = true;

	for(map < int , bool >::iterator it=horizon.begin(); it!=horizon.end(); ++it){
		int link_cost = getCost(id, it->first);
		if(link_cost == -1.0) continue;
		if(nodes[it->first].cost > nodes[id].cost + link_cost){
			nodes[it->first].cost = nodes[id].cost + link_cost;
			nodes[it->first].next = &nodes[id];
		}
	}


	for(int i = 0; i < nodes[id].link_id.size(); i++){
	//for(int i = 0; i < link_count; i++){
		int new_ID = -1;
		double cost = 0.0;


		if(links[nodes[id].link_id[i]].ends[0] == id){

			new_ID = links[nodes[id].link_id[i]].ends[1];
			cost = links[nodes[id].link_id[i]].cost;
			if(enable_node_cost) cost += 1.0 / nodes[id].remaining;

		}
		else if(links[nodes[id].link_id[i]].ends[1] == id){
			new_ID = links[nodes[id].link_id[i]].ends[0];
			cost = links[nodes[id].link_id[i]].cost;

			if(enable_node_cost) cost += 1.0 / nodes[id].remaining;
		}

		if(new_ID != -1 && horizon.find(new_ID) == horizon.end() 
			&& done.find(new_ID) == done.end()){


			horizon[new_ID] = true;
			//nodes[new_ID] = new node();
			nodes[new_ID].cost = cost + nodes[id].cost;
			nodes[new_ID].next = &nodes[id];
			//nodes[new_ID]->id = new_ID;
		}
	}
}

void topo::getDone(int id, double cost_threshold){
	horizon.erase(id);
	done[id] = true;

	for(map < int , bool >::iterator it=horizon.begin(); it!=horizon.end(); ++it){
		int link_cost = getCost(id, it->first);
		if(link_cost == -1.0) continue;
		if(nodes[it->first].cost > nodes[id].cost + link_cost){
			nodes[it->first].cost = nodes[id].cost + link_cost;
			nodes[it->first].next = &nodes[id];
		}
	}


	for(int i = 0; i < nodes[id].link_id.size(); i++){
	//for(int i = 0; i < link_count; i++){
		int new_ID = -1;
		float cost = 0.0;


		if(links[nodes[id].link_id[i]].ends[0] == id){

			new_ID = links[nodes[id].link_id[i]].ends[1];
			cost = links[nodes[id].link_id[i]].cost;


		}
		else if(links[nodes[id].link_id[i]].ends[1] == id){
			new_ID = links[nodes[id].link_id[i]].ends[0];
			cost = links[nodes[id].link_id[i]].cost;
		}

		if(new_ID != -1 && horizon.find(new_ID) == horizon.end() 
			&& done.find(new_ID) == done.end() 
			&& (cost + nodes[id].cost) <= cost_threshold){


			horizon[new_ID] = true;
			//nodes[new_ID] = new node();
			nodes[new_ID].cost = cost + nodes[id].cost;
			nodes[new_ID].next = &nodes[id];
			//nodes[new_ID]->id = new_ID;
		}
	}
}

double topo::getCost(int start, int end){
	for(int i = 0; i < nodes[start].link_id.size(); i++){
		if(links[nodes[start].link_id[i]].ends[0] == end
			|| links[nodes[start].link_id[i]].ends[1] == end)
				return links[nodes[start].link_id[i]].cost;
	}

	return -1.0;
}

double topo::getRemaining(int start, int end){
	for(int i = 0; i < nodes[start].link_id.size(); i++){
		if(links[nodes[start].link_id[i]].ends[0] == end
			|| links[nodes[start].link_id[i]].ends[1] == end)
				return links[nodes[start].link_id[i]].remaining;
	}

	return -1.0;
}

void topo::setCost(int start, int end, double cost){
	for(int i = 0; i < nodes[start].link_id.size(); i++){
		if(links[nodes[start].link_id[i]].ends[0] == end
			|| links[nodes[start].link_id[i]].ends[1] == end)
				links[nodes[start].link_id[i]].cost = cost;
	}

}

void topo::readNodeCap(){
	FILE* input_file = fopen("nodeCapacity.txt", "r");

	total_capacity = 0;	
	while(!feof(input_file)){
		int nodeID;
		int nodeCap;
		fscanf(input_file, "%d\t%d", &nodeID, &nodeCap);
	
		total_capacity += nodeCap;
		nodes[nodeID].capacity = nodeCap;
		nodes[nodeID].remaining = nodeCap;
	}

	fclose(input_file);
}

int topo::getTotalCap(){
	return total_capacity;
}

void topo::dump(){
	cout << "nodes:" << endl;

	for(int i = 0; i < nodes.size(); i++){
		cout << "node " << i << ":" << endl;
		for(int j = 0; j < nodes[i].link_id.size(); j++){
			cout << "  " << links[nodes[i].link_id[j]].ends[0] << " --> "
				<< links[nodes[i].link_id[j]].ends[1] << endl;
		}
	}
}

