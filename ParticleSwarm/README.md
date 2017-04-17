#Particle Swarm Algorithm 
##for Inverse Travelling Salesman Problem

Run: 
	$ ./main <zFile> <paramFile>

Parameters:
	zFile 	  = filename of time series file
	paramFile = filename of parameter file

Example:
	$ ./main ./PA-G-series-01.dat ./param.txt
	
Output:
	Iteration Used 		= by maxIteration or convergence
	Fitness of fittest	= negative of squared difference error of fittest
	result.txt			= prediction of time series based on the weight of fittest
	performance.txt		= fitness of fittest on training and testing data over iterations
	w.txt				= weights of the fittest
	
Plotting:
	$ sudo chmod a+x ./performance.gp ./result.gp
	$ ./performance.gp
	$ ./result.gp
	
(Re)Compile: 
	$ make clean
	$ make
