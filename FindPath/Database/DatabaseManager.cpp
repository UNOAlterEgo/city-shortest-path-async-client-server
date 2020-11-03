#include "DatabaseManager.h"

DatabaseManager::DatabaseManager(string cityFileName)
{
	if (cityFileName != "")
	{
		this->cityFileName = cityFileName;
	}
	initDB();
}

map<int, City> DatabaseManager::getCities() const
{
	shared_lock<shared_mutex> lock(mutex_);
	return cities;
}

City DatabaseManager::getCity(int id) const
{
	shared_lock<shared_mutex> lock(mutex_);
	auto it = cities.find(id);
	if (it != cities.end())
	{
		return it->second;
	}

	return City();
}

bool DatabaseManager::addCity(City& city)
{
	// add city to DB	
	unique_lock<shared_mutex> lock(mutex_);
	auto it = cities.find(city.id);
	if (it == cities.end())
	{
		cities[city.id] = city;
		return save();
	}
	else
	{
		cout << "City  with given id already exists: " << it->second << endl;
	}

	return false;
}

bool DatabaseManager::updateCityWeight(int id, int weight)
{
	unique_lock<shared_mutex> lock(mutex_);
	// update city weight in DB
	auto it = cities.find(id);
	if (it != cities.end())
	{
		it->second.weight = weight;
		return save();
	}
	else
	{
		cout << "City with given id doesn't exist: " << it->second << endl;
		return false;;
	}

	return false;
}

bool DatabaseManager::updateCity(City& city)
{
	unique_lock<shared_mutex> lock(mutex_);
	// update city in DB
	auto it = cities.find(city.id);
	if (it != cities.end())
	{
		it->second = city;
		return save();
	}
	else
	{
		cout << "City with given id doesn't exist: " << it->second << endl;
		return false;;
	}

	return false;
}

bool DatabaseManager::deleteCity(int id)
{
	// delete city to DB	
	unique_lock<shared_mutex> lock(mutex_);
	auto it = cities.find(id);
	if (it != cities.end())
	{
		cities.erase(id);
		return save();
	}

	return false;
}

bool DatabaseManager::save()
{
	ofstream outfile(cityFileName);
	// File doesn't exist
	if (outfile.fail())
	{
		cout << "File " << cityFileName << " does not exist." << endl;
		return false;
	}

	// Write values in map to file;
	for (auto it = cities.begin(); it != cities.end(); ++it)
	{
		outfile << it->second << endl;
	}

	outfile.close();

	return true;
}

bool DatabaseManager::initDB() {
	unique_lock<shared_mutex> lock(mutex_);

	ifstream infile(cityFileName);

	// File doesn't exist
	if (infile.fail())
	{
		cout << "File " << cityFileName << " does not exist." << endl;
		// create file if it doesn't exist
		ofstream outfile(cityFileName);
		if (outfile.fail())
			return false;
		outfile.close();
	}

	// Create a map of <city.id, city>
	City city;
	while (infile >> city)
	{
		cities[city.id] = city;
		// successfully extracted one line
	}

	infile.close();
	cout << "Done." << endl << endl;

	return true;
}

ostream& operator<<(ostream& os, const DatabaseManager& city)
{
	shared_lock<shared_mutex> lock(city.mutex_);
	for (auto it = city.cities.begin(); it != city.cities.end(); ++it)
	{
		os << it->second << endl;
	}
	return os;
}