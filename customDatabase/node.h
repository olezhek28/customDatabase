#pragma once
#include <string>
#include <utility>
#include <memory>

#include "date.h"

enum class Comparison
{
	Less,
	LessOrEqual,
	Greater,
	GreaterOrEqual,
	Equal,
	NotEqual
};

enum LogicalOperation
{
	Or,
	And
};

class Node
{
public:
	virtual ~Node() = default;

	virtual bool Evaluate(const Date& date, const std::string& event) = 0;
};

class LogicalOperationNode : public Node
{
public:
	LogicalOperationNode(const LogicalOperation operation, std::shared_ptr<Node> left, std::shared_ptr<Node> right)
	{
		this->operation = operation;
		this->left = std::move(left);
		this->right = std::move(right);
	}

	bool Evaluate(const Date& date, const std::string& event) override;

private:
	std::shared_ptr<Node> left;
	std::shared_ptr<Node> right;
	LogicalOperation operation;
};

class EmptyNode : public Node
{
	bool Evaluate(const Date& date, const std::string& event) override;
};

class DateComparisonNode : public Node
{
public:
	
	DateComparisonNode(Comparison cmp, const Date& date);

	bool Evaluate(const Date& date, const std::string& event) override;

private:
	Date date{};
	Comparison cmp;
};


class EventComparisonNode : public Node
{
public:
	
	EventComparisonNode(Comparison cmp, const std::string& event);

	bool Evaluate(const Date& date, const std::string& event) override;

private:
	std::string event;
	Comparison cmp;
};