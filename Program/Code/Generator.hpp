/*
* Alexander Ocheretyany
*
* Class for a graph generator
*
* Charles University in Prague, 2018
*/

class Generator {

	std::vector<std::list<size_t>> graph; // Graph
	std::vector<size_t> VC; // Vertex cover
	std::vector<size_t> IS; // Independent set
	size_t numOfVertices; // Number of vertices in the graph
	size_t sizeOfIS; // Size of the independent set
	size_t sizeOfVC; // SIze of the vertex cover
	double probability; // Probability of an edge presence

public:

	Generator(size_t n = 0, size_t is = 0, double prob = 0.0) {
		
		numOfVertices = n; // Set number of vertices
		sizeOfIS = is; // Set size of the independent set
		sizeOfVC = n - is; // Set size of the vertex cover
		probability = prob; // Probability of an edge presence
		graph.resize(n);
		// Vertex cover
		for (size_t i = 0; i < sizeOfVC; i++) {
			VC.push_back(i);
		}
		// Independent set
		for (size_t i = sizeOfVC; i < n; i++) {
			IS.push_back(i);
		}

		if (sizeOfVC != 0) {
			generate(); // Generate a graph
		}
	};

	// std::vector<std::list<size_t>> getGraph() - returns graph
	std::vector<std::list<size_t>> getGraph() const {
		return graph;
	}

	// std::vector<size_t> getVC() - returns vertex cover
	std::vector<size_t> getVC() const {
		return VC;
	}

	// std::vector<size_t> getIS() - returns independent set
	std::vector<size_t> getIS() const {
		return IS;
	}
 
private:

	void generate() {
		// Add edges to the vertex cover
		for (size_t i = 0; i < sizeOfVC - 1; i++) { // Go through all vertices of the VC
			for (size_t j = i + 1; j < sizeOfVC; j++) {
				if (rand() / float(RAND_MAX) <= probability) {
					graph[i].push_back(j);
					graph[j].push_back(i);
				}
			}
		}

		// Add some edges between VC and IS
		for (size_t i = sizeOfVC; i < numOfVertices; i++) { // Go through vertices of the IS
			for (size_t j = 0; j < sizeOfVC; j++) { // For all vertices of VC
				if (rand() / float(RAND_MAX) <= probability) {
					graph[i].push_back(j);
					graph[j].push_back(i);
				}
			}
		}
	}
};