#pragma once

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
};

using IndexPair = std::pair<size_t, size_t>;