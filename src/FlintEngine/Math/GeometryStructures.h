#pragma once

struct IndexPair
	: public std::pair<size_t, size_t>
{

	constexpr IndexPair(size_t first, size_t second)
		: std::pair<size_t, size_t>(first, second)
	{}

	constexpr bool ContainsIndex(size_t index) const
	{
		return first == index || second == index;
	}

	constexpr bool operator==(const IndexPair& other) const
	{
		return (first == other.first && second == other.second)
			|| (first == other.second && second == other.first);
	}
};

struct IndexTriangle
{
public:
	int idx1 = -1;
	int idx2 = -1;
	int idx3 = -1;

public:
	constexpr IndexTriangle(int idx1, int idx2, int idx3)
		: idx1(idx1)
		, idx2(idx2)
		, idx3(idx3)
	{}

	constexpr bool ContainsMutualEdge(const IndexTriangle& other) const
	{
		int mutual_idx_count = 0;

		if (other.ContainsIndex(idx1))
		{
			mutual_idx_count++;
		}
		if (other.ContainsIndex(idx2))
		{
			mutual_idx_count++;
		}
		if (other.ContainsIndex(idx3))
		{
			mutual_idx_count++;
		}

		return mutual_idx_count >= 2;
	}

	constexpr bool ContainsIndex(int idx) const
	{
		return idx1 == idx
			|| idx2 == idx
			|| idx3 == idx;
	}

	constexpr bool ContainsIndexPair(const IndexPair& pair) const
	{
		return ContainsIndex(pair.first)
			&& ContainsIndex(pair.second);
	}
};