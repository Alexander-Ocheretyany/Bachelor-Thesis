/*
* Alexander Ocheretyany
*
* Class Data for keeping all data needed for the program
*
* Charles University in Prague, 2018
*/

#include "boost/graph/adjacency_list.hpp"
#include "boost/graph/graphviz.hpp"

typedef boost::adjacency_list < boost::vecS, boost::vecS, boost::undirectedS,
								boost::property<boost::vertex_color_t, int>, // Colour for vertices
								boost::no_property > ColouredGraph;

class Data {

private:

	std::vector<std::list<size_t>> graph; // Main graph
	std::vector<size_t>VC; // Vertex cover
	std::vector<size_t>IS; // Independent set
	size_t numOfPartitions; // Number of partitions
	size_t ratio; // Maximum number of vertices in a single part
	ColouredGraph boostGraph; // Graph in Boost form
	bool correctGraph; // Flag of graph correctness

public:

	void init() {
		bool success = initialDialogue(); // Start initial dialogue
		if (success) {
			correctGraph = true; // Graph is correct
			setBoostGraph(); // Set the Boost graph
			ratio = getRatio(graph.size(), numOfPartitions); // Maximum number of vertices in a single part
		}
		else {
			correctGraph = false; // Graph is incorrect
		}
	}

	// std::vector<std::list<size_t>> getGraph() - returns the graph
	std::vector<std::list<size_t>> getGraph() const {
		return graph; // Return the graph
	}

	// std::vector<std::list<size_t>> getGraph() - returns the graph
	ColouredGraph getColouredGraph() const {
		return boostGraph; // Return the boost graph
	}

	// std::vector<size_t> getVC() - returns the VC
	std::vector<size_t> getVC() const {
		return VC; // Return VC
	}

	// std::vector<size_t> getIS() - returns the IS
	std::vector<size_t> getIS() const {
		return IS; // Return IS
	}

	// size_t getNumOfPartitions() - returns the number of partitions
	size_t getNumOfPartitions() const {
		return numOfPartitions; // Return the number of partitions
	}

	// size_t getRatio() - returns the maximum number of vertices in a single part of a partition
	size_t getRatio() const {
		return ratio; // Return the maximum number of vertices in a single part of a partition
	}

	// void toGraphvizFile(boost::adjacency_list<boost::vecS, boost::vecS, boost::undirectedS> G, char* nameOfFile) - save the graph into nameOfFile file
	void toGraphvizFile(char* nameOfFile) const {
		std::ofstream dotFile; // File stream
		dotFile.open(nameOfFile); // Open nameOfFile file for writing
		boost::write_graphviz(dotFile, boostGraph); // Write the graph into the file
		dotFile.close(); // Close the file
	}

	// bool isCorrect() - returns "true" if data is correct and "false" otherwise
	bool isCorrect() const {
		return correctGraph; // Return the flag of graph correctness
	}

	// Destructor
	~Data() {
		graph.clear(); // Delete graph
		graph.shrink_to_fit();
		VC.clear(); // Delete VC
		VC.shrink_to_fit();
		IS.clear(); // Delete IS
		IS.shrink_to_fit();
	}

private:

	// bool initialDialogue() - starts the initial dialogue and returns 'true' if success and 'false' otherwise
	bool initialDialogue() {

		std::cout << "To generate a graph type 'g' and press 'ENTER'" << std::endl;
		std::cout << "To read a graph from the input file type 'r' and press 'ENTER'" << std::endl;
		std::cout << std::endl; // Formatting
		std::cout << "Command: ";
		char choice;
		std::cin >> choice;
		std::cout << std::endl;
		switch (choice) {
		case 'r':
		{
			std::cout << "Reading of a graph from the file \"Input.txt\"...";
			Reader reader; // Reader
			graph = std::vector<std::list<size_t>>(reader.getGraph()); // Graph
			if (graph.size() == 0) { // If the program could not read the file
				system("pause");
				return(0); // End main program
			}
			numOfPartitions = reader.getNumOfPartitions(); // Number of partitions
			std::cout << " Done!" << std::endl;
			char subchoice;
			std::cout << std::endl << "-> Read VC and IS from a file [y\\n]? ";
			std::cin >> subchoice;
			std::cout << std::endl; // Formatting
			switch (subchoice) {
			case 'y':
			{
				std::cout << "Reading of VC and IS from the file \"Input_VC_IS.txt\"...";
				std::ifstream inStream("Input_VC_IS.txt"); // Input stream
				if (inStream.fail()) {
					std::cout << "Cannot open the file \"Input_VC_IS.txt\"" << std::endl;
					std::cout << std::endl;
					inStream.close(); // Close the input stream
					return false;
				}
				std::string str;
				std::string number;
				std::getline(inStream, str);
				for (size_t i = 0; i < str.size(); ++i) {
					while (i < str.size() && str[i] <= '9' && str[i] >= '0') {
						number += str[i++];
					}
					VC.push_back(std::stoi(number));
					number.clear();
				}
				std::getline(inStream, str);
				for (size_t i = 0; i < str.size(); ++i) {
					while (i < str.size() && str[i] <= '9' && str[i] >= '0') {
						number += str[i++];
					}
					IS.push_back(std::stoi(number));
					number.clear();
				}
				std::cout << " Done!" << std::endl;
			}
			break;
			case 'n':
			{
				std::cout << "Finding of the VC and the IS...";
				VertexCover cover(numOfPartitions, graph); // Generate vertex cover
				VC = cover.getVertexCover(); // Vertex cover
				IS = cover.getIndependentSet(); // Independent set
				std::cout << " Done!" << std::endl;
			}
			break;
			default:
			{
				std::cout << "Sorry, the program did not recognize the key" << std::endl;
				return false;
			}
			}

		}
		break;
		case'g':
		{
			std::cout << "GENERATION OF A GRAPH" << std::endl;
			std::cout << std::endl; // Empty line
			std::cout << "Number of vertices: ";
			size_t n = 0;
			std::cin >> n; // Read a number of vertices
			if (n == 0) {
				std::cout << std::endl << "The graph is empty." << std::endl;
				return (0);
			}
			std::cout << "Size of a vertex cover: ";
			size_t vcsize = 0;
			std::cin >> vcsize; // Read size of a VC
			if (n < vcsize) {
				std::cout << std::endl << "Wrong size of the vertex cover. The graph does not have so many vertices." << std::endl;
				return (0);
			}
			std::cout << "Probability of an edge (0.0 ... 1.0): ";
			double prob = 0;
			std::cin >> prob; // Read a probability of an edge
			if (prob < 0 || prob > 1) {
				std::cout << std::endl << "Wrong probability." << std::endl;
				return (0);
			}
			std::cout << "Number of partitions (> 0): ";
			std::cin >> numOfPartitions; // Read a number of partitions
			if (n < numOfPartitions) {
				std::cout << std::endl << "Cannot partite the graph since there are not enough vertices in the graph." << std::endl;
				return (0);
			}
			std::cout << std::endl;
			if (n < 0 || vcsize < 0 || prob < 0 || numOfPartitions < 0) {
				std::cout << std::endl << "Parameters cannot be negative." << std::endl;
				return (0);
			}
			Generator generatedGraph(n, n - vcsize, prob); // Generated graph
			graph = generatedGraph.getGraph(); // Graph
			VC = generatedGraph.getVC(); // VC
			IS = generatedGraph.getIS(); // IS
			Writer(graph, numOfPartitions, VC, IS); // Store to a file
		}
		break;
		default:
			std::cout << "Sorry, the program did not recognize the key" << std::endl;
			return false;
		}

		return true;
	}

	// size_t getRatio(size_t numOfVertices, size_t numOfPartitions) - calculates the maximum number of vertices in a single part of a partition
	size_t getRatio(const size_t numOfVertices, const size_t numOfPartitions) {
		double ratio = (double)numOfVertices / (double)numOfPartitions;
		return (size_t)(ceil(ratio)); // Celeing function of the ratio
	}

	// void setBoostGraph() - sets the graph for Boost library
	void setBoostGraph() {
		size_t* vertices = new size_t[graph.size()]; // Vertices of the graph
		for (size_t i = 0; i < graph.size(); i++) { // Create a list of vertices
			vertices[i] = boost::add_vertex(boostGraph);
		}
		for (size_t i = 0; i < graph.size(); i++) { // Add edges into the graph
			for (std::list<size_t>::iterator j = graph[i].begin(); j != graph[i].end(); j++) {
				if (!edge(vertices[i], vertices[*j], boostGraph).second) { // If the edge does not exist
					auto e = boost::add_edge(vertices[i], vertices[*j], boostGraph); // Add the edge to the graph
				}
			}
		}
	}
};