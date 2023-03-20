#include "word_ladder.h"
#include <unordered_set>
#include <cstring>
#include <fstream>
#include <iostream>
#include <map>
#include <queue>
#include <stdexcept>
#include <string>
#include <vector>
#include <tuple>

auto word_ladder::read_lexicon(const std::string &path) -> std::unordered_set<std::string> {
	auto lexicon_set = std::unordered_set<std::string>{};
	
	auto fstream = std::ifstream{path};
    if (!fstream) throw std::runtime_error("An error occurred while opening the file!"); 

	std::string temp_string;
	while (std::getline(fstream, temp_string)) {
		lexicon_set.insert(temp_string);
	}

	return lexicon_set;
}

namespace ladder_helpers {
	/*
		This is a helper function that takes in a current node, adds it to the word_neighbours 
		map with the current node as the key and all its neighbours (given the lexicon).
	*/
	auto get_neighbour_nodes(
		const std::string &current_node,
		const std::unordered_set<std::string> &lexicon,
		std::map<std::string, std::vector<std::string>> &word_neighbours
	) -> void {

		std::vector<std::string> current_neighbours;
		for (std::string::size_type i = 0; i < current_node.size(); i++) {
			auto string_alt = current_node;
			for (int j = 0; j < 26; j++) {
				string_alt.at(i) = static_cast<char>('a' + j);
				if (lexicon.contains(string_alt)) {
					current_neighbours.push_back(string_alt);
				}
			}
		}
		word_neighbours.insert({current_node,current_neighbours});
	}

	/*
		This is a helper function that takes in a lexicon as an unordered set and a source 
		word. With this, it returns a map<string, int> containing all words and their 
		distances from the source. 

		In addition, it takes in word_neighbours and, through repeated calls of get_neighbour_nodes,
		populates the adjacency list.

		this function will bfs over the lexicon:
			1. start from the source, add it to the queue, add it to the map as distance 0
			LOOP: 2. dequeue from the queue
			LOOP: 3. run function get_neighbour_nodes to get the nodes that neighbour the current_node
			LOOP: 4. add all those nodes to the map, and put them in as current_distance + 1
			LOOP: 5. add all neighbour nodes to the queue
			LOOP: Don't stop at the destination, continue until full traversal.

	*/
	auto get_word_distances(
		const std::string &source,
		const std::unordered_set<std::string> &lexicon,
		std::map<std::string, std::vector<std::string>> &word_neighbours
	) -> std::map<std::string, int> {

		// auto counter = lexicon.size();
		std::map<std::string, int> distance_map;
		std::queue<std::string> bfs_queue;
		std::unordered_set<std::string> visited;

		distance_map.insert({source, 0});
		bfs_queue.push(source);
		visited.insert(source);

		while (!bfs_queue.empty()) {
			
			const auto current_node = bfs_queue.front();
			const auto current_distance = distance_map.at(current_node);
			get_neighbour_nodes(current_node, lexicon, word_neighbours);
			for (std::string s : word_neighbours.at(current_node)) { // for all the neighbours of current
				if (std::find(visited.begin(), visited.end(), s) == visited.end()) { // if the neighbour hasn't been visited
					distance_map.insert({s, current_distance + 1}); // add it to the distance map
					visited.insert(s);
					bfs_queue.push(s); // add it to the queue
				}
			}
			bfs_queue.pop();
		}

		return distance_map;
	}

	/*
		This function is recursively called DFS from the destination to the source.
		It recursively calls itself over it's neighbours whose distance from the source
		is one less than the current node and accumulates and returns a std::vector<std::vector<std::string>>
		containing all the paths.
	*/
	auto dfs_get_paths(
		const std::string &source,
		const std::string &destination,
		const std::map<std::string, std::vector<std::string>> &word_neighbours,
		const std::map<std::string, int> &node_distances, 
		std::vector<std::string> working_path = {}
	) -> std::vector<std::vector<std::string>> { 
		
		std::vector<std::vector<std::string>> all_paths;

		working_path.push_back(destination);

		if (source == destination) {
			return std::vector<std::vector<std::string>>{{source}};
		} else {
			for (auto &neighbour : word_neighbours.at(destination)) {
				if (node_distances.at(neighbour) == node_distances.at(destination) - 1) {
					for (auto neighbour_path : dfs_get_paths(source, neighbour, word_neighbours, node_distances, working_path)) {
						neighbour_path.push_back(destination);
						all_paths.push_back(neighbour_path);
					}
				}
			}
		}

		return all_paths;
		
	}

	/*
		This function preprocesses the lexicon by removing all words that are a different
		length to the source word and returns a new lexicon. This is done to optimise
		the algorithm runtime.
	*/
	auto preprocess_lexicon(
		const std::unordered_set<std::string> &lexicon,
		const std::string &to
	) -> std::unordered_set<std::string> {
		std::unordered_set<std::string> new_lexicon;
		
		for (auto s : lexicon) {
			if (s.size() == to.size()) new_lexicon.emplace(s); 
		}

		return new_lexicon;
	}
}

/*
	This is the main generate function that takes in a source and destination word,
	alongside a lexicon in order to return a vector of all possible word ladders.

	It works by first tagging all words by their distance from the source node.
	Distance data will be stored in a map<string, int>
	Then, we will use a DFS from the destination node to the source node, with each
	step trying ALL nodes whose distance is 1 less than the current node's distance.
*/
auto word_ladder::generate(
	const std::string &from,
	const std::string &to,
	const std::unordered_set<std::string> &lexicon
) -> std::vector<std::vector<std::string>> { 

	// PROCESS THE LEXICON TO GET RID OF WORDS WHOSE LENTH DOES NOT CONFORM TO SOURCE
	auto new_lexicon = ladder_helpers::preprocess_lexicon(lexicon, from);

	// POPULATE word_neighbours (ADJACENCY LIST)
	// POPULATE node_distances 
	std::map<std::string, std::vector<std::string>> word_neighbours;
	const auto node_distances = ladder_helpers::get_word_distances(from, new_lexicon, word_neighbours);

	// IF THE DESTINATION WASN'T TRAVERSED TO IN THE ABOVE BFS, IT IS UNREACHABLE
	// SO RETURN NO SOLUTION
	if (!node_distances.contains(to)) return {}; 

	// DFS TO GET ALL PATHS
	auto all_paths = ladder_helpers::dfs_get_paths(from, to, word_neighbours, node_distances);

	// SORT PATHS ALPHABETICALLY
	std::sort(all_paths.begin(), all_paths.end(), std::less<std::vector<std::string>>());

	return all_paths;
}