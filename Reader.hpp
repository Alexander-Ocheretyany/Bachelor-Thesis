/* 
* Alexander Ocheretyany
* Class Reader for input reading
*
* Charles University in Prague, 2018
*/

class Reader {

private:

	size_t numOfPartitions; // Number of partitions
	size_t numOfVertices = 0; // Number of vertices
	std::vector<std::list<size_t>> graph; // Graph

public:

	Reader() {
		numOfPartitions = 0; // Set number of partitions to zero
		numOfVertices = 0; // Set number of vertices to zero
		readInput(); // Read from the input file
	}

	~Reader() {
		graph.clear(); // Delete all lists
		graph.shrink_to_fit(); // Deallocate memory
	}

	// Returns the graph
	std::vector<std::list<size_t>> getGraph() const {
		return graph;
	}

	// Returns number of partitions
	size_t getNumOfPartitions() const {
		return numOfPartitions;
	}

private:

	// Reads an input
	void readInput() {

		std::ifstream inStream("Input.txt"); // Input stream

		if (inStream.fail()) {
			std::cout << "Cannot open the file \"Input.txt\"" << std::endl;
			std::cout << std::endl;
			inStream.close(); // Close the input stream
			return;
		}

		inStream >> numOfPartitions; // Read number of partitions
		inStream >> numOfVertices; // Read number of vertices

		graph.resize(numOfVertices); // Create numOfVertises lists, one list for each vertex

		int a = 0, b = 0; // Endpoints of an edge

		// Reading of edges
		while (inStream >> a >> b) {

			if (a < 0 || b < 0) {
				std::cout << "Wrong input format. Vertices must have positive indices." << std::endl; // There are negative indices in the input file
				std::exit(0); // Stop main program
			}

			// Add edges to the graph
			graph.at(a - 1).emplace_back(b - 1);
			graph.at(b - 1).emplace_back(a - 1);

		}

		inStream.close(); // Close the input stream
	}

};