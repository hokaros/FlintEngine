#include "pch.h"
#include <FTL/PriorityQueue.h>

#ifdef SUITE_NAME
#error Cannot redefine suite name
#endif

#define SUITE_NAME PriorityQueue

using ftl::PriorityQueue;

float returnExact(float value)
{
	return value;
}

TEST(SUITE_NAME, AddsToFront)
{
	// Arrange
	PriorityQueue<float> queue(returnExact);

	// Act
	queue.Enqueue(5);
	queue.Enqueue(10);

	// Assert
	ASSERT_EQ(10, queue.PeekFront());
}

TEST(SUITE_NAME, AddsToMiddle)
{
	// Arrange
	PriorityQueue<float> queue(returnExact);

	// Act
	queue.Enqueue(10);
	queue.Enqueue(5);
	queue.Enqueue(7);

	// Assert
	ASSERT_EQ(10, queue.PeekFront());
	queue.PopFront();
	ASSERT_EQ(7, queue.PeekFront());
}

TEST(SUITE_NAME, AddsToBack)
{
	// Arrange
	PriorityQueue<float> queue(returnExact);

	// Act
	queue.Enqueue(10);
	queue.Enqueue(7);
	queue.Enqueue(5);

	// Assert
	ASSERT_EQ(10, queue.PeekFront());
	queue.PopFront();
	ASSERT_EQ(7, queue.PeekFront());
	queue.PopFront();
	ASSERT_EQ(5, queue.PeekFront());
}

#undef SUITE_NAME