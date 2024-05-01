#include "pch.h"

#include <AI/DecisionTree/DecisionTree.h>
#include <AI/DecisionTree/DecisionTreeResult.h>
#include <AI/DecisionTree/DecisionTreeSplit.h>

#ifdef SUITE_NAME
#error Cannot redefine suite name
#endif

#define SUITE_NAME DecisionTree



class TrueCondition
	: public dt::ICondition
{
public:
	virtual bool Test() const override
	{
		return true;
	}
};

class FalseCondition
	: public dt::ICondition
{
public:
	virtual bool Test() const override
	{
		return false;
	}
};



TEST(SUITE_NAME, DecidesRootResult)
{
	// Arrange
	const int secret_result = 7;

	dt::DecisionTreeResult<int> result(secret_result);
	dt::DecisionTree<int> tree(result);

	// Act
	const int decision = tree.Decide();

	// Assert
	ASSERT_EQ(secret_result, decision);
}

TEST(SUITE_NAME, BranchesTrueCondition)
{
	// Arrange
	const int expected_result = 7;
	const int unexpected_result = 6;

	dt::DecisionTreeResult<int> expected_node(expected_result);
	dt::DecisionTreeResult<int> unexpected_node(unexpected_result);
	TrueCondition truth;
	dt::DecisionTreeSplit<int> true_condition(truth, expected_node, unexpected_node);

	dt::DecisionTree<int> tree(true_condition);

	// Act
	const int decision = tree.Decide();

	// Assert
	ASSERT_EQ(expected_result, decision);
}

TEST(SUITE_NAME, BranchesFalseCondition)
{
	// Arrange
	const int expected_result = 7;
	const int unexpected_result = 6;

	dt::DecisionTreeResult<int> expected_node(expected_result);
	dt::DecisionTreeResult<int> unexpected_node(unexpected_result);
	FalseCondition falsth;
	dt::DecisionTreeSplit<int> false_condition(falsth, unexpected_node, expected_node);
	dt::DecisionTree<int> tree(false_condition);

	// Act
	const int decision = tree.Decide();

	// Assert
	ASSERT_EQ(expected_result, decision);
}

#undef SUITE_NAME