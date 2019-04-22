for seed in $(seq 1 9)
do
	echo "seed = $seed:"
	./topo_generator 5 $seed
	./getFlow.pl 7 $seed
	./algo 1
	./algo 2
	./algo 3
done
