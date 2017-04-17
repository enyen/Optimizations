# Evolutionary Algorithm 
## for Inverse Travelling Salesman Problem

Run: 
	$ ./main <PositionFile> <populationSize> <survivalRate> <maxIteration>

Parameters:
	PositionFile 	= filename of Positions_PA_E.txt
	populationSize 	= size of population
	survivalRate	= fraction of population that survive after external selection
	maxIteration	= maximum iteration the algorithm will run
Example:
	$ ./main ./Positions_PA_E.txt 10000 0.5 5000
	
Output:
	Iteration Used 		= by maxIteration or convergence
	Fitness of fittest	= longest route length in population
	performance.txt		= max, mean and min of parents over iterations
	route.txt			= position sequence of the longest route
	
Plotting:
	$ sudo chmod a+x ./performance.gp ./route.gp
	$ ./performance.gp
	$ ./route.gp
	
(Re)Compile: 
	$ make clean
	$ make
