#pragma once
#include <string>
#include <iostream>

using namespace std;

struct City
{
	City();
	City(int id, string name, int weight);
	int id;
	int weight;
	string name;
	bool operator==(const City& rhs);
	friend ostream& operator<<(ostream& os, const City& city);
	friend istream& operator>>(std::istream& is, City& city);
};