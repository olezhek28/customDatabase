#include "conditionParser.h"
#include "testRunner.h"
#include "date.h"
#include "database.h"

#include <sstream>
using namespace std;

bool operator==(const std::pair<Date, const std::string>& lhs, const std::pair<Date, const std::string>& rhs)
{
	return tuple<Date, string>(lhs.first, lhs.second) == tuple<Date, string>(rhs.first, rhs.second);
}

void TestParseCondition()
{
	{
		Database database;

		try
		{
			auto pr = database.Last(Date{2017, 1, 1});
			cout << pr.first << " " << pr.second << endl;
		}
		catch (exception ex)
		{
			cout << ex.what() << " Check Last 1" << endl;
		}
		
		//Assert(database.Last(Date{2017, 1, 1}) == "No entry", "Check Last 1");

		database.Add({2017, 1, 1}, "111");
		database.Add({2017, 1, 1}, "222");
		database.Add({2017, 1, 1}, "000");

		Assert(database.Last(Date{2017, 1, 1}) == pair{Date{2017, 1, 1}, "000"}, "Check Last 2");

		database.Add({2018, 2, 3}, "555");
		database.Add({2018, 2, 3}, "666");
		database.Add({2018, 2, 3}, "444");

		istringstream is("date == 2018-2-3");
		auto condition = ParseCondition(is);
		auto predicate = [condition](const Date& date, const string& event)
		{
			return condition->Evaluate(date, event);
		};

		cout << database.RemoveIf(predicate) << endl;

		Assert(database.Last(Date{2019, 8, 1}) == pair{Date{2017, 1, 1}, "000"}, "Check Last 3");
		
		/*Assert(database.Last(Date{2017, 8, 1}) == pair{Date{2017, 1, 1}, "000"}, "Check Last 3");
		Assert(database.Last(Date{2018, 8, 1}) == pair{Date{2018, 2, 3}, "444"}, "Check Last 4");

		try
		{
			auto pr = database.Last(Date{2016, 1, 1});
			cout << pr.first << " " << pr.second << endl;
		}
		catch (exception ex)
		{
			cout << ex.what() << " Check Last 5" << endl;
		}*/
	}
	
	{
		istringstream is("date != 2017-11-18");
		shared_ptr<Node> root = ParseCondition(is);
		Assert(root->Evaluate(Date{2017, 1, 1}, ""), "Parse condition 1");
		Assert(!root->Evaluate({2017, 11, 18}, ""), "Parse condition 2");
	}

	{
		istringstream is("date < 2017-11-18");
		shared_ptr<Node> root = ParseCondition(is);
		Assert(root->Evaluate(Date{2017, 1, 1}, ""), "Parse condition 1*");
		Assert(!root->Evaluate({2017, 11, 18}, ""), "Parse condition 2*");
		Assert(!root->Evaluate({2017, 12, 18}, ""), "Parse condition 3*");
	}

	{
		istringstream is(R"(event < "111")");
		shared_ptr<Node> root = ParseCondition(is);
		Assert(!root->Evaluate({2017, 1, 1}, "222"), "Parse condition 1**");
		Assert(root->Evaluate({2017, 11, 18}, "000"), "Parse condition 2**");
		Assert(!root->Evaluate({2017, 12, 18}, "111"), "Parse condition 3**");
	}
	
	{
		istringstream is(R"(event == "sport event")");
		shared_ptr<Node> root = ParseCondition(is);
		Assert(root->Evaluate({2017, 1, 1}, "sport event"), "Parse condition 3");
		Assert(!root->Evaluate({2017, 1, 1}, "holiday"), "Parse condition 4");
	}

	{
		istringstream is("date >= 2017-01-01 AND date < 2017-07-01");
		shared_ptr<Node> root = ParseCondition(is);
		Assert(root->Evaluate({2017, 1, 1}, ""), "Parse condition 5");
		Assert(root->Evaluate({2017, 3, 1}, ""), "Parse condition 6");
		Assert(root->Evaluate({2017, 6, 30}, ""), "Parse condition 7");
		Assert(!root->Evaluate({2017, 7, 1}, ""), "Parse condition 8");
		Assert(!root->Evaluate({2016, 12, 31}, ""), "Parse condition 9");
	}

	{
		istringstream is("date != 2017-11-17  ");
		shared_ptr<Node> root = ParseCondition(is);
		Assert(root->Evaluate({2016, 11, 17}, ""), "Parse condition 9*");
	}

	{
		istringstream is(R"(event != "sport event" AND event != "Wednesday")");
		shared_ptr<Node> root = ParseCondition(is);
		Assert(root->Evaluate({2017, 1, 1}, "holiday"), "Parse condition 10");
		Assert(!root->Evaluate({2017, 1, 1}, "sport event"), "Parse condition 11");
		Assert(!root->Evaluate({2017, 1, 1}, "Wednesday"), "Parse condition 12");
	}
	
	{
		istringstream is(R"(event == "holiday AND date == 2017-11-18")");
		shared_ptr<Node> root = ParseCondition(is);
		Assert(!root->Evaluate({2017, 11, 18}, "holiday"), "Parse condition 13");
		Assert(!root->Evaluate({2017, 11, 18}, "work day"), "Parse condition 14");
		Assert(root->Evaluate({1, 1, 1}, "holiday AND date == 2017-11-18"), "Parse condition 15");
	}

	{
		istringstream is(R"(((event == "holiday" AND date == 2017-01-01)))");
		shared_ptr<Node> root = ParseCondition(is);
		Assert(root->Evaluate({2017, 1, 1}, "holiday"), "Parse condition 16");
		Assert(!root->Evaluate({2017, 1, 2}, "holiday"), "Parse condition 17");
	}

	{
		istringstream is(R"(date > 2017-01-01 AND (event == "holiday" OR date < 2017-07-01))");
		shared_ptr<Node> root = ParseCondition(is);
		Assert(!root->Evaluate({2016, 1, 1}, "holiday"), "Parse condition 18");
		Assert(root->Evaluate({2017, 1, 2}, "holiday"), "Parse condition 19");
		Assert(root->Evaluate({2017, 1, 2}, "workday"), "Parse condition 20");
		Assert(!root->Evaluate({2018, 1, 2}, "workday"), "Parse condition 21");
	}

	{
		istringstream is(R"(date > 2017-01-01 AND event == "holiday" OR date < 2017-07-01)");
		shared_ptr<Node> root = ParseCondition(is);
		Assert(root->Evaluate({2016, 1, 1}, "event"), "Parse condition 22");
		Assert(root->Evaluate({2017, 1, 2}, "holiday"), "Parse condition 23");
		Assert(root->Evaluate({2017, 1, 2}, "workday"), "Parse condition 24");
		Assert(!root->Evaluate({2018, 1, 2}, "workday"), "Parse condition 25");
	}

	{
		istringstream is(R"(((date == 2017-01-01 AND event == "holiday")))");
		shared_ptr<Node> root = ParseCondition(is);
		Assert(root->Evaluate({2017, 1, 1}, "holiday"), "Parse condition 26");
		Assert(!root->Evaluate({2017, 1, 2}, "holiday"), "Parse condition 27");
	}

	{
		istringstream is(R"(((event == "2017-01-01" OR date > 2016-01-01)))");
		shared_ptr<Node> root = ParseCondition(is);
		Assert(root->Evaluate({1, 1, 1}, "2017-01-01"), "Parse condition 28");
		Assert(!root->Evaluate({2016, 1, 1}, "event"), "Parse condition 29");
		Assert(root->Evaluate({2016, 1, 2}, "event"), "Parse condition 30");
	}
}
