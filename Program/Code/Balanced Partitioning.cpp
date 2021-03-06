/*
* Bachelor thesis of Alexander Ocheretyany
*
* Charles University in Prague, 2019
*/

#include "pch.h"
#include "windows.h"

Data mainData; // Data object with all the data needed

void printHeader();
void printGraphSummary();
void startMainDialogue();

int main()
{
	// Adjust window
	HWND console = GetConsoleWindow();
	RECT ConsoleRect;
	GetWindowRect(console, &ConsoleRect);
	MoveWindow(console, ConsoleRect.left, ConsoleRect.top, 500, 500, TRUE);
	// -------------

	srand(time(NULL)); // Random seed
	printHeader(); // Header of the program
	mainData.init(); // Get all the data needed
	if (!mainData.isCorrect()) return 1; // Return "error" if data is incorrect

	printGraphSummary();
	startMainDialogue();

	system("pause");
	return 0;
}

void printHeader() {
	std::cout << "---------------------------------------------------------------" << std::endl;
	std::cout << "--------- BACHELOR THESIS OF ALEXANDER OCHERETYANYY -----------" << std::endl;
	std::cout << "--- Balanced Partitioning of Graphs with Small Vertex Cover ---" << std::endl;
	std::cout << "---------------------------------------------------------------" << std::endl;
	std::cout << "------------- Charles University in Prague, 2019 --------------" << std::endl;
	std::cout << "---------------------------------------------------------------" << std::endl;
	std::cout << std::endl;
}

void printGraphSummary() {
	std::cout << std::endl << "------------- INPUT DATA -------------" << std::endl;
	std::cout << "Number of vertices: " << mainData.getGraph().size() << std::endl;
	size_t num = 0; // Initial value
	std::vector<std::vector<bool>> table;
	table.resize(mainData.getGraph().size());
	for (auto x : table) {
		x.resize(mainData.getGraph().size());
	}
	for (auto x : mainData.getGraph()) {
		num += x.size();
	}
	num /= 2;
	std::cout << "Number of edges: " << num << std::endl;
	std::cout << "Size of the vertex cover: " << mainData.getVC().size() << std::endl;
	std::cout << "Number of partitions: " << mainData.getNumOfPartitions() << std::endl;
	std::cout << "--------------------------------------" << std::endl;
}

void startMainDialogue() {

	std::cout << std::endl << "-> Start the evolutionary algoritm [y\\n]? ";
	char canswer;
	std::cin >> canswer;
	switch (canswer) {
	case 'y':
	{
		Evolution eva(mainData);
		eva.startEvolution();
	}
	break;
	case 'n':
		break;
	default:
		Data::error("Wrong input data!");
	}

	std::cout << "-> Start the FPT algorithm [y\\n]?: ";
	std::cin >> canswer;
	switch (canswer) {
	case 'y':
	{
		FPT fpt(mainData);
		fpt.startFPT();
	}
	break;
	case 'n':
		break;
	default:
		Data::error("Wrong input data!");
	}
	std::cout << std::endl;

}