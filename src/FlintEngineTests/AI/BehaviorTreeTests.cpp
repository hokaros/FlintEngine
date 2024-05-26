#include "pch.h"
#include <AI/BehaviorTree/Selector.h>
#include <AI/BehaviorTree/Sequence.h>

#include "BehaviorTreeTestHelpers.h"

using bt::ENodeStatus;

#ifdef SUITE_NAME
#error Cannot redefine suite name
#endif

#define SUITE_NAME BehaviorTree



class Failer
	: public bt::Node
{
protected:
	virtual ENodeStatus Update() override { return ENodeStatus::Failure; }
};

class Successor
	: public bt::Node
{
protected:
	virtual ENodeStatus Update() override { return ENodeStatus::Success; }
};

class InProgressor
	: public bt::Node
{
protected:
	virtual ENodeStatus Update() override { return ENodeStatus::InProgress; }
};


class CountingNode
	: public bt::Node
{
private:
	int m_Counter = 0;

protected:
	virtual ENodeStatus Update() override
	{
		m_Counter++;
		return ENodeStatus::Success;
	}

public:
	int GetCounter() const { return m_Counter; }
};

class CountingFailer
	: public CountingNode
{
protected:
	virtual ENodeStatus Update() override
	{
		CountingNode::Update();
		return ENodeStatus::Failure;
	}
};

class CountingSuccessor
	: public CountingNode
{
protected:
	virtual ENodeStatus Update() override
	{
		CountingNode::Update();
		return ENodeStatus::Success;
	}
};

class CountingInProgressor
	: public CountingNode
{
protected:
	virtual ENodeStatus Update() override
	{
		CountingNode::Update();
		return ENodeStatus::InProgress;
	}
};



TEST(SUITE_NAME, EmptySelectorReturnsFailure)
{
	// Arrange
	bt::Selector selector;

	// Act
	ENodeStatus result = selector.Run();

	// Assert
	ASSERT_EQ(ENodeStatus::Failure, result);
}

TEST(SUITE_NAME, EmptySequenceReturnsSuccess)
{
	// Arrange
	bt::Sequence sequence;

	// Act
	ENodeStatus result = sequence.Run();

	// Assert
	ASSERT_EQ(ENodeStatus::Success, result);
}

TEST(SUITE_NAME, SelectorWithSuccessOnEndReturnsSuccess)
{
	// Arrange
	bt::Selector selector;
	selector.AddChild(std::make_unique<Failer>());
	selector.AddChild(std::make_unique<Failer>());
	selector.AddChild(std::make_unique<Successor>());

	// Act
	ENodeStatus result = selector.Run();

	// Assert
	ASSERT_EQ(ENodeStatus::Success, result);
}

TEST(SUITE_NAME, SelectorWithSuccessInMiddleReturnsSuccess)
{
	// Arrange
	bt::Selector selector;
	selector.AddChild(std::make_unique<Failer>());
	selector.AddChild(std::make_unique<Successor>());
	selector.AddChild(std::make_unique<Failer>());

	// Act
	ENodeStatus result = selector.Run();

	// Assert
	ASSERT_EQ(ENodeStatus::Success, result);
}

TEST(SUITE_NAME, SequenceWithFailureOnEndReturnsFailure)
{
	// Arrange
	bt::Sequence sequence;
	sequence.AddChild(std::make_unique<Successor>());
	sequence.AddChild(std::make_unique<Successor>());
	sequence.AddChild(std::make_unique<Failer>());

	// Act
	ENodeStatus result = sequence.Run();

	// Assert
	ASSERT_EQ(ENodeStatus::Failure, result);
}

TEST(SUITE_NAME, SequenceWithFailureInMiddleReturnsFailure)
{
	// Arrange
	bt::Sequence sequence;
	sequence.AddChild(std::make_unique<Successor>());
	sequence.AddChild(std::make_unique<Failer>());
	sequence.AddChild(std::make_unique<Successor>());

	// Act
	ENodeStatus result = sequence.Run();

	// Assert
	ASSERT_EQ(ENodeStatus::Failure, result);
}

TEST(SUITE_NAME, SelectorWithAllFailuresReturnsFailure)
{
	// Arrange
	bt::Selector selector;
	selector.AddChild(std::make_unique<Failer>());
	selector.AddChild(std::make_unique<Failer>());
	selector.AddChild(std::make_unique<Failer>());

	// Act
	ENodeStatus result = selector.Run();

	// Assert
	ASSERT_EQ(ENodeStatus::Failure, result);
}

TEST(SUITE_NAME, SequenceWithAllSuccessesReturnsSuccess)
{
	// Arrange
	bt::Sequence sequence;
	sequence.AddChild(std::make_unique<Successor>());
	sequence.AddChild(std::make_unique<Successor>());
	sequence.AddChild(std::make_unique<Successor>());

	// Act
	ENodeStatus result = sequence.Run();

	// Assert
	ASSERT_EQ(ENodeStatus::Success, result);
}

TEST(SUITE_NAME, SelectorWithInProgressInMiddleReturnsInProgress)
{
	// Arrange
	bt::Selector selector;
	selector.AddChild(std::make_unique<Failer>());
	selector.AddChild(std::make_unique<InProgressor>());
	selector.AddChild(std::make_unique<Successor>()); // This will be reached after the InProgressor

	// Act
	ENodeStatus result = selector.Run();

	// Assert
	ASSERT_EQ(ENodeStatus::InProgress, result);
}

TEST(SUITE_NAME, SequenceWithInProgressInMiddleReturnsInProgress)
{
	// Arrange
	bt::Sequence sequence;
	sequence.AddChild(std::make_unique<Successor>());
	sequence.AddChild(std::make_unique<InProgressor>());
	sequence.AddChild(std::make_unique<Failer>()); // This will be reached after the InProgressor

	// Act
	ENodeStatus result = sequence.Run();

	// Assert
	ASSERT_EQ(ENodeStatus::InProgress, result);
}

TEST(SUITE_NAME, SequenceWithAllSuccessesRunsEveryNode)
{
	// Arrange
	bt::Sequence sequence;

	CountingSuccessor& first_child = BehaviorTreeTestHelpers::AddChildTo<CountingSuccessor>(sequence);
	CountingSuccessor& second_child = BehaviorTreeTestHelpers::AddChildTo<CountingSuccessor>(sequence);
	CountingSuccessor& third_child = BehaviorTreeTestHelpers::AddChildTo<CountingSuccessor>(sequence);

	// Pre-act assert
	ASSERT_EQ(0, first_child.GetCounter());
	ASSERT_EQ(0, second_child.GetCounter());
	ASSERT_EQ(0, third_child.GetCounter());

	// Act
	sequence.Run();

	// Assert
	ASSERT_EQ(1, first_child.GetCounter());
	ASSERT_EQ(1, second_child.GetCounter());
	ASSERT_EQ(1, third_child.GetCounter());
}

TEST(SUITE_NAME, SequenceWithInProgressInMiddleSkipsPreviousNodesOn2ndRun)
{
	// Arrange
	bt::Sequence sequence;

	CountingSuccessor& first_child = BehaviorTreeTestHelpers::AddChildTo<CountingSuccessor>(sequence);
	CountingInProgressor& in_progress_child = BehaviorTreeTestHelpers::AddChildTo<CountingInProgressor>(sequence);
	CountingSuccessor& third_child = BehaviorTreeTestHelpers::AddChildTo<CountingSuccessor>(sequence);

	// Pre-act assert
	ASSERT_EQ(0, first_child.GetCounter());
	ASSERT_EQ(0, in_progress_child.GetCounter());
	ASSERT_EQ(0, third_child.GetCounter());

	// Act 1
	sequence.Run();

	// Assert 1
	ASSERT_EQ(1, first_child.GetCounter());
	ASSERT_EQ(1, in_progress_child.GetCounter());
	ASSERT_EQ(0, third_child.GetCounter());

	// Act 2
	sequence.Run();

	// Assert 2
	ASSERT_EQ(1, first_child.GetCounter());
	ASSERT_EQ(2, in_progress_child.GetCounter());
	ASSERT_EQ(0, third_child.GetCounter());
}

TEST(SUITE_NAME, SelectorWithInProgressInMiddleSkipsPreviousNodesOn2ndRun)
{
	// Arrange
	bt::Selector selector;

	CountingFailer& first_child = BehaviorTreeTestHelpers::AddChildTo<CountingFailer>(selector);
	CountingInProgressor& in_progress_child = BehaviorTreeTestHelpers::AddChildTo<CountingInProgressor>(selector);
	CountingFailer& third_child = BehaviorTreeTestHelpers::AddChildTo<CountingFailer>(selector);

	// Pre-act assert
	ASSERT_EQ(0, first_child.GetCounter());
	ASSERT_EQ(0, in_progress_child.GetCounter());
	ASSERT_EQ(0, third_child.GetCounter());

	// Act 1
	selector.Run();

	// Assert 1
	ASSERT_EQ(1, first_child.GetCounter());
	ASSERT_EQ(1, in_progress_child.GetCounter());
	ASSERT_EQ(0, third_child.GetCounter());

	// Act 2
	selector.Run();

	// Assert 2
	ASSERT_EQ(1, first_child.GetCounter());
	ASSERT_EQ(2, in_progress_child.GetCounter());
	ASSERT_EQ(0, third_child.GetCounter());
}