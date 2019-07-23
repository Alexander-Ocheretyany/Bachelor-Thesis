/*
* Alexander Ocheretyany
* Class Writer for writing of generated graphs into a file
*
* Charles University in Prague, 2019
*/

#include "pch.h"

class Writer {

	std::vector<std::list<size_t>> graph; // Graph
	std::vector<size_t>VC; // Vertex cover
	std::vector<size_t>IS; // Independent set

public:

	Writer(std::vector<std::list<size_t>> graph, size_t numOfPartitions, std::vector<size_t> VC, std::vector<size_t> IS) {

		std::string prefix = "Generated_";
		std::string name;
		size_t index = 0;
		do {
			name = prefix;
			name.append(std::to_string(index++));
			name.append(".txt");
		} while (file_exists(name));

		std::ofstream outStream(name); // Output stream

		if (outStream.fail()) {
			std::cout << "Cannot open the file \"Generated____.txt\"" << std::endl;
			std::cout << std::endl;
			outStream.close(); // Close the output stream
			return;
		}

		outStream << numOfPartitions << std::endl; // Write number of partitions
		outStream << graph.size() << std::endl; // Read number of vertices
		
		for (size_t i = 0; i < graph.size(); ++i) {
			for (auto x : graph[i]) {
				outStream << i + 1 << " " << x + 1;
				for (auto y = graph[x].begin(); y != graph[x].end(); ++y) {
					if (*y == i) {
						graph[x].erase(y);
						break;
					}
				}
				outStream << std::endl;
			}
		}

		outStream.close(); // Close the output stream

		// Store the VC and the IS
		name.resize(name.size() - 4);
		name.append("_VC_IS.txt");
		std::ofstream outStream_2(name);
		for (auto x : VC) outStream_2 << x << " ";
		outStream_2 << std::endl;
		for (auto x : IS) outStream_2 << x << " ";
		outStream_2.close(); // Close the output stream
	}

	bool file_exists(std::string fileName)
	{
		std::ifstream infile(fileName);
		return infile.good();
	}
};