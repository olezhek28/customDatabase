#include <iostream>
#include <sstream>

#include "database.h"
#include "date.h"
#include "conditionParser.h"
#include "node.h"
#include "testRunner.h"

using namespace std;

void TestAll();

string ParseEvent(istream& is)
{
	std::string event;
	bool first = true;

	while(true)
	{
		std::string tmp;

		if(!first)
		{
			getline(is, tmp);
			event += tmp;
			
			break;
		}

		first = false;
		is >> tmp;
		event += tmp;
	}

	return event;
}

ostream& operator<<(ostream& stream, const pair<Date, string>& value)
{
	stream << value.first << " " << value.second;

	return stream;
}

int main()
{
	TestAll();

	return 0;

	Database db;

	for (string line; getline(cin, line); ) 
	{
		try
		{
			istringstream is(line);

			string command;
			is >> command;
			
			if (command == "Add") 
			{
				const auto date = ParseDate(is);
				const auto event = ParseEvent(is);
				db.Add(date, event);
			}
			else if (command == "Print") 
			{
				db.Print(cout);
			}
			else if (command == "Del") 
			{
				auto condition = ParseCondition(is);
				auto predicate = [condition](const Date& date, const string& event)
				{
					return condition->Evaluate(date, event);
				};
				
				int count = db.RemoveIf(predicate);
				cout << "Removed " << count << " entries" << endl;
			}
			else if (command == "Find") 
			{
				auto condition = ParseCondition(is);
				auto predicate = [condition](const Date& date, const string& event)
				{
					return condition->Evaluate(date, event);
				};

				const auto entries = db.FindIf(predicate);
				for (const auto& entry : entries) 
				{
					cout << entry << endl;
				}
				
				cout << "Found " << entries.size() << " entries" << endl;
			}
			else if (command == "Last") 
			{
				try 
				{
					cout << db.Last(ParseDate(is)) << endl;
				}
				catch (invalid_argument&) 
				{
					cout << "No entries" << endl;
				}
			}
			else if (command.empty()) 
			{
				continue;
			}
			else 
			{
				throw logic_error("Unknown command: " + command);
			}
			
		}
		catch (invalid_argument& ex)
		{
			cout << ex.what() << endl;
		}
		catch (range_error& ex)
		{
			cout << ex.what() << endl;
		}
		catch (logic_error& ex)
		{
			cout << ex.what() << endl;
		}
		catch (exception& ex)
		{
			cout << ex.what() << endl;
		}
	}
		
	return 0;
}

void TestParseEvent()
{
	{
		istringstream is("event");
		AssertEqual(ParseEvent(is), "event", "Parse event without leading spaces");
	}
	
	{
		istringstream is("   sport event ");
		AssertEqual(ParseEvent(is), "sport event ", "Parse event with leading spaces");
	}
	
	{
		istringstream is("  first event  \n  second event");
		vector<string> events;
		events.push_back(ParseEvent(is));
		events.push_back(ParseEvent(is));
		AssertEqual(events, vector<string>{"first event  ", "second event"}, "Parse multiple events");
	}
}

void TestAll()
{
	TestRunner tr;
	tr.RunTest(TestParseEvent, "TestParseEvent");
	tr.RunTest(TestParseCondition, "TestParseCondition");
}