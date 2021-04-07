#pragma once

#include <sstream>
#include <stdexcept>
#include <iostream>
#include <map>
#include <set>
#include <string>
#include <vector>

using namespace std;

template <class T>
ostream& operator << (ostream& os, const vector<T>& s)
{
	os << "{";
	bool first = true;
	
	for (const auto& x : s) 
	{
		if (!first) 
		{
			os << ", ";
		}
		
		first = false;
		os << x;
	}
	
	return os << "}";
}

template <class T>
ostream& operator << (ostream& os, const set<T>& s)
{
	os << "{";
	bool first = true;
	
	for (const auto& x : s) 
	{
		if (!first) 
		{
			os << ", ";
		}
		
		first = false;
		os << x;
	}
	
	return os << "}";
}

template <class K, class V>
ostream& operator << (ostream& os, const map<K, V>& m)
{
	os << "{";
	bool first = true;
	
	for (const auto& kv : m) 
	{
		if (!first) 
		{
			os << ", ";
		}
		
		first = false;
		os << kv.first << ": " << kv.second;
	}
	
	return os << "}";
}

template<class T, class U>
void AssertEqual(const T& t, const U& u, const string& hint = {})
{
	if (!(t == u)) 
	{
		ostringstream os;
		os << "Assertion failed: " << t << " != " << u;
		
		if (!hint.empty()) 
		{
			os << " hint: " << hint;
		}
		
		throw runtime_error(os.str());
	}
}

inline void Assert(bool b, const string& hint)
{
	AssertEqual(b, true, hint);
}

class TestRunner
{
public:
	template <class TestFunc>
	void RunTest(TestFunc func, const string& testName);

	~TestRunner();

private:
	int failCount = 0;
};

inline TestRunner::~TestRunner()
{
	if (failCount > 0)
	{
		cerr << failCount << " unit tests failed. Terminate" << endl;
		exit(1);
	}
}

template <class TestFunc>
void TestRunner::RunTest(TestFunc func, const string& testName)
{
	try
	{
		func();
		cerr << testName << " OK" << endl;
	}
	catch (exception& e)
	{
		++failCount;
		cerr << testName << " fail: " << e.what() << endl;
	}
	catch (...)
	{
		++failCount;
		cerr << "Unknown exception caught" << endl;
	}
}

#define ASSERT_EQUAL(x, y) {            \
  ostringstream os;                     \
  os << #x << " != " << #y << ", "      \
    << __FILE__ << ":" << __LINE__;     \
  AssertEqual(x, y, os.str());          \
}

#define ASSERT(x) {                     \
  ostringstream os;                     \
  os << #x << " is false, "             \
    << __FILE__ << ":" << __LINE__;     \
  Assert(x, os.str());                  \
}

#define RUN_TEST(tr, func) \
  tr.RunTest(func, #func)