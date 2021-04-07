#include <iomanip>
#include <sstream>

#include "date.h"

using namespace std;

int Date::GetYear() const
{
	return year; 
}

int Date::GetMonth() const
{
	return month;
}

int Date::GetDay() const
{
	return day;
}

void Date::SetYear(const int year)
{
	this->year = year;
}

void Date::SetMonth(const int month)
{
	if (month > 12 || month < 1)
	{
		stringstream ss;
		ss << "Month value is invalid: " << month ;
		throw range_error(ss.str());
	}

	this->month = month;
}

void Date::SetDay(const int day)
{
	if (day > 31 || day < 1)
    {
		std::stringstream ss;
		ss << "Day value is invalid: " << day;
		throw invalid_argument(ss.str());
    }
	
	this->day = day;
}

ostream& operator<<(ostream& stream, const Date& date)
{
	stream << setfill('0') << setw(4) << date.year << '-' << setw(2) << date.month << '-' << setw(2) << date.day;
	return stream;
}

bool operator<(const Date& lhs, const Date& rhs)
{
	return tuple<int, int, int>(lhs.year, lhs.month, lhs.day) < tuple<int, int, int>(rhs.year, rhs.month, rhs.day);
}

bool operator<=(const Date& lhs, const Date& rhs)
{
	return tuple<int, int, int>(lhs.year, lhs.month, lhs.day) <= tuple<int, int, int>(rhs.year, rhs.month, rhs.day);
}

bool operator>(const Date& lhs, const Date& rhs)
{
	return tuple<int, int, int>(lhs.year, lhs.month, lhs.day) > tuple<int, int, int>(rhs.year, rhs.month, rhs.day);
}

bool operator>=(const Date& lhs, const Date& rhs)
{
	return tuple<int, int, int>(lhs.year, lhs.month, lhs.day) >= tuple<int, int, int>(rhs.year, rhs.month, rhs.day);
}

bool operator==(const Date& lhs, const Date& rhs)
{
	return tuple<int, int, int>(lhs.year, lhs.month, lhs.day) == tuple<int, int, int>(rhs.year, rhs.month, rhs.day);
}

bool operator!=(const Date& lhs, const Date& rhs)
{
	return tuple<int, int, int>(lhs.year, lhs.month, lhs.day) != tuple<int, int, int>(rhs.year, rhs.month, rhs.day);
}

Date ParseDate(istream& is)
{
	int year, month, day;
	char ch1, ch2;

	auto r1 = is.eof();
	auto r2 = is.fail();
	
	is >> year >> ch1 >> month >> ch2 >> day;

	if(is.fail() || ch1 != '-' || ch2 != '-')
	{
		std::stringstream ss;
		ss << "Wrong date format: " << is.tellg();
		throw invalid_argument(ss.str());
	}

	Date date{};

	date.SetYear(year);
	date.SetMonth(month);
	date.SetDay(day);

	return date;
}