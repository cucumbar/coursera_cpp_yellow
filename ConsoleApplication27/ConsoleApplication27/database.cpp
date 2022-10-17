#include "database.h"
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

void EventSet::Add(const string& event) {
	auto insert_result = events_.insert(event);
	if (insert_result.second) {
		event_order_.push_back(event);
	}
}

const vector<string>& EventSet::GetAll() const {
	return event_order_;
}


void Database::Add(const Date &date, const string &event) {
	storage[date].Add(event);
}

void Database::Print(ostream &os) const {
	for (const auto &item : storage) {
		for (const auto &event : item.second.GetAll()) {
			os << item.first << " " << event << endl;
		}
	}
}

Entry Database::Last(const Date &date) const {
	auto it = storage.upper_bound(date);

	if (it == storage.begin()) {
		throw invalid_argument("");
	}
	--it;
	return { it->first, it->second.GetAll().back() };
}


ostream &operator<<(ostream &stream, const Entry &e) {
	return stream << e.date << ' ' << e.event;
}

bool operator==(const Entry &lhs, const Entry &rhs) {
	return tie(lhs.date, lhs.event) == tie(rhs.date, rhs.event);
}

bool operator!=(const Entry &lhs, const Entry &rhs) {
	return tie(lhs.date, lhs.event) != tie(rhs.date, rhs.event);
}