#include "algo.h"


algo::algo(int _mode){
	mode = _mode;
	string topoName("output.txt");
	myTopo = topoName;
	readFlow();
}

void algo::readFlow(){
	char data[1024];
	int count = 0;
	int src_num;
	int dst_num;

	ifstream inputFile("flow.txt", ifstream::in);
	while(!inputFile.eof()){
		inputFile.getline(data, 1024);
		char* token;

		token = strtok(data, "\t");


		if(token){
			src_num = atoi(token);
		}
		else break;

		token = strtok(NULL, "\t");

		if(token){
			dst_num = atoi(token);
                }
                else break;

		sources.push_back(src_num);
		dsts.push_back(dst_num);
	}

	inputFile.close();	
}

void algo::run(){
	for(int i = 0; i < sources.size(); i++){
		int src_num = sources[i];
		int dst_num = dsts[i];

		vector < pair < int, double > > sp = myTopo.shortest_path(src_num, dst_num);
		flow_cost[i] = sp.size() - 1;
	}
}

int main(int argc, char* argv[]){
	if(argc < 2){
		cout << "Usage: ./algo <mode>" << endl;
		return 1;
	}
	int mode = atoi(argv[1]);

	switch(mode){
	case OURS:
		cout << "OURS: ";
		break;
	case REDUCE:
		cout << "REDUCE: ";
		break;
	case ORIGIN:
		cout << "ORIGIN: ";
		break;
	default:
		cout << "<< Wroung mode >>" << endl;
		return 1;
	} 

	algo myAlgo(mode);
	if(mode == ORIGIN) myAlgo.run_random();
	else myAlgo.run();
}
