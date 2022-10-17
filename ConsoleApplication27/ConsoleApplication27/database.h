#pragma once
#include "date.h"

class Database {
public:
	void AddEvent(const Date& date, const std::string& evento);

	void DeleteEvent(const Date& date, const std::string& evento);

	void DeleteDate(const Date& date);
	void Find(const Date& date) const;

	void Print() const;

private:
	std::map<Date, std::set<std::string>> real_db;
};