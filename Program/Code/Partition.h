/*
* Alexander Ocheretyany
* Functions for partitioning
*
* Charles University in Prague, 2018
*/

class Partition {

private:

	int size; // Size of partition
	int* a; // Array of elements
	int* b; // Auxiliary array
	int m; // The last element of auxiliary array as a separate variable
	bool next; // Flag which says whether there is a new partition

public:	

	// Constructor
	Partition(size_t bins) {

		size = bins; // Store size

		// Define the array of elements and initialize it
		a = int[size];
		for (size_t i = 0; i < size; i++) {
			a[i] = 0;
		}

		// Define the auxiliary array and initialize it
		b = int[size - 1];
		for (size_t i = 0; i < size - 1; i++) {
			b[i] = 1;
		}

		int m = 1; // Initialize the last element of the auxiliary array
		next = 1; // Initialize the flag
	}
	
	// Destructor
	~Partition() {
		delete[] a; // Delete the array a
		delete[] b; // Delete the auxiliary array
	}

	int* getPartiton() {
		
		// Store the current partition before update
		int* partition = int[size];
		for (size_t i = 0; i < size; i++) {
			partition[i] = a[i];
		}

		bool hasNext = getNext(); // Get next partition

		return partition;
	}

	bool hasNext() const {
		return next;
	}

private:

	bool getNext() {

		if (a[size - 1] == m) {
			
			// Go to H4 of the algorithm
			int n = size - 1; // Define n for the algorithm (used for convenience)
			int j = n - 1; // Define j
			
			while (a[j] == b[j]) {
				j--;
				if (j == 1) break; // Terminate if j == 1
			}

			if (j == 1) {
				return 0; // The end
			}
			else {
				a[j]++; // Otherwise
			}

			m = b[j] + a[j];
			j++;

			while (j < n) {
				a[j] = 0;
				b[j] = m;
				j++;
			}

			a[n] = 0;

		}
		else {
			a[size - 1]++; // Increase the last element
		}

		return 1;

	}
};