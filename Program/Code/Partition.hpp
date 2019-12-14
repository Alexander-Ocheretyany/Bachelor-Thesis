/*
* Alexander Ocheretyany
*
* Functions for partitioning
*
* Charles University in Prague, 2018
*/

class Partition {

private:

	size_t size; // Size of partition
	std::vector<size_t> a; // Array of elements
	std::vector<size_t> b; // Auxiliary array
	size_t m; // The last element of auxiliary array as a separate variable
	bool next; // Flag which says whether there is a new partition
	size_t maximal; // Maximal number of partitions

public:	

	// Constructor for generator of restricted growth strings in lexicographic order
	Partition(size_t elements) {

		maximal = -1; // If we want to generate restricted growth strings in lexicographic order
		size = elements; // Store size

		// Define the array of elements and initialize it
		a.resize(size);
		for (size_t i = 0; i < size; i++) {
			a[i] = 0;
		}

		// Define the auxiliary array and initialize it
		b.resize(size - 1);
		for (size_t i = 0; i < size - 1; i++) {
			b[i] = 1;
		}

		m = 1; // Initialize the last element of the auxiliary array
		next = 1; // Initialize the flag
	}

	// Constructor for a general generator where max is the maximal number of partitions
	Partition(size_t elements, size_t max) {

		maximal = max - 1; // Maximal number of partitions
		size = elements; // Store size

		// Define the array of elements and initialize it
		a.resize(size);
		for (size_t i = 0; i < size; i++) {
			a[i] = 0;
		}

		next = 1; // Initialize the flag
	}
	
	// Destructor
	~Partition() {
		a.clear(); // Delete the array a
		a.shrink_to_fit();
		b.clear(); // Delete the auxiliary array
		b.shrink_to_fit();
	}

	// std::vector<size_t> getPartiton() - returns a partition
	std::vector<size_t> getPartiton() {
		
		// Store the current partition before update
		std::vector<size_t> partition(size);
		for (size_t i = 0; i < size; i++) {
			partition[i] = a[i];
		}

		next = getNext(); // Get next partition

		return partition;
	}

	// bool hasNext() - returns 'true' if there is next partition
	bool hasNext() const {
		return next;
	}

	// void reset() - resets the partition
	void reset() {
		for (size_t i = 0; i < size; i++) {
			a[i] = 0;
		}
		next = 1;
	}

private:

	// bool getNext() - generates next partition
	bool getNext() {

		if (maximal == static_cast<size_t>(-1)) {
			
			int n = size - 1; // Define n for the algorithm (used for convenience)

			if (a[n] == m) {
				// Go to H4 of the algorithm
				int j = n - 1; // Define j

				while (a[j] == b[j]) {
					j--;
				}

				if (j == 0) {
					return 0; // The end
				}
				else {
					a[j]++; // Otherwise
				}

				if (a[j] == b[j]) m = b[j] + 1;
				else m = b[j];
				j++;

				while (j < n) {
					a[j] = 0;
					b[j] = m;
					j++;
				}

				a[n] = 0;
				return 1;
			}
			else {
				a[n]++; // Increase the last element
				if(b.size() != 0) return 1; // If several digits in a string
				else return 0; // If only 1 digit in a string
			}
		}
		else {

			// Generate all
			size_t i = size - 1; // Last element

			// Find the last element we can increase
			while (a[i] == maximal) {
				if (--i == -1) return 0;
			}

			a[i]++; // Increase the element

			// Reset everything to the right of the element
			while (++i <= size - 1) {
				a[i] = 0;
			}

			return 1;
		}
	}
};