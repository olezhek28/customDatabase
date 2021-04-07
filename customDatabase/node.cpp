#include "node.h"

using namespace std;

bool LogicalOperationNode::Evaluate(const Date& date, const string& event)
{
	switch (operation)
	{
		case Or: 
			return left->Evaluate(date, event) || right->Evaluate(date, event);
		
		case And:
			return left->Evaluate(date, event) && right->Evaluate(date, event);
		
		default:
			throw invalid_argument("Operation comparison not found");
	}
}

bool EmptyNode::Evaluate(const Date& date, const string& event)
{
	return true;
}

DateComparisonNode::DateComparisonNode(Comparison cmp, const Date& date)
{
	this->date = date;
	this->cmp = cmp;
}

bool DateComparisonNode::Evaluate(const Date& date, const string& event)
{
	switch(cmp)
	{
		case Comparison::Less:
			return this->date > date;
		
		case Comparison::LessOrEqual:
			return this->date >= date;

		case Comparison::Greater:
			return this->date < date;

		case Comparison::GreaterOrEqual:
			return this->date <= date;
		
		case Comparison::Equal:
			return this->date == date;
		
		case Comparison::NotEqual:
			return this->date != date;
		
		default:
			throw invalid_argument("Operation comparison not found");
	}
}

EventComparisonNode::EventComparisonNode(Comparison cmp, const string& event)
{
	this->event = event;
	this->cmp = cmp;
}

bool EventComparisonNode::Evaluate(const Date& date, const string& event)
{
	switch(cmp)
	{
		case Comparison::Less:
			return this->event > event;
		
		case Comparison::LessOrEqual:
			return this->event >= event;

		case Comparison::Greater:
			return this->event < event;

		case Comparison::GreaterOrEqual:
			return this->event <= event;
		
		case Comparison::Equal:
			return this->event == event;
		
		case Comparison::NotEqual:
			return this->event != event;
		
		default:
			throw invalid_argument("Operation comparison not found");
	}
}
