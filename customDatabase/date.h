#pragma once
#include <istream>

class Date
{
public:

	Date() = default;
	Date(const int year, const int month, const int day) : year(year), month(month), day(day) {}

	// GET
	[[nodiscard]] int GetYear() const;

	[[nodiscard]] int GetMonth() const;

	[[nodiscard]] int GetDay() const;

	// SET
	void SetYear(const int year);

	void SetMonth(const int month);

	void SetDay(const int day);
	
	// OPERATIONS

	friend std::ostream& operator<< (std::ostream& stream, const Date& date);
	friend bool operator<(const Date& lhs, const Date& rhs);
	friend bool operator<=(const Date& lhs, const Date& rhs);
	friend bool operator>(const Date& lhs, const Date& rhs);
	friend bool operator>=(const Date& lhs, const Date& rhs);
	friend bool operator==(const Date& lhs, const Date& rhs);
	friend bool operator!=(const Date& lhs, const Date& rhs);

private:
	int year;
	int month;
	int day;
};

Date ParseDate(std::istream& is);