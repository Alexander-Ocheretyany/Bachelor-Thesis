/*
* Alexander Ocheretyany
*
* Class Vertex Cover for generation of vertex covers
*
* Charles University in Prague, 2018
*/

class VertexCover {

private:
	
	size_t size; // Size of a cover
	std::vector<std::list<size_t>> graph; // Graph to be explored
	std::vector<size_t> VC; // Vertex cover
	std::vector<size_t> IS; // Independent set

public:

	VertexCover(size_t s, std::vector<std::list<size_t>> g) {
		size = s; // Size of the cover
		graph = g; // Graph to be explored
		VC = findVC(graph, 0, 1, VC); // Vertex cover
		findIS(); // Set independent set
	}

	~VertexCover() {
		graph.clear(); // Clear graph
		graph.shrink_to_fit();
		VC.clear(); // Clear VC
		VC.shrink_to_fit();
		IS.clear(); // Clear IS
		VC.shrink_to_fit();
	}

	// std::vector<size_t> getIndependentSet() - returns the independent set of the graph
	std::vector<size_t> getIndependentSet() const {
		return IS; 
	}

	// std::vector<size_t> getVertexCover() - returns the vertex cover of the graph
	std::vector<size_t> getVertexCover() const {
		return VC;
	}

private:

	// std::vector<size_t> findVC(std::vector<std::list<size_t>> aux, size_t deletedVertex, bool first, std::vector<size_t> accumulator) - finds a vertex cover
	std::vector<size_t> findVC(std::vector<std::list<size_t>> aux, size_t deletedVertex, bool first, std::vector<size_t> accumulator) {

		if (!first) { // If we are in the middle of recursion
			for (auto x : aux[deletedVertex]) { // For all neighbours of a vertex being deleted
				aux[x].remove(deletedVertex); // Delete all adjacent edges
			}
			aux[deletedVertex].clear(); // Delete the deletedVertex from the graph if it's not the beginning of recursion
		}

		bool edge = 0; // Flag of existance of an edge
		size_t v = 0; // Vertex with an adjacent edge

		for (auto x : aux) { // Go through all adjacency lists 
			if (!x.empty()) { // If there is an edge
				edge = 1; // Set up the flag of an edge existance to 'true'
				break; // Stop traversing the graph
			}
			else { // If no edge found
				edge = 0; // Set up the flag of an edge existance to 'false'
			}
			v++; // Take next vertex
		}

		std::vector<size_t> acc_1; // Left child
		std::vector<size_t> acc_2; // Right child

		if (edge) { // If there is an edge in the graph
			acc_1 = findVC(aux, v, 0, accumulator); // Left child
			acc_2 = findVC(aux, aux[v].front(), 0, accumulator); // Right child
		}
		else { // If there are no edges
			if (!first) accumulator.push_back(deletedVertex); // Add the vertex to the VC
			return accumulator; // Return the accumulator with current VC
		}
		
		// If there is an edge in the graph and we got results of recursion
		if (acc_2.size() == 0) { // If the left child returned a result quicker 
			for (auto x : acc_1) {
				accumulator.push_back(x);
			}
		}
		else if (acc_1.size() == 0) { // If the right child returned a result quicker 
			for (auto x : acc_2) {
				accumulator.push_back(x);
			}
		}
		else if (acc_1.size() <= acc_2.size()){ //If the number of vertices in the accumulator of the left child is smaller or the same
			for (auto x : acc_1) {
				accumulator.push_back(x);
			}
		}
		else { //If the number of vertices in the accumulator of the right child is smaller
			for (auto x : acc_2) { 
				accumulator.push_back(x);
			}
		}

		if(!first) accumulator.push_back(deletedVertex); // Add the vertex to the VC

		return accumulator;
	}

	// void findIS() - finds the independent set in a graph
	void findIS() {
		bool* tmp = new bool[graph.size()];
		for (size_t i = 0; i < graph.size(); i++) {
			tmp[i] = 0;
		}
		for (auto x : VC) {
			tmp[x] = 1;
		}
		for (size_t i = 0; i < graph.size(); i++) {
			if (tmp[i] == 0) IS.push_back(i);
		}
		delete[] tmp;
	}
};