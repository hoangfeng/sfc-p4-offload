all:algo topo_generator
algo:topo.cpp algo.cpp
	g++ -o algo topo.cpp algo.cpp -w
topo_generator:getPath.cpp topo_generator.cpp
	g++ -o topo_generator getPath.cpp topo_generator.cpp -w
clean:
	rm -f algo
	rm -f topo_generator
