#pragma once
#include <unordered_map>
#include <unordered_set>
#include <algorithm>
#include <xhash>

// CLASS TEMPLATE oriented_graph
template<class _Kty,
	class _Hasher = std::hash<_Kty>,
	class _Eq = std::equal_to<_Kty>>
	class oriented_graph
{

	using container_type = _Kty;
	using hasher = _Hasher;
	using container_compare = _Eq;

	using adjacency_list_container_data_type = std::unordered_multiset
		<container_type, hasher, container_compare>;

	using adjacency_list_container_type = std::unordered_map
		<container_type, adjacency_list_container_data_type, hasher>;

public:

	/* Iterator interface */
	template<class _Kty,
		class _Hasher = std::hash<_Kty>,
		class _Eq = std::equal_to<_Kty>>
		class iterator
	{

		using container_type = _Kty;
		using hasher = _Hasher;
		using container_compare = _Eq;

	public:

		virtual const bool is_end() const = 0;
		virtual void next() const = 0;
		virtual const container_type& value() const = 0;

	protected:

		iterator() {}
		iterator(const iterator& r) {}
		iterator(const iterator&& r) {}

	};
	/* END Iterator interface */


	/* Iterators */
	template<class _Kty,
		class _Hasher = std::hash<_Kty>,
		class _Eq = std::equal_to<_Kty>>
		class vertex_iterator : public iterator<_Kty, _Hasher, _Eq>
	{

		using container_type = _Kty;
		using hasher = _Hasher;
		using container_compare = _Eq;

		using iterator_type = typename adjacency_list_container_type::const_iterator;

	public:

		inline const bool is_end() const override
		{
			return this->_current_iterator == this->_end;
		}

		inline void next() const noexcept override
		{
			if (is_end())
				return;

			++this->_current_iterator;
		}

		inline const container_type& value() const override
		{
			return this->_current_iterator->first;
		}

	private:

		vertex_iterator(iterator_type from, iterator_type end) noexcept :
			_current_iterator(from),
			_end(end)
		{ }


		mutable iterator_type _current_iterator;
		iterator_type _end;

		friend oriented_graph;

	};

	template<class _Kty,
		class _Hasher = std::hash<_Kty>,
		class _Eq = std::equal_to<_Kty>>
		class edge_iterator : public iterator<_Kty, _Hasher, _Eq>
	{

		using container_type = _Kty;
		using hasher = _Hasher;
		using container_compare = _Eq;

		using iterator_type = typename adjacency_list_container_data_type::const_iterator;

	public:

		inline const bool is_end() const noexcept override
		{
			return this->_current_iterator == this->_end;
		}

		inline void next() const noexcept override
		{
			if (is_end())
				return;

			++this->_current_iterator;
		}

		inline const container_type& value() const override
		{
			return *this->_current_iterator;
		}

	private:

		edge_iterator(iterator_type& from, iterator_type& end) noexcept :
			_current_iterator(from),
			_end(end)
		{ }


		mutable iterator_type _current_iterator;
		iterator_type _end;

		friend oriented_graph;

	};

	/* END Iterators */

public:

	explicit oriented_graph() noexcept
	{ }

	/*
	 * Returns true if the vertex presented into the graph.
	 *
	 */
	inline const bool contains_vertex(const container_type& vertex) const noexcept
	{
		const auto it = this->_edges_out.find(vertex);

		return it != this->_edges_out.end();
	}

	/*
	 * Returns true if the edge presented into the graph.
	 *
	 */
	inline const bool contains_edge(const container_type& from,
		const container_type& to) const noexcept
	{
		const auto it = this->_edges_out.find(from);

		return it != this->_edges_out.end()
			&& it->second.find(to) != it->second.end();
	}

	/*
	 * Returns vertices count in the graph.
	 *
	 */
	inline const size_t vertices_count() const noexcept
	{
		return this->_vertices_count;
	}

	/*
	 * Returns edges count in the graph.
	 *
	 */
	inline const size_t edges_count() const noexcept
	{
		return this->_edges_count;
	}

	/*
	 * Returns vertex degree or -1 if vertex is not into the graph.
	 *
	 */
	inline const int vertex_degree(const container_type& vertex) const noexcept
	{
		if (!this->contains_vertex(vertex))
			return -1;

		const auto it = this->_edges_out.find(vertex);
		return it->second.size();
	}

	/*
	 * Returns edge degree (for this type of graph it's always 2).
	 *
	 */
	inline const size_t edge_degree() const noexcept
	{
		return oriented_graph::_edge_degree;
	}

	/*
	 * Adds vertex to the graph.
	 * Returns iterator to that vetex.
	 *
	 */
	inline vertex_iterator<container_type, hasher, container_compare>
		add_vertex(const container_type& vertex) noexcept
	{
		if (!this->contains_vertex(vertex))
		{
			this->_edges_out[vertex]
				= this->_edges_in[vertex]
				= std::forward<adjacency_list_container_data_type>(
				(adjacency_list_container_data_type()));

			++this->_vertices_count;
		}

		return find_vertex(vertex);
	}

	/*
	 * Adds edge to the graph.
	 * Returns iterator to that edge.
	 *
	 */
	inline edge_iterator<container_type, hasher, container_compare>
		add_edge(const container_type& from, const container_type& to) noexcept
	{
		this->add_vertex(from);
		this->add_vertex(to);

		this->_edges_out[from].insert(to);
		this->_edges_in[to].insert(from);

		++this->_edges_count;

		return this->find_edge(from, to);
	}

	/*
	 * Erase vertex from the graph.
	 *
	 * Returns number of edges that was erased with vertex,
	 * or -1 if vertex wasn't presented into the graph.
	 *
	 */
	inline const int erase_vertex(const container_type& vertex) noexcept
	{
		if (!this->contains_vertex(vertex))
			return -1;

		auto out_it = _edges_out.find(vertex);
		auto in_it = _edges_in.find(vertex);

		int result = (out_it->second.size() + in_it->second.size());
		this->_edges_count -= result;

		this->_edges_out.erase(out_it);
		this->_edges_in.erase(in_it);

		--this->_vertices_count;
		return result;
	}

	/*
	 * Erase edge from the graph.
	 *
	 * Returns true if edge was presented into the graph and deleted,
	 * otherwise returns false.
	 *
	 */
	inline const bool erase_edge(const container_type& from,
		const container_type& to) noexcept
	{
		if (!this->contains_edge(from, to))
			return false;

		auto& edges_out = this->_edges_out[from];
		auto& edges_in = this->_edges_in[to];

		edges_out.erase(edges_out.lower_bound(to));
		edges_in.erase(edges_in.lower_bound(from));

		--this->_edges_count;
		return true;
	}

	/*
	 * Returns iterator to first vertex (vertices located in random order).
	 *
	 */
	inline auto begin_vertex() const noexcept
	{
		auto from_it = this->_edges_out.begin();
		auto end_it = this->_edges_out.end();

		return vertex_iterator<container_type, hasher, container_compare>(from_it, end_it);
	}

	/*
	 * Returns iterator to first edge from vertex (edges located in random order).
	 *
	 */
	inline auto begin_edge(const container_type& vertex) const noexcept
	{
		auto from_it = this->_edges_out.at(vertex).cbegin();
		auto end_it = this->_edges_out.at(vertex).cend();

		return edge_iterator<container_type, hasher,
			container_compare>(from_it, end_it);
	}

	/*
	 * Returns iterator to vertex.
	 *
	 */
	inline auto find_vertex(const container_type& vertex) const noexcept
	{
		auto from_it = this->_edges_out.find(vertex);
		auto end_it = this->_edges_out.end();

		return vertex_iterator<container_type, hasher,
			container_compare>(from_it, end_it);
	}

	/*
	 * Returns iterator to edge.
	 *
	 */
	inline const edge_iterator<container_type, hasher,
		container_compare> find_edge(const container_type& from, const container_type& to) const
	{
		if (!this->contains_vertex(from) || !this->contains_vertex(to))
		{
			adjacency_list_container_data_type empty;

			auto from_it = empty.cbegin();
			auto end_it = empty.cend();

			return edge_iterator<container_type, hasher,
				container_compare>(from_it, end_it);
		}

		
		auto from_it = this->_edges_out.at(from).find(to);
		auto end_it = this->_edges_out.at(from).end();

		return edge_iterator<container_type, hasher,
			container_compare>(from_it, end_it);
	}

	/*
	 * Returns true if the graph is empty (0 vertices).
	 *
	 */
	inline const bool empty() const noexcept
	{
		return this->vertices_count() == 0;
	}

	/*
	 * Erase all vertices and edges in the graph.
	 *
	 */
	inline void clear() noexcept
	{
		this->_edges_out.clear();
		this->_edges_in.clear();

		this->_edges_count = 0;
		this->_vertices_count = 0;
	}

private:

	adjacency_list_container_type _edges_out; /* vertex -> multiset(vertex) */
	adjacency_list_container_type _edges_in;  /* vertex -> multiset(vertex) */


	size_t _edges_count = 0;
	size_t _vertices_count = 0;

	static constexpr size_t _edge_degree = 2;

};