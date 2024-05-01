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

	std::unique_ptr<dt::DecisionTreeResult<int>> result = std::make_unique<dt::DecisionTreeResult<int>>(secret_result);
	dt::DecisionTree<int> tree(std::move(result));

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

	std::unique_ptr<dt::DecisionTreeResult<int>> expected_node = std::make_unique<dt::DecisionTreeResult<int>>(expected_result);
	std::unique_ptr<dt::DecisionTreeResult<int>> unexpected_node = std::make_unique<dt::DecisionTreeResult<int>>(unexpected_result);
	std::unique_ptr<dt::DecisionTreeSplit<int>> true_condition = std::make_unique<dt::DecisionTreeSplit<int>>(std::make_unique<TrueCondition>(), std::move(expected_node), std::move(unexpected_node));
	dt::DecisionTree<int> tree(std::move(true_condition));

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

	std::unique_ptr<dt::DecisionTreeResult<int>> expected_node = std::make_unique<dt::DecisionTreeResult<int>>(expected_result);
	std::unique_ptr<dt::DecisionTreeResult<int>> unexpected_node = std::make_unique<dt::DecisionTreeResult<int>>(unexpected_result);
	std::unique_ptr<dt::DecisionTreeSplit<int>> true_condition = std::make_unique<dt::DecisionTreeSplit<int>>(std::make_unique<FalseCondition>(), std::move(unexpected_node), std::move(expected_node));
	dt::DecisionTree<int> tree(std::move(true_condition));

	// Act
	const int decision = tree.Decide();

	// Assert
	ASSERT_EQ(expected_result, decision);
}

#undef SUITE_NAME