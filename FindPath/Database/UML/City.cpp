#include "City.h"


City::City() : id(-1), name(""), weight(-1) {}

City::City(int id, string name, int weight) : id(id), name(name), weight(weight) {}

bool City::operator==(const City& rhs)
{
	return id == rhs.id;
}

ostream& operator<<(ostream& os, const City& city)
{
	os << city.id << ',' << city.name << ',' << city.weight;
	return os;
}

istream& operator>>(std::istream& is, City& city)
{
    std::string id, weight;
    if (getline(is, id, ',') &&
        getline(is, city.name, ',') &&
        getline(is, weight))
    {
        city.id = std::stoi(id);
        city.weight = std::stoi(weight);
    }

    return is;
}
