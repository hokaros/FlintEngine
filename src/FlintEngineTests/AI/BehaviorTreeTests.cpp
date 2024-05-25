#include "pch.h"
#include <AI/BehaviorTree/Selector.h>
#include <AI/BehaviorTree/Sequence.h>

using bt::ENodeResult;

#ifdef SUITE_NAME
#error Cannot redefine suite name
#endif

#define SUITE_NAME BehaviorTree



class Failer
	: public bt::Node
{
public:
	virtual ENodeResult Run() override { return ENodeResult::Failure; }
};

class Successor
	: public bt::Node
{
public:
	virtual ENodeResult Run() override { return ENodeResult::Success; }
};

class InProgressor
	: public bt::Node
{
public:
	virtual ENodeResult Run() override { return ENodeResult::InProgress; }
};



TEST(SUITE_NAME, EmptySelectorReturnsFailure)
{
	// Arrange
	bt::Selector selector;

	// Act
	ENodeResult result = selector.Run();

	// Assert
	ASSERT_EQ(ENodeResult::Failure, result);
}

TEST(SUITE_NAME, EmptySequenceReturnsSuccess)
{
	// Arrange
	bt::Sequence sequence;

	// Act
	ENodeResult result = sequence.Run();

	// Assert
	ASSERT_EQ(ENodeResult::Success, result);
}

TEST(SUITE_NAME, SelectorWithSuccessOnEndReturnsSuccess)
{
	// Arrange
	bt::Selector selector;
	selector.AddChild(std::make_unique<Failer>());
	selector.AddChild(std::make_unique<Failer>());
	selector.AddChild(std::make_unique<Successor>());

	// Act
	ENodeResult result = selector.Run();

	// Assert
	ASSERT_EQ(ENodeResult::Success, result);
}

TEST(SUITE_NAME, SelectorWithSuccessInMiddleReturnsSuccess)
{
	// Arrange
	bt::Selector selector;
	selector.AddChild(std::make_unique<Failer>());
	selector.AddChild(std::make_unique<Successor>());
	selector.AddChild(std::make_unique<Failer>());

	// Act
	ENodeResult result = selector.Run();

	// Assert
	ASSERT_EQ(ENodeResult::Success, result);
}

TEST(SUITE_NAME, SequenceWithFailureOnEndReturnsFailure)
{
	// Arrange
	bt::Sequence sequence;
	sequence.AddChild(std::make_unique<Successor>());
	sequence.AddChild(std::make_unique<Successor>());
	sequence.AddChild(std::make_unique<Failer>());

	// Act
	ENodeResult result = sequence.Run();

	// Assert
	ASSERT_EQ(ENodeResult::Failure, result);
}

TEST(SUITE_NAME, SequenceWithFailureInMiddleReturnsFailure)
{
	// Arrange
	bt::Sequence sequence;
	sequence.AddChild(std::make_unique<Successor>());
	sequence.AddChild(std::make_unique<Failer>());
	sequence.AddChild(std::make_unique<Successor>());

	// Act
	ENodeResult result = sequence.Run();

	// Assert
	ASSERT_EQ(ENodeResult::Failure, result);
}

TEST(SUITE_NAME, SelectorWithAllFailuresReturnsFailure)
{
	// Arrange
	bt::Selector selector;
	selector.AddChild(std::make_unique<Failer>());
	selector.AddChild(std::make_unique<Failer>());
	selector.AddChild(std::make_unique<Failer>());

	// Act
	ENodeResult result = selector.Run();

	// Assert
	ASSERT_EQ(ENodeResult::Failure, result);
}

TEST(SUITE_NAME, SequenceWithAllSuccessesReturnsSuccess)
{
	// Arrange
	bt::Sequence sequence;
	sequence.AddChild(std::make_unique<Successor>());
	sequence.AddChild(std::make_unique<Successor>());
	sequence.AddChild(std::make_unique<Successor>());

	// Act
	ENodeResult result = sequence.Run();

	// Assert
	ASSERT_EQ(ENodeResult::Success, result);
}

TEST(SUITE_NAME, SelectorWithInProgressInMiddleReturnsInProgress)
{
	// Arrange
	bt::Selector selector;
	selector.AddChild(std::make_unique<Failer>());
	selector.AddChild(std::make_unique<InProgressor>());
	selector.AddChild(std::make_unique<Successor>()); // This will be reached after the InProgressor

	// Act
	ENodeResult result = selector.Run();

	// Assert
	ASSERT_EQ(ENodeResult::InProgress, result);
}

TEST(SUITE_NAME, SequenceWithInProgressInMiddleReturnsInProgress)
{
	// Arrange
	bt::Sequence sequence;
	sequence.AddChild(std::make_unique<Successor>());
	sequence.AddChild(std::make_unique<InProgressor>());
	sequence.AddChild(std::make_unique<Failer>()); // This will be reached after the InProgressor

	// Act
	ENodeResult result = sequence.Run();

	// Assert
	ASSERT_EQ(ENodeResult::InProgress, result);
}