echo "start"
./topo_generator 600 1
./getFlow.pl 70 1
./algo 1
./algo 2
./algo 3
python visual/run.py

