#include "Address.h"
#include <vector>
#include "utils.h"
#include "Exceptions.h"

Address::Address() {

}

Address::~Address() {

}


bool Address::parse(string str) { 
	vector<string> parts = utils::split(str, '/');
	for (auto &part : parts) utils::trim(part);

	if (parts.size() == 7) {
		district = parts.at(0);
		if (!isString(district))
			return false;

		town = parts.at(1);
		if (!isString(town))
			return false;

		street = parts.at(2);
		if (!isString(street))
			return false;

		number = stoi(parts.at(3));

		if (!isNumber(parts.at(4))) floor = -1;

		else floor = stoi(parts.at(4));

		latitude = stod(parts.at(5));

		longitude = stod(parts.at(6));

		return true;
	}

	else return false;
}

bool Address::parseInsideBase(string str, string dist) { 
	vector<string> parts = utils::split(str, '/');
	for (auto &part : parts) utils::trim(part);

	if (parts.size() == 6) {
		district = dist;
		if (!isString(district))
			return false;

		town = parts.at(0);
		if (!isString(town))
			return false;

		street = parts.at(1);
		if (!isString(street))
			return false;

		number = stoi(parts.at(2));

		if (!isNumber(parts.at(3))) floor = -1;

		else floor = stoi(parts.at(3));

		latitude = stod(parts.at(4));

		longitude = stod(parts.at(5));

		return true;
	}

	else return false;
}

string Address::str() const {
	string result = district + " / ";
	result += town + " / ";
	result += street + " / ";
	result += to_string(number) + " / ";
	result += to_string(floor) + " / ";
	result += to_string(latitude) + " / ";
	result += to_string(longitude) + " / ";
	return result;
}


void Address::setTown(string t) {
	town = t;
}

void Address::setDistrict(string d) {
	district = d;
}

void Address::setStreet(string s) {
	street = s;
}

void Address::setNumber(unsigned int n) {
	number = n;
}

void Address::setFloor(int f) {
	floor = f;
}


string Address::get_town() const {
	return town;
}

string Address::get_district() const {
	return district;
}

string Address::get_street() const {
	return street;
}

unsigned int Address::get_number() const {
	return number;
}

int Address::get_floor() const {
	return floor;
}


std::ostream & operator<<(std::ostream & stream, Address address) {
	stream << address.district << " / ";
	stream << address.town << " / ";
	stream << address.street << " / ";
	stream << address.number << " / ";
	stream << address.floor << " / ";
	/*
	if (address.floor != -1) stream << address.floor << " / ";
	else stream << "Ground Floor" << " / ";
	*/
	stream << address.latitude << " / ";
	stream << address.longitude;

	return stream;
}