#include "database.h"

using namespace std;

void Database::Add(const Date& date, const string& event)
{
	if(dataBaseSet[date].count(event) == 0)
	{
		dataBaseSet[date].insert(event);
		dataBaseVector[date].push_back(event);
	}
}

void Database::Print(ostream& stream) const
{
	for(const auto& [date, events] : dataBaseVector)
	{
		for(const auto& event : events)
		{
			stream << date << " " << event << "\n";
		}
	}
}

pair<Date, string> Database::Last(const Date& date) const
{
	if(dataBaseVector.empty())
		throw range_error("No entries");
	
	if(date < (*dataBaseVector.begin()).first)
		throw range_error("No entries");
	
	auto dateEvent = dataBaseVector.upper_bound(date);
	dateEvent = prev(dateEvent);

	return { dateEvent->first, *dateEvent->second.rbegin() };
}
