#include "oriented_graph.hpp"

#include <iostream>
#include <algorithm>


class PathFinder
{
public:

	int find_minimal_path(
		const oriented_graph<int>& graph,
		const int from,
		const int to
	)
	{
		std::unordered_set<int> visited;
		std::unordered_map<int, int> path;

		int current_vertex = from;

		visited.insert(current_vertex);
		path[current_vertex] = 0;

		do
		{
			for (auto it = graph.begin_edge(current_vertex); !it.is_end(); it.next())
			{
				int next_vertex = it.value();

				if (path.find(next_vertex) != path.end())
				{
					path[next_vertex] = std::min(
						path[next_vertex],
						path[current_vertex] + 1
					);
				}
				else
				{
					path[next_vertex] = path[current_vertex] + 1;
				}
			}

			visited.insert(current_vertex);

		} while (get_next_vertex_to_visit(current_vertex, visited, path));


		if (path.find(to) != path.end())
			return path.at(to);

		return -1;
	}


private:

	bool get_next_vertex_to_visit(
		int& current_vertex,
		const std::unordered_set<int>& visited,
		const std::unordered_map<int, int>& path
	)
	{
		bool result = false;

		int current_weight = INT_MAX;
		for (auto it = path.begin(); it != path.end(); ++it)
		{
			int vertex = it->first;
			int weight = it->second;

			if (visited.find(vertex) == visited.end()
				&& path.at(vertex) < current_weight)
			{
				current_vertex = vertex;
				current_weight = path.at(vertex);

				result = true;
			}
		}

		return result;
	}

};


int main()
{
	oriented_graph<int> graph;

	graph.add_edge(1, 2);
	graph.add_edge(2, 5);

	graph.add_edge(1, 3);
	graph.add_edge(3, 4);
	graph.add_edge(4, 5);

	PathFinder finder;

	int minimal_path = finder.find_minimal_path(graph, 1, 5);

	std::cout << "Minimal path from 1 to 5 is " << minimal_path << std::endl;
	
	system("pause");
	return 0;
}