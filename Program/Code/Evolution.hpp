/*
* Alexander Ocheretyany
*
* Class for the Evolutionary Algorithm
*
* Charles University in Prague, 2019
*/

#include "pch.h"

class Individual {

	std::vector<size_t> ind; // Vector of mappings
	size_t length; // Length of an individual
	size_t max; // Number of cells
	size_t normal; // Normal number of vertices in a cell
	size_t fitness; // Fitness
	std::vector<std::list<size_t>> graph; // Graph
	size_t numOfEdges; // Number of edges in the graph

public:	

	Individual() {} // For containers

	Individual(size_t length, std::vector<std::list<size_t>> &graph, size_t numOfEdges) {
		this -> length = length;
		this -> graph = graph;
		this -> numOfEdges = numOfEdges;
		ind.reserve(length);
	}

	bool operator == (Individual ind_1){
		for (size_t i = 0; i < ind.size(); ++i) {
			if (ind[i] != ind_1.getState(i)) return false;
		}
		return true;
	}

	// size_t getState(size_t index) - returns the bin number at index
	size_t getState(size_t index) {
		if(index < ind.size()) return ind[index];
		return -1;
	}

	// size_t setState(size_t index, size_t value) - sets the bin number = value at index
	void setState(size_t index, size_t value) {
		ind[index] = value;
	}

	// void fillRandomly(size_t normal, size_t max) - fills the individual randomly and correctly sized
	void fillRandomly(size_t normal, size_t max) {
		
		this->max = max;
		this->normal = normal;

		size_t total = normal * max; // Total number of vacant places
		std::vector<size_t> tmp; // A vector of cluster indices

		size_t index = 0; // Start index

		// Define all possible vacant places in a partition
		for (size_t i = 0; i < max; ++i) {
			for (size_t j = 0; j < normal; ++j) {
				tmp.push_back(i);
			}
		}
		// ------------------------------------------------

		// Randomize
		std::random_shuffle(tmp.begin(), tmp.end()); // Permute

		// Fill in the individual
		for (size_t i = 0; i < length; ++i) {
			ind.push_back(tmp.back());
			tmp.pop_back();
		}

		calculateFitness();
	}

	// void calculateFitness() - calculates number of preserved (not cut) edges as fitness
	void calculateFitness() {
		size_t numOfCrossedEdges = 0;
		for (size_t i = 0; i < ind.size() - 1; ++i) {
			for (size_t j = i + 1; j < ind.size(); ++j) {
				if (ind[i] != ind[j]) {
					// Check, whether an edge is crossed
					for (auto x : graph[i]) {
						if (x == j) {
							++numOfCrossedEdges;
							break;
						}
					}
				}
			}
		}
		fitness = numOfEdges - numOfCrossedEdges;
	}

	// size_t getLength() - returns the length of the individual
	size_t getLength() {
		return length;
	}

	// size_t getFitness() - returns the fitness of the individual
	size_t getFitness() {
		return fitness;
	}

	// size_t getNumOfPartitions() - returns the number of partitions
	size_t getNumOfPartitions() {
		return max;
	}

	// void setFitness(size_t fitness) - sets fitness manually
	void setFitness(size_t fitness) {
		this->fitness = fitness;
	}

	void updateFitness(Individual &initial, size_t start, size_t end, std::pair<size_t, size_t> pair = std::pair<size_t, size_t>()) {
		size_t updatedFitness = initial.getFitness(); // Initial fitness
		size_t formerFriends = 0; // Count of neighbours in the same cell in the initial individual
		size_t friends = 0; // Count of neighbours in the same cell in the final individual
		bool repeat = false; // Flag

		if (pair.first != pair.second) {
			start = pair.first;
			end = start + 1;
			repeat = true;
		}

		do {
			for (size_t i = start; i < end; ++i) { // For each vertex in the range
				for (auto x : graph[i]) {
					if (i < x || x < start) { // We need i < x so as not to count twice the same vertices
						if (initial.getState(i) == initial.getState(x)) ++formerFriends;
						if (getState(i) == getState(x)) ++friends;
					}
				}
				int difference = friends - formerFriends;

				updatedFitness += difference;

				formerFriends = 0;
				friends = 0;
			}
			if (repeat) {
				if (start == pair.second) repeat = false; // Stop the cycle
				start = pair.second;
				end = start + 1;
			}
		} while (repeat);
		setFitness(updatedFitness); // Update fitness of the final individual
	}

	// const std::vector<size_t> getStateVector() - returns the vector of states
	const std::vector<size_t> getStateVector() {
		return ind;
	}

	// setStateVector(std::vector<size_t> vec) - sets vec as the vector of states
	void setStateVector(std::vector<size_t> vec) {
		ind = vec;
	}

	// bool isCorrect() - returns true if the individual is correctly sized
	bool isCorrect() {
		std::vector<size_t> vec(max);
		for (auto x : ind) {
			++vec[x];
		}
		for (auto x : vec) {
			if (x > normal) {
				return false;
			}
		}
		return true;
	}

	// void printInd(std::ostream &outStream) - prints out the individual
	void printInd(std::ostream &outStream) {
		for (auto x : ind) {
			outStream << x << " ";
		}
		outStream << std::endl;
	}

	void rename() {
		size_t index_1 = rand() % max;
		size_t index_2;
		do(index_2 = rand() % max);
		while(index_1 == index_2);

		for (auto x : ind) {
			if (x == index_1) x = index_2;
			else if (x == index_2) x = index_1;
		}
	}

};

class Population {
	std::vector<Individual> pop; // Population
	size_t size; // Size of the population
	std::vector<std::list<size_t>> graph;
	size_t numOfEdges; // Number of edges for fitness
public:

	Population() = default;

	Population(size_t size, std::vector<std::list<size_t>> &graph) {
		this->size = size;
		this->graph = graph;
		pop.reserve(size);
		numOfEdges = calculateNumberOfEdges(graph); // Getting the number of edges for fitness function
	}

	// void fillRandomly(size_t length, size_t normal, size_t max) - fills the population with random individuals
	void fillRandomly(size_t length, size_t normal, size_t max) {
		std::cout << std::endl << "-> Growth of the random population size: "; // Status
		Individual ind;
		for (size_t i = 0; i < size; ++i) {
			ind = Individual(length, graph, numOfEdges);
			ind.fillRandomly(normal, max);
			pop.emplace_back(ind);
			std::cout << i << " ";
		}
		std::cout << std::endl;
	}

	// std::vector<Individual> getIndividuals() - returns the individuals of the popualtion
	std::vector<Individual>& getIndividuals() {
		return pop;
	}

	// size_t calculateNumberOfEdges(std::vector<std::list<size_t>> &graph) - gets the number of edges in a given graph
	size_t calculateNumberOfEdges(std::vector<std::list<size_t>> &graph) {
		size_t num = 0; // Initial value
		size_t size = graph.size();
		std::vector<std::vector<bool>> table;
		table.resize(size);
		for (auto x : table) {
			x.resize(size);
		}
		for (auto x : graph) {
			num += x.size();
		}
		num /= 2;
		return num;
	}

	size_t getNumberOfEdges() {
		return numOfEdges;
	}

	size_t getPopulationSize() {
		return pop.size();
	}

	Individual getBestInd() {
		Individual bestInd = pop[0];
		for (Individual x : pop) {
			if (x.getFitness() > bestInd.getFitness()) {
				bestInd = x;
			}
		}
		return bestInd;
	}

	// std::vector<Individual> getBest(size_t percentage) - returns the best percentage% of individuals of the population
	std::vector<Individual> getBest(size_t percentage) {
		std::multimap<size_t, size_t> map; // Map
		for (size_t i = 0; i < pop.size(); ++i) {
			map.emplace(pop[i].getFitness(), i);
		}
		size_t size = std::ceil(((float)pop.size() / 100) * percentage);
		std::vector<Individual> bestIndividuals;
		auto it = map.end();
		for (size_t i = 0; i < size; ++i) {
			bestIndividuals.push_back(pop[(--it)->second]);
		}
		if(bestIndividuals.size() % 2 != 0) bestIndividuals.push_back(pop[(--it)->second]); // If not an even number
		return bestIndividuals;
	}

	// float getAverageFitness() - returns the average fitness among all individuals of the population
	float getAverageFitness() {
		size_t sumFitness = 0;
		for (auto x : pop) {
			sumFitness += x.getFitness();
		}
		return (float)sumFitness / size;
	}

	// Individuals getIndividual(size_t index) - 
	Individual& getIndividual(size_t index) {
		return pop[index];
	}

	// void addToPopulation(size_t index, Individual &ind) - adds the ind to the population at index
	void addToPopulation(Individual &ind) {
		pop.push_back(ind);
	}

	// void killAll() - kills all individuals in the population
	void killAll() {
		pop.clear();
		pop.shrink_to_fit();
		pop.reserve(size);
	}

	// void printPopulation() - prints the whole population out
	void printPopulation() {
		for (auto x : pop) {
			for (size_t i = 0; i < x.getLength(); ++i) {
				std::cout << x.getState(i) << " ";
			}
			std::cout << "with fitness: ";
			std::cout << x.getFitness();
			std::cout << std::endl;
		}
	}
};

class Selector {
public:
	virtual Individual& select(Population &pop) = 0;
};

class RouletteWheelSelector : public Selector {
public:
	Individual& select(Population &pop) override {

		size_t sumFitness = 0;
		for (size_t i = 0; i < pop.getPopulationSize(); ++i) sumFitness += pop.getIndividual(i).getFitness(); // Get sum of all fitness functions through the population

		// Split the wheel into sectors
		std::vector<float> sectors(pop.getPopulationSize());
		float previous_probability = 0.0;
		size_t index = 0;
		auto start = pop.getIndividuals().begin();
		auto end = pop.getIndividuals().end();
		for (auto i = start; i < end; ++i) {
			sectors[index] = previous_probability + ((float)(*i).getFitness() / sumFitness);
			previous_probability = sectors[index++];
		}
		// ----------------------------

		float strike = rand() / float(RAND_MAX);

		previous_probability = 0.0;
		index = -1;

		// Choose the winner (binary search)
		index = sectors.size() / 2;
		size_t left = -1;
		size_t right = sectors.size();
		while (index != sectors.size() - 1 && index != 0 && (sectors[index] > strike || sectors[index + 1] < strike)) {
			if (strike > sectors[index + 1]) {
				left = index;
				if (right - left != 1) index += (right - left) / 2;
				else index += 1;
			}
			else {
				right = index;
				if (right - left != 1) index -= (right - left) / 2;
				else index -= 1;
			}
		}

		return pop.getIndividual(index);
	}
};

class TournamentSelector : public Selector {

	float probOfBest;

public:
	TournamentSelector(float probOfBest) : probOfBest(probOfBest) {}

	Individual& select(Population &pop) override {
		size_t index_1 = rand() % (pop.getPopulationSize() - 1); // Get a random index 1
		size_t index_2 = rand() % pop.getPopulationSize(); // Get the second index distinct from index 1
		if (index_2 == index_1) ++index_2; // That's why index_1 is always 1 less than the last index

		// Put the best individual on the 1st place
		if (pop.getIndividual(index_2).getFitness() > pop.getIndividual(index_1).getFitness()) {
			size_t tmp = index_2;
			index_2 = index_1;
			index_1 = tmp;
		}

		float random = rand() / float(RAND_MAX);

		if (random <= probOfBest) {
			return pop.getIndividual(index_1); // Return the best
		}
		else return pop.getIndividual(index_2); // Return the worst
	}
};

class Crossover {
protected:
	float probOfXover;
public:
	virtual std::pair<Individual, Individual> XOver(Population &pop, Individual ind_1, Individual ind_2) = 0;
};

class FairCrossover : public Crossover {
public:
	FairCrossover(float probOfXover) {
		this->probOfXover = probOfXover;
	}

	std::pair<Individual, Individual> XOver(Population &pop, Individual ind_1, Individual ind_2) override {
		float random = rand() / float(RAND_MAX);
		if (random < probOfXover) {
			Individual init_1 = ind_1; // Initial state of ind_1
			Individual init_2 = ind_2; // Initial state of ind_2

			size_t crossPointIndex = (rand() % (ind_1.getLength() - 2)) + 1; // Get any index but the first and the last

			size_t lowerBound = 0; // LHS is smaller than RHS
			size_t upperBound = crossPointIndex;
			if (ind_1.getLength() - crossPointIndex < crossPointIndex) {
				lowerBound = crossPointIndex; // RHS is smaller than LHS
				upperBound = ind_1.getLength();
			}

			// Initial distribution
			std::vector<size_t> sectors_ind_1(ind_1.getNumOfPartitions());
			std::vector<size_t> sectors_ind_2(ind_2.getNumOfPartitions());
			for (size_t i = lowerBound; i < upperBound; ++i) {
				++sectors_ind_1[ind_1.getState(i)];
				++sectors_ind_2[ind_2.getState(i)];
			}
			// --------------------

			// Swap parts
			size_t tmp; // Temporary state
			for (size_t i = lowerBound; i < upperBound; ++i) {
				tmp = ind_1.getState(i);
				ind_1.setState(i, ind_2.getState(i));
				ind_2.setState(i, tmp);
			}
			// ----------

			// Difference vector
			std::vector<int> diffVector(ind_1.getNumOfPartitions());
			for (size_t i = 0; i < sectors_ind_1.size(); ++i) {
				diffVector[i] = sectors_ind_1[i] - sectors_ind_2[i];
			}
			// -----------------

			// Stack of needed sectors
			std::vector<size_t> stack;
			for (size_t i = 0; i < diffVector.size(); ++i) {
				while (diffVector[i] > 0) {
					stack.push_back(i);
					--diffVector[i];
				}
			}
			// -----------------------

			std::random_shuffle(stack.begin(), stack.end()); // Permute the stack of lack elements

			// Fix ind_1
			for (size_t i = lowerBound; i < upperBound; ++i) {
				if (diffVector[ind_1.getState(i)] != 0) {
					size_t state = ind_1.getState(i);
					ind_1.setState(i, stack.back());
					stack.pop_back();
					++diffVector[state];
				}
			}

			// Difference vector
			diffVector.resize(ind_2.getNumOfPartitions());
			for (size_t i = 0; i < sectors_ind_2.size(); ++i) {
				diffVector[i] = sectors_ind_2[i] - sectors_ind_1[i];
			}
			// -----------------

			// Stack of needed sectors
			for (size_t i = 0; i < diffVector.size(); ++i) {
				while (diffVector[i] > 0) {
					stack.push_back(i);
					--diffVector[i];
				}
			}
			// -----------------------

			// Fix ind_2
			for (size_t i = lowerBound; i < upperBound; ++i) {
				if (diffVector[ind_2.getState(i)] != 0) {
					size_t state = ind_2.getState(i);
					ind_2.setState(i, stack.back());
					stack.pop_back();
					++diffVector[state];
				}
			}

			ind_1.updateFitness(init_1, lowerBound, upperBound);
			ind_2.updateFitness(init_2, lowerBound, upperBound);
		}
		return std::make_pair(ind_1, ind_2);
	}
};

class Mutation {
protected:
	float probOfMutation = 0;
public:
	virtual void mutate(Individual &ind) = 0;
};

class BigBangMutation : public Mutation {
public:
	BigBangMutation(float probOfMutatiee1on) {
		this->probOfMutation = probOfMutation;
	};
	void mutate(Individual &ind) override {
		float random = rand() / float(RAND_MAX);
		if (random < probOfMutation) {
			std::vector<size_t> vec = ind.getStateVector();
			std::random_shuffle(vec.begin(), vec.end());
			ind.setStateVector(vec);
			ind.calculateFitness();
		}
	}
};

class ShiftMutation : public Mutation {
public:
	ShiftMutation(float probOfMutation) {
		this->probOfMutation = probOfMutation;
	};

	void mutate(Individual &ind) override {
		float random = rand() / float(RAND_MAX);
		if (random < probOfMutation) {
			Individual initial = ind;
			size_t shift = rand() % (ind.getLength() - 1) + 1;
			for (size_t i = 0; i < ind.getLength(); i++) {
				ind.setState(i, initial.getState(shift++));
				if (shift > ind.getLength() - 1) shift = 0;
			}
			ind.calculateFitness();
		}
	}
};

class RandomMutation : public Mutation {
public:
	RandomMutation(float probOfMutation) {
		this->probOfMutation = probOfMutation;
	};

	void mutate(Individual &ind) override {
		float random = rand() / float(RAND_MAX);
		if (random < probOfMutation) {
			Individual initial = ind;
			size_t pos_1 = rand() % ind.getLength();
			size_t pos_2;
			do pos_2 = rand() % ind.getLength();
			while (pos_2 == pos_1 || ind.getState(pos_1) == ind.getState(pos_2));
			// Swap
			size_t tmp = ind.getState(pos_1);
			ind.setState(pos_1, ind.getState(pos_2));
			ind.setState(pos_2, tmp);
			ind.updateFitness(initial, 0, 0, std::make_pair(pos_1, pos_2));
		}
	}
};

class UniformMutation : public Mutation {	
	float probOfBitFlip;
public:
	UniformMutation(float probOfMutation, float probOfBitFlip) : probOfBitFlip(probOfBitFlip) {
		this->probOfMutation = probOfMutation;
	}

	void mutate(Individual &ind) override {
		float random = rand() / float(RAND_MAX);
		if (random < probOfMutation) {
			Individual init = ind;
			size_t pos;
			for (size_t i = 0; i < ind.getLength(); ++i) {
				if (probOfMutation <= probOfBitFlip) {
					do pos = rand() % ind.getLength();
					while (pos == i || ind.getState(i) == ind.getState(pos));
					size_t tmp = ind.getState(i);
					ind.setState(i, ind.getState(pos));
					ind.setState(pos, tmp);
				}
			}
			ind.calculateFitness();
		}
	}
};

class Evolution {

	std::vector<std::list<size_t>> graph; // Graph
	size_t numOfPartitions; // Number of partitions
	size_t normal; // Normal size of a single cell
	size_t sizeOfPopulation; // Size of a single population
	float probOfMutation; // Probability of mutation
	float probOfBitFlip; // Probability of bit flipping
	float probOfShiftMutation; // Probability of shift mutation
	Selector *selector; // Selector
	Crossover *crossover; // Crossover
	Mutation *mutation; // Mutation
	size_t opt = -1; // Optimum
	size_t numOfGen = -1; // Number of generations
	bool killParents; // Flag for a strategy
	size_t percentageOfBest; // Percentage of the best individuals transfered to the next generation
	Individual globalBestInd; // The best individual among all generations
	bool stopFlag; // Flag showing whether the algorithm must be finished
	std::ofstream logStream; // Logging stream
	clock_t startTime, endTime, curTime; // Start, end and current time of the algorithm
	Population currentPopulation, nextGeneration;

public:

	Evolution(Data &mainData) {
		graph = mainData.getGraph();
		numOfPartitions = mainData.getNumOfPartitions();
		normal = mainData.getRatio();
		logStream = std::ofstream("Log.txt"); // Log file
	}

	~Evolution() {
		if (endTime == NULL) {
			endTime = clock();
			double elapsed_secs = double(endTime - startTime) / CLOCKS_PER_SEC;
			std::cout << "Time taken by EVA: " << elapsed_secs << " s" << std::endl << std::endl;
			logStream << "Time taken by EVA: " << elapsed_secs << " s" << std::endl << std::endl;
		}
		logStream.close(); // Close log file
	}

	// void error() - prints an error and finishes the program
	void error() {
		std::cout << "\nError: Wrong value!" << std::endl;
		exit(0);
	}

	Population getInitialPopulation(std::vector<std::list<size_t>> &graph, size_t size, size_t normal, size_t max) {
		Population population(size, graph);
		population.fillRandomly(graph.size(), normal, max);
		return population;
	}

	void startEvolution() {

		init(); // Initial dialogue
		initFromFile("EVALastParams.txt", logStream); // Initial log file
		startTime = clock(); // Start clock
		currentPopulation = getInitialPopulation(graph, sizeOfPopulation, normal, numOfPartitions); // Initial population
		// Empty space for new generation
		nextGeneration = Population(sizeOfPopulation, graph); // New generation
		// ------------------------------
		run(currentPopulation); // Run the algorithm
		endTime = clock();
		// ---------

		// Output
		std::cout << std::endl << "------- RESULTS OF EVA -------" << std::endl;
		std::cout << "Fitness of the best individual found: " << globalBestInd.getFitness() << std::endl;
		std::cout << "The best partition found: ";
		globalBestInd.printInd(std::cout);
		std::cout << "Cost of the solution: " << currentPopulation.getNumberOfEdges() - globalBestInd.getFitness() << std::endl;
		std::cout << "------------------------------" << std::endl;
		std::cout << std::endl << "END OF EVA" << std::endl;
		std::cout << "------------------" << std::endl;
		// ------

		// Log
		logStream << std::endl << "------- RESULTS OF EVA -------" << std::endl;
		logStream << "Fitness of the best individual found: " << globalBestInd.getFitness() << std::endl;
		logStream << "The best partition found: ";
		globalBestInd.printInd(logStream);
		logStream << "Cost of the solution: " << currentPopulation.getNumberOfEdges() - globalBestInd.getFitness() << std::endl;
		logStream << "------------------------------" << std::endl;
		logStream << std::endl << "END OF EVA" << std::endl;
		logStream << "------------------" << std::endl;
		// ------

		double elapsed_secs = double(endTime - startTime) / CLOCKS_PER_SEC;
		std::cout << "Time taken by EVA: " << elapsed_secs << " s" << std::endl << std::endl;
		logStream << "Time taken by EVA: " << elapsed_secs << " s" << std::endl << std::endl;
	}

	void run(Population &pop) {
		
		size_t globalMaxFitness = pop.getNumberOfEdges(); // Set global max fitness
		globalBestInd = pop.getBestInd(); // Set global best individual

		size_t localMaxFitness = globalBestInd.getFitness();

		size_t generation = 0; // Count of generations
		printInfo(generation, pop, globalMaxFitness, localMaxFitness, std::cout); // Print information
		printInfo(generation++, pop, globalMaxFitness, localMaxFitness, logStream); // Log information

		Individual localBestInd = globalBestInd;

		if (opt != -1) numOfGen = -1; // If run until optimum

		stopFlag = false;

		while (!stopFlag) {

			pop = evolve(globalMaxFitness, pop); // Evolve generation

			// Best individual
			localBestInd = pop.getBestInd();
			localMaxFitness = localBestInd.getFitness();

			if (localMaxFitness > globalBestInd.getFitness()) globalBestInd = localBestInd; // Update global best
			// ---------------

			printInfo(generation, pop, globalMaxFitness, localMaxFitness, std::cout); // Print information
			printInfo(generation++, pop, globalMaxFitness, localMaxFitness, logStream); // Log information

			if ((opt != -1 && globalMaxFitness - localMaxFitness == opt) || localMaxFitness == globalMaxFitness || generation == numOfGen) stopFlag = true;
		}
	}

	// void printInfo(Population &pop, size_t globalMaxFitness, size_t localMaxFitness) - prints information about generation
	void printInfo(size_t generation, Population &pop, size_t globalMaxFitness, size_t localMaxFitness, std::ostream &outStream) {
		curTime = clock();
		outStream << "------------------------------" << std::endl;
		outStream << "Generation " << generation << ":" << std::endl;
		outStream << "-> Best fitness: " << localMaxFitness << std::endl;
		outStream << "-> The average fitness: " << pop.getAverageFitness() << std::endl;
		outStream << "-> Cost of best solution: " << globalMaxFitness - localMaxFitness << std::endl;
		double elapsed_secs = double(curTime - startTime) / CLOCKS_PER_SEC;
		outStream << "Time: " << elapsed_secs << " s" << std::endl;
		outStream << "------------------------------" << std::endl;
	}

	Population evolve(size_t globalMaxFitness, Population &pop) {

		std::cout << std::endl << "-> Growth of the population size: "; // Status

		nextGeneration.killAll();

		// Get best percentageOfBest % individuals to the next generation
		std::vector<Individual> best = pop.getBest(percentageOfBest);
		for (auto &x: best) {
			nextGeneration.addToPopulation(x);
			std::cout << ".";
		}
		// --------------------------------------------------------------

		while (nextGeneration.getPopulationSize() != sizeOfPopulation) {

			Individual parent_1; // First parent
			Individual parent_2; // Second parent

			// Selection
			parent_1 = selector->select(pop);
			do parent_2 = selector->select(pop);
			while (parent_1 == parent_2); // To make sure the parents are different
			// ---------

			Individual child_1 = parent_1; // Child 1
			Individual child_2 = parent_2; // Child 2

			// Crossover
			std::pair<Individual, Individual> children = crossover->XOver(pop, parent_1, parent_2); // Crossover
			child_1 = children.first;
			child_2 = children.second;
			// ---------

			// Mutation
			mutation->mutate(child_1);
			mutation->mutate(child_2);
			// --------

			// Add to new population
			if (killParents) { // Kill parents strategy
				nextGeneration.addToPopulation(child_1);
				nextGeneration.addToPopulation(child_2);
			}
			else {
				if (parent_1.getFitness() > child_1.getFitness()) nextGeneration.addToPopulation(parent_1);
				else nextGeneration.addToPopulation(child_1);
				if (parent_2.getFitness() > child_2.getFitness()) nextGeneration.addToPopulation(parent_2);
				else nextGeneration.addToPopulation(child_2);
			}
			// ---------------------

			std::cout << "..";
		}
		std::cout << std::endl;
		return nextGeneration;
	}

	// void init() - initial dialogue for setting parameters
	void init() {
		char answer;
		float probability;
		std::cout << std::endl << "----------- EVA PARAMETERS -----------" << std::endl;
		std::cout << "Repeat last parameters [y\\n]?: ";
		std::cin >> answer;
		switch(answer){
		case 'y':
			initFromFile("EVALastParams.txt", std::cout);
			system("pause");
			break;
		case 'n':
		{
			std::cout << "Read parameters from the file [y\\n]?: ";
			std::cin >> answer;
			switch (answer) {
			case 'y':
				initFromFile("SetEVA.txt", std::cout);
				system("pause");
				break;
			case 'n':
			{
				std::ofstream outStream("EVALastParams.txt");
				if (outStream.fail()) {
					std::cout << "Cannot open the file \"EVALastParams.txt\"" << std::endl;
					std::cout << std::endl;
					outStream.close(); // Close the output stream
					exit(0);
				}
				std::cout << "Type of selection (r - Roulette Wheel, t - Tournament): ";
				std::cin >> answer;
				outStream << answer;
				outStream << std::endl;
				switch (answer) {
				case 'r':
					selector = new RouletteWheelSelector();
					break;
				case 't':
					std::cout << "	Probability of the best individual to be selected (0.0 - 1.0): ";
					std::cin >> probability;
					outStream << probability;
					outStream << std::endl;
					if (probability < 0 || probability > 1) {
						outStream.close();
						error();
					}
					selector = new TournamentSelector(probability);
					break;
				default:
					error();
				}
				std::cout << "Probability of XOver (0.0 - 1.0): ";
				std::cin >> probability;
				outStream << probability;
				outStream << std::endl;
				crossover = new FairCrossover(probability);
				std::cout << "Probability of mutation (0.0 - 1.0): ";
				std::cin >> probability;
				outStream << probability;
				outStream << std::endl;
				if (probability < 0 || probability > 1) {
					outStream.close();
					error();
				}
				std::cout << "Type of mutation (r - random, u - uniform, s - shift, b - big bang): ";
				std::cin >> answer;
				outStream << answer;
				outStream << std::endl;
				switch (answer) {
				case 'r':
					mutation = new RandomMutation(probability);
					break;
				case 'u':
				{
					float bitFlipProb = 0.0;
					std::cout << "	Probability of bitflipping (0.0 - 1.0): ";
					std::cin >> bitFlipProb;
					outStream << bitFlipProb;
					outStream << std::endl;
					if (bitFlipProb < 0 || bitFlipProb > 1) {
						outStream.close();
						error();
					}
					mutation = new UniformMutation(probability, bitFlipProb);
					break;
				}
				case 's':
					mutation = new ShiftMutation(probability);
					break;
				case 'b':
					mutation = new BigBangMutation(probability);
					break;
				default:
					error();
				}
				std::cout << "Size of a population: ";
				std::cin >> sizeOfPopulation;
				outStream << sizeOfPopulation;
				outStream << std::endl;
				std::cout << "Run until optimum [y\\n]?: ";
				std::cin >> answer;
				outStream << answer;
				outStream << std::endl;
				switch (answer) {
				case 'y':
					std::cout << "	-> Optimum cost: ";
					std::cin >> opt;
					outStream << opt;
					outStream << std::endl;
					break;
				case 'n':
					std::cout << "Number of generations (-1 for infinity): ";
					std::cin >> numOfGen;
					outStream << numOfGen;
					outStream << std::endl;
					break;
				default:
					error();
				}
				std::cout << "Strategy (k - kill parents, b - only best)?: ";
				std::cin >> answer;
				outStream << answer;
				outStream << std::endl;
				switch (answer) {
				case 'k':
					killParents = true;
					break;
				case 'b':
					killParents = false;
					break;
				default:
					error();
				}
				std::cout << "Transfer the best individuals to the next generation [y\\n]?: ";
				std::cin >> answer;
				outStream << answer;
				outStream << std::endl;
				switch (answer) {
				case 'y':
					std::cout << "	-> Percentage of the best transfered individuals (0 - 100): ";
					std::cin >> percentageOfBest;
					outStream << percentageOfBest;
					outStream << std::endl;
					if (percentageOfBest < 0 || percentageOfBest > 100) {
						outStream.close();
						error();
					}
					break;
				case 'n':
					break;
				default:
					error();
				}
				outStream.close();
			}
			break;
			default:
				error();
			}
		}
		break;
		default:
			error();
		}
		std::cout << "--------------------------------------" << std::endl;
	}
	
	// void initFromFile() - sets ip parameters from the file "SetEVA.txt"
	void initFromFile(std::string fileName, std::ostream &outStream) {
		std::ifstream inStream(fileName); // Input stream
		if (inStream.fail()) {
			outStream << "Cannot open the file \"SetEVA.txt\"" << std::endl;
			outStream << std::endl;
			inStream.close(); // Close the input stream
			exit(0);
		}
		char answer;
		float probability;
		inStream >> answer;
		outStream << "Type of selection (r - Roulette Wheel, t - Tournament): " << answer << std::endl;
		switch (answer) {
		case 'r':
			selector = new RouletteWheelSelector();
			break;
		case 't':
			inStream >> probability;
			outStream << "	Probability of the best individual to be selected (0.0 - 1.0): " << probability << std::endl;
			if (probability < 0 || probability > 1) error();
			selector = new TournamentSelector(probability);
			break;
		default:
			error();
		}
		inStream >> probability;
		outStream << "Probability of XOver (0.0 - 1.0): " << probability << std::endl;
		crossover = new FairCrossover(probability);
		inStream >> probability;
		outStream << "Probability of mutation (0.0 - 1.0): " << probability << std::endl;
		if (probability < 0 || probability > 1) error();
		inStream >> answer;
		outStream << "Type of mutation (r - random, u - uniform, s - shift, b - big bang): " << answer << std::endl;
		switch (answer) {
		case 'r':
			mutation = new RandomMutation(probability);
			break;
		case 'u':
		{
			float bitFlipProb = 0.0;
			inStream >> bitFlipProb;
			outStream << "	Probability of bitflipping (0.0 - 1.0): " << bitFlipProb << std::endl;
			if (bitFlipProb < 0 || bitFlipProb > 1) error();
			mutation = new UniformMutation(probability, bitFlipProb);
			break;
		}
		case 's':
			mutation = new ShiftMutation(probability);
			break;
		case 'b':
			mutation = new BigBangMutation(probability);
			break;
		default:
			error();
		}
		inStream >> sizeOfPopulation;
		outStream << "Size of a population: " << sizeOfPopulation << std::endl;
		inStream >> answer;
		outStream << "Run until optimum [y\\n]?: " << answer << std::endl;
		switch (answer) {
		case 'y':
			inStream >> opt;
			outStream << "	-> Optimum cost: " << opt << std::endl;
			break;
		case 'n':
			inStream >> numOfGen;
			outStream << "Number of generations (-1 for infinity): " << numOfGen << std::endl;
			break;
		default:
			error();
		}
		inStream >> answer;
		outStream << "Strategy (k - kill parents, b - only best)?: " << answer << std::endl;
		switch (answer) {
		case 'k':
			killParents = true;
			break;
		case 'b':
			killParents = false;
			break;
		default:
			error();
		}
		inStream >> answer;
		outStream << "Transfer the best individuals to the next generation [y\\n]?: " << answer << std::endl;
		switch (answer) {
		case 'y':
			inStream >> percentageOfBest;
			outStream << "	-> Percentage of the best transfered individuals (0 - 100): " << percentageOfBest << std::endl;
			if (percentageOfBest < 0 || percentageOfBest > 100) error();
			break;
		case 'n':
			break;
		default:
			error();
		}
		inStream.close();
	}

};