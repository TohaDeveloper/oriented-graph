#include "pch.h"

#include <oriented_graph.hpp>


TEST(Graph, VertexCreation)
{
	oriented_graph<int> graph;

	EXPECT_FALSE(graph.contains_vertex(1));
	EXPECT_EQ(graph.vertices_count(), 0);

	graph.add_vertex(1);

	EXPECT_EQ(graph.vertices_count(), 1);

	EXPECT_TRUE(graph.contains_vertex(1));

	EXPECT_FALSE(graph.contains_vertex(-1));
	EXPECT_FALSE(graph.contains_vertex(INT_MAX));
	EXPECT_FALSE(graph.contains_vertex(INT_MIN));
}

TEST(Graph, VertexDeletion)
{
	oriented_graph<int> graph;

	EXPECT_FALSE(graph.contains_vertex(-1));
	EXPECT_EQ(graph.vertices_count(), 0);

	graph.add_vertex(-1);

	EXPECT_EQ(graph.vertices_count(), 1);
	EXPECT_TRUE(graph.contains_vertex(-1));

	EXPECT_EQ(graph.erase_vertex(-1), 0);

	EXPECT_EQ(graph.vertices_count(), 0);
	EXPECT_FALSE(graph.contains_vertex(-1));
}

TEST(Graph, EdgeCreation)
{
	oriented_graph<int> graph;

	EXPECT_FALSE(graph.contains_vertex(1));
	EXPECT_FALSE(graph.contains_vertex(2));

	EXPECT_FALSE(graph.contains_edge(1, 2));
	EXPECT_FALSE(graph.contains_edge(2, 1));

	EXPECT_EQ(graph.edges_count(), 0);
	EXPECT_EQ(graph.vertices_count(), 0);

	graph.add_edge(1, 2);

	EXPECT_EQ(graph.vertices_count(), 2);
	EXPECT_EQ(graph.edges_count(), 1);

	EXPECT_TRUE(graph.contains_vertex(1));
	EXPECT_TRUE(graph.contains_vertex(2));

	EXPECT_TRUE(graph.contains_edge(1, 2));
	EXPECT_FALSE(graph.contains_edge(2, 1));
}

TEST(Graph, VertexDeletionWithEdge)
{
	oriented_graph<int> graph;

	EXPECT_FALSE(graph.contains_edge(1, 2));
	EXPECT_FALSE(graph.contains_edge(2, 1));

	EXPECT_EQ(graph.edges_count(), 0);

	graph.add_edge(1, 2);

	EXPECT_EQ(graph.edges_count(), 1);

	EXPECT_TRUE(graph.contains_vertex(1));
	EXPECT_TRUE(graph.contains_vertex(2));

	EXPECT_TRUE(graph.contains_edge(1, 2));
	EXPECT_FALSE(graph.contains_edge(2, 1));

	EXPECT_EQ(graph.erase_vertex(1), 1);

	EXPECT_EQ(graph.edges_count(), 0);

	EXPECT_FALSE(graph.contains_edge(1, 2));
	EXPECT_FALSE(graph.contains_edge(2, 1));

	EXPECT_FALSE(graph.contains_vertex(1));
	EXPECT_TRUE(graph.contains_vertex(2));
}

TEST(Graph, EdgeDeletion)
{
	oriented_graph<int> graph;

	EXPECT_FALSE(graph.contains_edge(1, 2));
	EXPECT_FALSE(graph.contains_edge(2, 1));

	EXPECT_EQ(graph.edges_count(), 0);

	graph.add_edge(1, 2);

	EXPECT_TRUE(graph.contains_vertex(1));
	EXPECT_TRUE(graph.contains_vertex(2));

	EXPECT_TRUE(graph.contains_edge(1, 2));
	EXPECT_FALSE(graph.contains_edge(2, 1));

	EXPECT_TRUE(graph.erase_edge(1, 2));
	EXPECT_FALSE(graph.erase_edge(1, 2));

	EXPECT_EQ(graph.edges_count(), 0);

	EXPECT_FALSE(graph.contains_edge(1, 2));
	EXPECT_FALSE(graph.contains_edge(2, 1));

	EXPECT_TRUE(graph.contains_vertex(1));
	EXPECT_TRUE(graph.contains_vertex(2));
}

TEST(Graph, VertexDegree)
{
	oriented_graph<int> graph;

	EXPECT_EQ(graph.vertex_degree(1), -1);

	graph.add_edge(1, 2);
	graph.add_edge(1, 3);

	EXPECT_TRUE(graph.contains_edge(1, 2));
	EXPECT_TRUE(graph.contains_edge(1, 3));

	EXPECT_EQ(graph.vertex_degree(1), 2);
	EXPECT_EQ(graph.vertex_degree(2), 0);
	EXPECT_EQ(graph.vertex_degree(3), 0);
}

TEST(Graph, EdgeDegree)
{
	oriented_graph<int> graph;

	EXPECT_EQ(graph.edge_degree(), 2);
}

TEST(Graph, Clear)
{
	oriented_graph<int> graph;

	graph.add_vertex(1);
	graph.add_edge(2, 3);

	EXPECT_EQ(graph.edges_count(), 1);
	EXPECT_EQ(graph.vertices_count(), 3);

	graph.clear();

	EXPECT_EQ(graph.edges_count(), 0);
	EXPECT_EQ(graph.vertices_count(), 0);
}

TEST(Graph, Empty)
{
	oriented_graph<int> graph;

	EXPECT_TRUE(graph.empty());

	graph.add_vertex(1);

	EXPECT_FALSE(graph.empty());

	graph.add_edge(1, 2);

	graph.erase_vertex(1);

	EXPECT_FALSE(graph.empty());

	graph.erase_vertex(2);

	EXPECT_TRUE(graph.empty());
}

TEST(GraphIterator, FindVertex)
{
	oriented_graph<int> graph;

	auto it_before = graph.find_vertex(1);

	EXPECT_TRUE(it_before.is_end());

	graph.add_vertex(1);

	auto it_after = graph.find_vertex(1);

	EXPECT_FALSE(it_after.is_end());
	EXPECT_EQ(it_after.value(), 1);
}

TEST(GraphIterator, FindEdge)
{
	oriented_graph<int> graph;

	auto it_before = graph.find_edge(1, 2);

	EXPECT_TRUE(it_before.is_end());

	graph.add_edge(1, 2);

	auto it_after = graph.find_edge(1, 2);

	EXPECT_FALSE(it_after.is_end());
	EXPECT_EQ(it_after.value(), 2);
}


TEST(GraphIterator, VertexIteratorBegin)
{
	oriented_graph<int> graph;

	auto it_before = graph.begin_vertex();

	EXPECT_TRUE(it_before.is_end());

	graph.add_vertex(1);

	auto it_after = graph.begin_vertex();

	EXPECT_FALSE(it_after.is_end());
	EXPECT_EQ(it_after.value(), 1);
}

TEST(GraphIterator, EdgeIteratorBegin)
{
	oriented_graph<int> graph;

	auto it_before = graph.begin_edge(1);

	EXPECT_TRUE(it_before.is_end());

	graph.add_edge(1, 2);

	auto it_after = graph.begin_edge(1);

	EXPECT_FALSE(it_after.is_end());
	EXPECT_EQ(it_after.value(), 2);
}