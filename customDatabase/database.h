#pragma once

#include <algorithm>
#include <map>
#include <set>
#include <string>
#include <vector>

#include "date.h"

class Database
{
public:
	void Add(const Date& date, const std::string& event);
	void Print(std::ostream& stream) const;
	
	template <class Predicate>
	int RemoveIf(Predicate predicate);

	template <class Predicate>
	std::vector<std::pair<Date, std::string>> FindIf(Predicate predicate) const;

	std::pair<Date, std::string> Last(const Date& date) const;

	friend std::ostream& operator<<(std::ostream& stream, const std::pair<Date, std::string>& value);
	friend bool operator==(const std::pair<Date, const std::string>& lhs, const std::pair<Date, const std::string>& rhs);

private:
	std::map<Date, std::set<std::string>> dataBaseSet;
	std::map<Date, std::vector<std::string>> dataBaseVector;
};


template <class Predicate>
int Database::RemoveIf(Predicate predicate)
{
	int count = 0;

	auto dt = dataBaseVector.begin();

	while (dt != dataBaseVector.end())
	{
		auto date = dt->first;

		auto startDel = std::stable_partition(dt->second.begin(), dt->second.end(),
		[date, predicate, this](const std::string& event)
		{
			bool res = predicate(date, event);

			if (res)
			{
				dataBaseSet[date].erase(event);
			}

			return !res;
		});

		count += dt->second.end() - startDel;
		dt->second.erase(startDel, dt->second.end());

		if (dt->second.empty())
			dt = dataBaseVector.erase(dt);
		else
			++dt;
	}

	return count;
}

template <class Predicate>
std::vector<std::pair<Date, std::string>> Database::FindIf(Predicate predicate) const
{
	std::vector<std::pair<Date, std::string>> response;

	for (const auto& dt : dataBaseVector)
	{
		for (const auto& ev : dt.second)
		{
			if (predicate(dt.first, ev))
				response.emplace_back(dt.first, ev);
		}
	}

	return response;
}
