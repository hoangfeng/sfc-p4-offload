#include <stdio.h>
#include <stdlib.h>
#include "topo_generator.h"
#include <map>

nodeUnit::nodeUnit(int nodeID_in){
	nodeID = nodeID_in;
	//next = NULL;
}

linkUnit::linkUnit(nodeUnit* end0_in, nodeUnit* end1_in){
	end0 = end0_in;
	end1 = end1_in;
	weight = (double) (rand() % 1000000) / 10000.0;
}

groupUnit::groupUnit(int nodeID_in){
	nodeUnit* firstNode = new nodeUnit(nodeID_in);
	nodeList.push_back(firstNode);
}

topoUnit::topoUnit(int num_of_node_in){
	num_of_node = num_of_node_in;

	for(int i = 0; i < num_of_node_in; i++){
		groupUnit newGroup(i);
		groupList.push_back(newGroup);
	}
}

int topoUnit::process(){
	while(groupList.size() > 1){
		int index_group1 = 0;
		int index_group2 = 0;

		while(index_group1 == index_group2){
			index_group1 = rand() % groupList.size();
			index_group2 = rand() % groupList.size();
		}

		connectGroup(&groupList[index_group1], &groupList[index_group2]);
		//delete groupList[index_group2];
		groupList.erase(groupList.begin() + index_group2);
	}
}

/*
groupUnit* topoUnit::getGroup(int index){
	groupUnit* retGroup;
	if(num_of_node <= index || index < 0) return NULL;
	if(index == 0){
		retGroup = groupList;
		groupList = groupList->next;
		return retGroup;
	}

	int count = index - 1;
	groupUnit* tempGroup = groupList;
	for(int i = 0; i < count; i++)
		tempGroup = tempGroup->next;
	
	retGroup = tempGroup->next;
	tempGroup = retGroup->next;

	
	num_of_node--;
	return retGroup;
}
*/

void topoUnit::connectGroup(groupUnit* group1, groupUnit* group2){
	nodeUnit* node1 = group1->nodeList[rand() % group1->nodeList.size()];
	nodeUnit* node2 = group2->nodeList[rand() % group2->nodeList.size()];
	linkUnit newLink(node1, node2);

	linkList.push_back(newLink);
	for(int i = 0; i < group2->nodeList.size(); i++){
		group1->nodeList.push_back(group2->nodeList[i]);
	}
}

void topoUnit::output(char* fileName ){
	FILE* output = fopen(fileName, "w");

	for(int i = 0; i < linkList.size(); i++){
		double newCost = (double) (rand() % 10) + 1.0;
		//newCost = 1.0;
		//fprintf(output, "%d\t%d\t%d\t%f\n", i, linkList[i].end0->nodeID, linkList[i].end1->nodeID, linkList[i].weight);
                //hoangeidt
//		fprintf(output, "%d\t%d\t%d\t%f\t%d\n", i, linkList[i].end0->nodeID, linkList[i].end1->nodeID, rand() % no_type , newCost);
		fprintf(output, "%d \t %d \t %d \t %f \n", i, linkList[i].end0->nodeID, linkList[i].end1->nodeID, newCost);
	}

	fclose(output);
}



void topoUnit::setLink(int num_of_link){
	printf("num of link 1 = %d\n", linkList.size());
	if(num_of_link > linkList.size()){
		int numExtraLink = num_of_link - linkList.size();
		for(int i = 0; i < numExtraLink; i++){
			linkUnit newLink(NULL, NULL); // = NULL;
			do{
				//if(newLink) delete newLink;
				int index_end0 = rand() % groupList[0].nodeList.size();
				int index_end1 = rand() % groupList[0].nodeList.size();

				while(index_end0 == index_end1){
					index_end0 = rand() % groupList[0].nodeList.size();
					index_end1 = rand() % groupList[0].nodeList.size();
				}

				//newLink = new linkUnit(groupList[0].nodeList[index_end0]
				//	, groupList[0].nodeList[index_end1]);
				newLink.end0 = groupList[0].nodeList[index_end0];
				newLink.end1 = groupList[0].nodeList[index_end1];

			}while(searchDuplicate(&newLink));

			linkList.push_back(newLink);
		}
	}
	printf("num of link 2 = %d\n", linkList.size());
}

bool topoUnit::searchDuplicate(linkUnit* newLink){
	for(int i = 0; i < linkList.size(); i++){
		if(linkList[i].end0->nodeID == newLink->end0->nodeID 
			&& linkList[i].end1->nodeID == newLink->end1->nodeID)
				return true;
		if(linkList[i].end0->nodeID == newLink->end1->nodeID
			&& linkList[i].end1->nodeID == newLink->end0->nodeID)
				return true;
	}
	return false;
}

void renderNetworkFunction (char* fileName, int no_node, int no_function){
    FILE* output = fopen(fileName, "w");
    for( int i=0; i < no_node; i++){        
        int get = rand()%4;
        fprintf(output, "%d\t", i );
        int temp;
        map <int,bool> check;
        for(int i = 0; i < get; ++i){
            do{
                temp = rand() % (no_function );
            }while(check.find(temp) != check.end());
            fprintf(output, "%d\t", temp );
            check[temp] = true;
        }
        fprintf(output, "\n");
    }
    fclose(output);
}

int main(int argc, char *argv[]){
        
    // 1st argv number of nodes
    // 2nd argv seed number
    // 3th argv number of node type

	if(argc != 3){
		cout << "One parameter is needed" << endl;
		exit(1);
	}

	int num_node = atoi(argv[1]);
	int seed = atoi(argv[2]);

    // int num_node = 600;
    // int seed = 1;

    srand(seed);
    renderNetworkFunction("function.txt",num_node, 30);
    topoUnit topo(num_node);
    topo.process();
    printf("link num = %d\n", topo.linkList.size());
    topo.setLink(num_node * 2);
    topo.output("output.txt");
}
