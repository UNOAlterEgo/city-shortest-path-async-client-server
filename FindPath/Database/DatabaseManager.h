#pragma once
#include "../Database/UML/City.h"
#include <iostream>
#include <stdio.h> 
#include <map>
#include <mutex>          // std::mutex, std::unique_lock
#include <shared_mutex>
#include <fstream>

using namespace std;

class DatabaseManager
{
public:
	DatabaseManager(string cityFileName = "");
	DatabaseManager(DatabaseManager const&) = default;
	DatabaseManager& operator=(DatabaseManager const&) = default;
	// copy of cities in DB
	map<int, City> getCities() const;
	// returns copy of a city in DB
	City getCity(int id) const;
	// add city to the DB
	bool addCity(City& city);
	// update city in weight the DB
	bool updateCityWeight(int id, int weight);
	// update city in the DB
	bool updateCity(City& city);
	// delete city from the DB
	bool deleteCity(int id);
	// print DB
	friend ostream& operator<<(ostream& os, const DatabaseManager& database);

private:
	// DB file name
	string cityFileName = "cities.txt";
	// mutex
	mutable std::shared_mutex mutex_;
	// hashmap of cities in db
	map<int, City> cities;
	// initializes db
	bool initDB();
	// save db after editing
	bool save();
};