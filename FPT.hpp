/*
* Alexander Ocheretyany
*
* Class for the FPT algorithm
*
* Charles University in Prague, 2019
*/

#include "pch.h"
#include <ilcplex/cplex.h>
#include "boost/graph/successive_shortest_path_nonnegative_weights.hpp"

class FPT {

	Data mainData;
	std::vector<std::list<size_t>> graph; // Main graph
	size_t numOfPartitions; // Number of partitions
	size_t ratio; // Maximum number of vertices in a single part
	std::vector<size_t> VC; // Vertex cover
	std::vector<size_t> IS; // Independent set

	typedef boost::adjacency_list_traits<boost::vecS, boost::vecS, boost::directedS> Traits;
	typedef boost::adjacency_list < boost::vecS, boost::vecS, boost::directedS,
		boost::no_property, // No properties for vertices
		boost::property<boost::edge_capacity_t, int, // Edge capacity
		boost::property<boost::edge_residual_capacity_t, int, // Edge residual capacity
		boost::property<boost::edge_reverse_t, Traits::edge_descriptor, // Reverse edge
		boost::property<boost::edge_weight_t, int > > > >  // Edge weight
	> FlowGraph;

	template <typename Map, typename Edge>
	void fill_capacity(Map &m, Edge e, Edge er, int c) {
		put(m, e, c);
		put(m, er, 0);
	}

	template <typename Map, typename Edge> void fill_rev(Map &m, Edge e, Edge er) {
		put(m, e, er);
		put(m, er, e);
	}

	template <typename Map, typename Edge>
	void fill_weight(Map &m, Edge e, Edge er, int w) {
		put(m, e, w);
		put(m, er, -w);
	}

public:

	FPT(Data &mainData) {
		this->mainData = mainData;
		graph = mainData.getGraph();
		numOfPartitions = mainData.getNumOfPartitions();
		ratio = mainData.getRatio();
		VC = mainData.getVC();
		IS = mainData.getIS();
	}

	void error(std::string msg) {
		std::cout << msg << std::endl;
		system("pause");
		exit(1);
	}

	int startFPT() {

		std::cout << std::endl << "---- FPT ALGORITHM IS WORKING ----" << std::endl;

		bool LP = false; // Flag for LP usage
		char answer;
		std::cout << std::endl << "Would you like to use LP [y\\n]? ";
		std::cin >> answer;
		std::cout << std::endl;
		if (answer == 'y') LP = true;

		std::ofstream outStream("LogFPT.txt");
		if (outStream.fail()) {
			std::cout << "Cannot open \"LogFPT.txt\" file!" << std::endl;
			exit(1);
		}
		
		clock_t begin = clock();

		if (numOfPartitions == graph.size()) {
			std::cout << "You are separating all vertices, thus the minimum cost is equal to the number of edges in the graph" << std::endl;
			return 0;
		}

		if (VC.size() == 0) { // Graph does not have edges
			// Print any balanced partition of the IS
			Partition partition(IS.size()); // Partite the IS
			while (partition.hasNext()) {
				std::vector<size_t> p = partition.getPartiton();
				if (check(p, IS.size(), numOfPartitions, ratio)) { // Get a feasible partition
					std::cout << "Partition is:\n" << std::endl;
					for (size_t i = 0; i < numOfPartitions; i++) {
						std::cout << "Part " << i << ": ";
						for (size_t j = 0; j < IS.size(); j++) {
							if (p[j] == i) std::cout << IS[j] << " ";
						}
						std::cout << std::endl;
					}
					system("pause");
					return 0;
				}
			}
		}

		Partition partition(VC.size()); // Strings for VC
		Partition ISpartition(IS.size(), numOfPartitions); // Strings for IS

		std::vector<size_t> parts(graph.size()); // Map of parts which says which part of the current partition each vertex of the VC belongs to
		size_t bestCost = -1; // Max value of size_t (infinity)
		std::vector<size_t> bestPartition; // Here the best partition will be stored

		while (partition.hasNext()) { // Go through all correct partitions of VC

			std::vector<size_t> p = partition.getPartiton(); // Get a new partition of the VC

			std::cout << "Current partition: ";
			for (auto x : p) std::cout << x << " ";
			std::cout << std::endl;

			if (check(p, VC.size(), numOfPartitions, ratio)) { // If the new partition is correct

				std::cout << std::endl;
				std::cout << "Current partition: ";
				for (size_t i = 0; i < VC.size(); ++i) {
					std::cout << p[i] << " ";
				}
				std::cout << std::endl;

				// Fill in the map of parts
				for (size_t i = 0; i < VC.size(); i++) {
					parts[VC[i]] = p[i];
				}
				// ------------------------

				// Now we know the whole partition and sizes of all parts
				// We now can compute number of free slots for each partition and
				// build an auxiliary bipartite graph
				// To do that we need to define costs

				// Now we are ready to define costs for each vertex
				std::vector<std::vector<size_t>> costs(IS.size()); // Vector of costs
				for (size_t i = 0; i < IS.size(); i++) {
					costs[i].resize(numOfPartitions); // Each vector contains the same number of elements == number of parts
				}
				for (size_t i = 0; i < IS.size(); i++) {
					for (size_t j = 0; j < numOfPartitions; j++) {
						size_t cost = 0; // Cost
						// For every neighbour of the vertex
						for (auto x : graph[IS[i]]) {
							if (parts[x] != j) cost++; // If the neighbour is in a different part of the partition we increase the cost
						}
						costs[i].at(j) = cost; // Assign cost to the part
					}
				}
				// ------------------------------------------------ //

				// Count number of free slots for each part
				std::vector<size_t> freeSlots(numOfPartitions); // Vacant slots in each part of the partition
				for (size_t i = 0; i < numOfPartitions; i++) {
					size_t counter = 0;
					for (auto x : VC) {
						if (parts[x] == i) counter++;
					}
					freeSlots[i] = ratio - counter;
				}
				// ------------------------------------- //

				// Now, in our bipartite graph one part consists of the vertices of the IS
				// and the second part contains only free slots of our partition. We want to
				// build a complete bipartitie graph on these vertises because each vertex of 
				// the IS can potentially lie in any free slot.

				if (!LP) {

					// Count number of edges in the bipartite graph
					size_t ne = 0; // Number of edges
					size_t nv = 0; // Number of vertices
					for (size_t i = 0; i < freeSlots.size(); i++) {
						ne += freeSlots[i];
					}
					nv = ne + IS.size(); // Store number of free slots + size of the IS
					ne *= IS.size(); // Calculate number of edges
					// ---------------------------------------- //

					// Preprocessing - create an array of partitions
					std::vector<size_t> indexOfPartition(nv - IS.size());
					size_t currentPart = 0;
					for (size_t i = 0; i < indexOfPartition.capacity(); i++) {
						for (size_t j = 0; j < freeSlots[currentPart]; j++) {
							indexOfPartition[i++] = currentPart;
						}
						i -= 1;
						currentPart++;
					}
					// Array "indexOfPartition" contains indices of parts which each free slot belongs to
					// so we can easily identify which part of the partition each vertex of IS goes to.
					// ----------------------------------------- //

					// Create a bipartite graph for BOOST	
					FlowGraph bipartiteGraph(nv + 2); // +2 vertices for source and sink

					boost::property_map<FlowGraph, boost::edge_capacity_t>::type capacity = boost::get(boost::edge_capacity, bipartiteGraph);
					boost::property_map<FlowGraph, boost::edge_reverse_t>::type	rev = boost::get(boost::edge_reverse, bipartiteGraph);
					boost::property_map<FlowGraph, boost::edge_weight_t>::type weight = boost::get(boost::edge_weight, bipartiteGraph);
					boost::property_map<FlowGraph, boost::edge_residual_capacity_t>::type residual_capacity = boost::get(boost::edge_residual_capacity, bipartiteGraph);

					Traits::edge_descriptor e, e_rev; // Edges

					// Add source and its edges
					for (size_t i = 1; i <= IS.size(); i++) {

						e = boost::add_edge(0, i, bipartiteGraph).first; // Forward edge
						e_rev = boost::add_edge(i, 0, bipartiteGraph).first; // Back edge

						fill_weight(weight, e, e_rev, 1); // Weights
						fill_capacity(capacity, e, e_rev, 1); // Capacities
						fill_rev(rev, e, e_rev); // Reverse edges

					}

					// Add inner edges
					for (size_t i = 1; i <= IS.size(); i++) {

						for (size_t j = IS.size() + 1; j <= nv; j++) {

							e = boost::add_edge(i, j, bipartiteGraph).first; // Forward edge
							e_rev = boost::add_edge(j, i, bipartiteGraph).first; // Back edge

							fill_weight(weight, e, e_rev, costs[i - 1].at(indexOfPartition.at(j - IS.size() - 1))); // Weights
							fill_capacity(capacity, e, e_rev, 1); // Capacities
							fill_rev(rev, e, e_rev); // Reverse edges

						}

					}

					// Add sink and its edges
					for (size_t i = IS.size() + 1; i <= nv; i++) {

						e = boost::add_edge(i, nv + 1, bipartiteGraph).first; // Forward edge
						e_rev = boost::add_edge(nv + 1, i, bipartiteGraph).first; // Back edge

						fill_weight(weight, e, e_rev, 1); // Weights
						fill_capacity(capacity, e, e_rev, 1); // Capacities
						fill_rev(rev, e, e_rev); // Reverse edges

					}

					std::cout << "Max flow calculation...";

					boost::successive_shortest_path_nonnegative_weights(bipartiteGraph, 0, nv + 1); // Find min cost max flow

					std::cout << " Done!" << std::endl;

					// Now residual_capacity contains information about min cost max matching.
					// We will find '0' entrances.

					std::vector<size_t> match(IS.size()); // List of matches

					for (size_t i = 1; i <= IS.size(); i++) {
						for (size_t j = IS.size() + 1; j <= nv; j++) {
							Traits::edge_descriptor e3 = (boost::edge(i, j, bipartiteGraph)).first;
							if (residual_capacity[e3] == 0) {
								match[i - 1] = j - 1; // Add matching vertex
								break;
							}
						}
					}

					// Now we can build the whole map for the partition
					for (size_t i = 0; i < IS.size(); ++i) {
						parts[IS[i]] = indexOfPartition[match[i] - IS.size()];
					}

					// Check whether we have exact number of partitions
					bool isIncorrect = true;
					for (size_t i = 0; i < mainData.getNumOfPartitions(); ++i) {
						isIncorrect = true;
						for (size_t j = 0; j < parts.size(); ++j) {
							if (i == parts[j]) {
								isIncorrect = false;
								break;
							}
						}
						if (isIncorrect) break;
					}

					// Fianaly, we can count the cost of the current partition
					size_t totalCost = calculateCost(graph, parts);

					if (totalCost < bestCost && !isIncorrect) {
						bestCost = totalCost;
						bestPartition.clear(); // CLear current favorite
						bestPartition = parts; // Store as the best partititon

						std::cout << "Best partition has been updated to best cost = " << bestCost << std::endl;
					}
				}
				else {

					size_t sizeOfRHS = 0; // LHS has the same size
					for (auto x : freeSlots) {
						sizeOfRHS += x;
					}

					// Container for the matrix of weights
					// Row indices represent LHS (IS) while columnd indices represent RHS (free slots) 
					std::vector<std::vector<double>> weights;
					weights.resize(sizeOfRHS);
					for (size_t i = 0; i < weights.size(); ++i) {
						weights[i] = std::vector<double>();
						weights[i].resize(sizeOfRHS);
					}
					// -----------------

					// Preprocessing - create an array of partitions
					std::vector<size_t> indexOfPartition(sizeOfRHS);
					size_t currentPart = 0;
					for (size_t i = 0; i < sizeOfRHS; i++) {
						for (size_t j = 0; j < freeSlots[currentPart]; j++) {
							indexOfPartition[i++] = currentPart;
						}
						i -= 1;
						currentPart++;
					}
					// Array "indexOfPartition" contains indices of parts which each free slot belongs to
					// so we can easily identify which part of the partition each vertex of IS goes to.
					// ----------------------------------------- //

					// Now we can build the matrix of weights
					for (size_t i = 0; i < IS.size(); ++i) {
						for (size_t j = 0; j < sizeOfRHS; ++j) {
							weights[i][j] = costs[i][indexOfPartition[j]];
						}
					}

					for (size_t i = IS.size(); i < sizeOfRHS; ++i){
						for (size_t j = 0; j < sizeOfRHS; ++j) {
							weights[i][j] = 1;
						}
					}
					// --------------------------------------

					std::vector<double> result = solveLP(weights); // Solve the minimum cost perfect matching LP
					
					size_t index = 0;
					std::vector<size_t> match(IS.size()); // List of matches
					for (size_t i = 0; i < IS.size() * sizeOfRHS; i++) {
						if (result[i] == 1.0) {
							match[index++] = i % sizeOfRHS; // Add matching vertex
						}
					}

					// Now we can build the whole map for the partition
					for (size_t i = 0; i < IS.size(); ++i) {
						parts[IS[i]] = indexOfPartition[match[i]];
					}

					// Fianaly, we can count the cost of the current partition
					std::cout << "LP solving...";

					size_t totalCost = calculateCost(graph, parts);

					std::cout << " Done!" << std::endl;

					if (totalCost < bestCost) {

						bestCost = totalCost;
						bestPartition.clear(); // CLear current favorite
						bestPartition = parts; // Store as the best partititon

						std::cout << "Best partition has been updated to best cost = " << bestCost << std::endl;
					}
					
				}
			}
		}

		clock_t end = clock();
		double elapsed_secs = double(end - begin) / CLOCKS_PER_SEC;

		std::cout << std::endl;
		std::cout << "The best partition with the cost " << bestCost << " is:" << std::endl << std::endl;
		for (size_t i = 0; i < bestPartition.size(); ++i) {
			std::cout << i << "->" << bestPartition[i] << std::endl;
		}

		std::cout << std::endl << "Writing the solution into the file \"Graph.dot\"... ";
		toGraphVizFileColoured(mainData.getColouredGraph(), bestPartition, (char*) "Graph.dot");
		std::cout << " Done!" << std::endl;

		std::cout << std::endl << "END OF THE FPT ALGORITHM" << std::endl;
		std::cout << "--------------------------" << std::endl;

		std::cout << "Time taken by the basic algorithm: " << elapsed_secs << " s" << std::endl << std::endl;

		// Log
		outStream << "OPT = " << bestCost << std::endl;
		outStream << "Time: " << elapsed_secs << " s" << std::endl;
		outStream.close();
		return 0;
	}

	// size_t calculateCost(std::vector<std::list<size_t>> graph, std::vector<size_t> partition) - calculates the total cost of the partition
	size_t calculateCost(std::vector<std::list<size_t>> graph, std::vector<size_t> partition) {

		size_t totalCost = 0; // Total cost of the partition

		for (size_t i = 0; i < graph.size(); i++) {

			size_t intermediateCost = 0; // Cost for the current edge
			size_t currentPart = partition[i];

			for (auto x : graph[i]) {
				if (currentPart != partition[x]) intermediateCost++;
				graph[x].remove(i); // Remove edge
			}

			totalCost += intermediateCost;

		}

		return totalCost;
	}

	bool check(std::vector<size_t> p, size_t numOfVertices, size_t numOfPartitions, size_t ratio) {

		// First check of correctness - number of partitions
		size_t max = 0; // Max number of partitions
		for (size_t i = 0; i < numOfVertices; i++) {
			if (p[i] > max) max = p[i];
		}
		if (max + 1 > numOfPartitions) return 0; // Return "false" if we overpartitioned the graph

		// Second check of correctness - size of each part
		size_t* auxArray = new size_t[numOfVertices]; // Auxiliary array

		// Initialize the array with 0s
		for (size_t i = 0; i < numOfVertices; i++) {
			auxArray[i] = 0;
		}

		// Count number of elements in each partition
		for (size_t i = 0; i < numOfVertices; i++) {
			auxArray[p[i]]++;
			if (auxArray[p[i]] > ratio) {
				delete[] auxArray; // Delete the auxiliary array
				return 0; // Return "false"
			}
		}

		delete[] auxArray; // Delete the auxiliary array

		return 1; // Return "true"
	}

	template <class ColourMap>
	class vertex_colour_writer {
	public:
		vertex_colour_writer(ColourMap cmap) : map(cmap) {}

		template <class Vertex>
		void operator()(std::ostream& out, const Vertex& v) const {

			if (map[v] < 15) {
				out << "[penwidth=3, color=\"" << colours[map[v]] << "\"]";
			}
			else {
				out << "[style = dashed, penwidth=1, color=\"" << (char*)"black" << "\"]"; // Default colour, dashed style
			}
		}
	private:
		ColourMap map;
		char* colours[15] = { (char*)"red", (char*)"yellow", (char*)"blue", (char*)"green", (char*)"magenta", (char*)"indigo", (char*)"brown", (char*)"aquamarine",  (char*)"firebrick1", (char*)"darkslateblue", (char*)"goldenrod", (char*)"greenyellow", (char*)"orange", (char*)"violet", (char*)"olivedrab1" };
	};

	template < class ColourMap >
	vertex_colour_writer<ColourMap>
		make_vertex_colour_writer(ColourMap n) {
		return vertex_colour_writer<ColourMap>(n);
	}

	void toGraphVizFileColoured(ColouredGraph graph, std::vector<size_t> partition, char* nameOfFile) {

		boost::property_map<ColouredGraph, boost::vertex_color_t>::type colour = boost::get(boost::vertex_color, graph); // Colour map of the graph

		for (size_t i = 0; i < partition.size(); ++i) { // Colour vertices of the graph
			boost::put(colour, i, partition[i]);
		}

		std::ofstream file;
		file.open(nameOfFile);

		boost::write_graphviz(file, graph, make_vertex_colour_writer(colour));
	}

	// This code is built using an example on the IBM website, exploiting C-style
	std::vector<double> solveLP(std::vector<std::vector<double>> &weights)
	{

		int       status = 0;
		CPXENVptr env = NULL;
		CPXLPptr  lp = NULL;

		size_t SIZE = weights.size();
		size_t NUMROWS = 2 * SIZE;
		size_t NUMCOLS = SIZE * SIZE;
		size_t NUMNZ = NUMCOLS * NUMROWS;
		size_t index = 0; // Index for initialization of variable's indices

		double   *obj = new double[NUMCOLS];
		double   *lb = new double[NUMCOLS];
		double   *ub = new double[NUMCOLS];

		double   *x = new double[NUMCOLS]; // Solution
		int      *rmatbeg = new int[NUMROWS];
		int      *rmatind = new int[NUMNZ];
		double   *rmatval = new double[NUMNZ];
		double   *rhs = new double[NUMROWS];
		char     *sense = new char[NUMROWS];

		int      solstat; // Status of the solution
		double   objval; // Objective value of the solution

		std::vector<double> result(NUMCOLS);

		env = CPXopenCPLEX(&status); // CPLEX environment
		if (!env) {
			error("Could not open CPLEX environment.\n");
		}
		lp = CPXcreateprob(env, &status, "lpex1");
		if (!lp) {
			error("Failed to create LP");
		}

		CPXchgobjsen(env, lp, CPX_MIN);

		for (size_t i = 0; i < NUMCOLS; ++i) obj[i] = weights[i / SIZE][i % SIZE]; // Objective function
		for (size_t i = 0; i < NUMCOLS; ++i) lb[i] = 0.0; // Left-hand side boundaries
		for (size_t i = 0; i < NUMCOLS; ++i) ub[i] = 1.0; // Right-hand side boundaries

		status = CPXnewcols(env, lp, NUMCOLS, obj, lb, ub, NULL, NULL);
		if (status) {
			error("Failed to populate problem");
		}

		for (size_t i = 0; i < NUMROWS; ++i) rmatbeg[i] = i * NUMCOLS; // First indices in rows
		for (size_t i = 0; i < NUMNZ; ++i) rmatind[i] = i % NUMCOLS; // Column inices
		for (size_t i = 0; i < NUMROWS; ++i) sense[i] = 'E'; // Equality
		for (size_t i = 0; i < NUMROWS; ++i) rhs[i] = 1.0; // Right-hand side

		// First part of the matrix
		for (size_t i = 0; i < SIZE; ++i) {
			for (size_t j = 0; j < NUMCOLS; ++j) {
				if (j >= i * SIZE && j < (i + 1) * SIZE) rmatval[index++] = 1.0;
				else rmatval[index++] = 0.0;
			}
		}

		// Second part of the matrix
		for (size_t i = 0; i < SIZE; ++i) {
			for (size_t j = 0; j < NUMCOLS; ++j) {
				if (j % SIZE == i) rmatval[index++] = 1.0;
				else rmatval[index++] = 0.0;
			}
		}

		status = CPXaddrows(env, lp, 0, NUMROWS, NUMNZ, rhs, sense, rmatbeg,
			rmatind, rmatval, NULL, NULL);
		if (status) {
			error("Failed to populate problem");
		}

		status = CPXlpopt(env, lp);
		if (status) {
			error("Failed to optimize LP");
		}

		status = CPXsolution(env, lp, &solstat, &objval, x, NULL, NULL, NULL);
		if (status) {
			error("Failed to obtain solution");
		}

		for (size_t i = 0; i < NUMCOLS; ++i) result[i] = x[i];

		// Clean up
		delete[] obj;
		delete[] lb;
		delete[] ub;
		delete[] x;
		delete[] rmatbeg;
		delete[] rmatind;
		delete[] rmatval;
		delete[] rhs;
		delete[] sense;
		CPXfreeprob(env, &lp);
		CPXcloseCPLEX(&env);
		// --------

		return result;

	}
};
