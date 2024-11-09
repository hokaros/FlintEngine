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
};

using IndexPair = std::pair<size_t, size_t>;