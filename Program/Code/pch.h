/*
* Alexander Ocheretyany
*
* Charles University in Prague, 2018
*/

#ifndef PCH_H
#define PCH_H

#include <iostream>
#include <string>
#include <fstream> // Works with file streams
#include <vector>
#include <list>
#include <iostream>
#include <utility>
#include <algorithm>
#include <deque>
#include <iterator>
#include <time.h> // For random generator

#include "Generator.hpp" // Graph generation
#include "Writer.hpp" // Writing into a file functions
#include "Reader.hpp" // Reading functions
#include "Partition.hpp" // Partition functions
#include "VertexCover.hpp" // Vertex covers
#include "Data.hpp"
#include "Evolution.hpp" // Evolutionary algorithm
#include "FPT.hpp" // FPT algorithm

#include "boost/graph/graphviz.hpp"
#include "boost/variant/variant.hpp"
#include "boost/graph/graph_traits.hpp"
#include "boost/graph/adjacency_list.hpp"
#include "boost/graph/successive_shortest_path_nonnegative_weights.hpp"
#include "boost/property_map/property_map.hpp"
#include "boost/tuple/tuple.hpp"

#endif //PCH_H