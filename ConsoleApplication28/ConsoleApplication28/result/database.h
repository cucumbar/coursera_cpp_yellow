#pragma once
#include "date.h"

#include <algorithm>
#include <string>
#include <set>
#include <vector>
using namespace std;

class EventSet {
public:
	void Add(const string& event);

	const vector<string>& GetAll() const;

	template <typename Predicate>
	int RemoveIf(Predicate predicate) {
		auto split_point = stable_partition(event_order_.begin(), event_order_.end(), predicate);
		int result = split_point - event_order_.begin();
		for (auto i = event_order_.begin(); i != split_point; ++i) {
			events_.erase(*i);
		}
		event_order_.erase(event_order_.begin(), split_point);

		return result;
	}

private:
	set<string> events_;
	vector<string> event_order_;
};


#include <iostream>
#include <map>

struct Entry {
    Date date;
    string event;
};

ostream &operator<<(ostream &os, const Entry &e);

bool operator==(const Entry &lhs, const Entry &rhs);

bool operator!=(const Entry &lhs, const Entry &rhs);

class Database {
public:
    void Add(const Date &date, const string &event); // add

    void Print(ostream &os) const; // print

    template<typename Predicate>
    vector<Entry> FindIf(Predicate predicate) const {
        vector<Entry> result;
        for (auto& kv : storage) {
            for (const auto& event : kv.second.GetAll()) {
                if (predicate(kv.first, event)) {
                    result.push_back(Entry{kv.first, event});
                }
            }
        }
        return result;
    }

    template <typename Predicate>
    int RemoveIf(Predicate predicate) {
        int result = 0;
        for (auto& kv : storage) {
            const Date& date = kv.first;
            result += kv.second.RemoveIf([=](const string& event) {
                return predicate(date, event);
            });
        }
        for (auto it = storage.begin(); it != storage.end(); ) {
            if (it->second.GetAll().empty()) {
                storage.erase(it++);
            } else {
                ++it;
            }
        }
        return result;
    }

    Entry Last(const Date &date) const;

private:
    map<Date, EventSet> storage;
};

ostream &operator<<(ostream &stream, const pair<Date, string> &p);

void TestDatabaseAddAndPrint();

void TestDatabaseFind();

void TestDatabaseRemove();

void TestDatabaseLast();

