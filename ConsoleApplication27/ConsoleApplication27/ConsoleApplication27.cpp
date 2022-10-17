#include <iostream>
#include <string>
#include <sstream>
#include <map>
#include <set>
#include <vector>
#include <iomanip>
#include <algorithm>

class Date {
public:
	Date(std::stringstream& jigoku, const std::string& mode) {
		int yer{ 21458 }, mons{ 21458 }, dei{ 21458 };
		std::stringstream jigoku_2{ jigoku.str() };
		std::string g1, g2;
		jigoku_2 >> g1 >> g2;
		pedrilla = g2;

		jigoku >> yer;
		if (jigoku.peek() == '-') {
			jigoku.ignore(1);
			jigoku >> mons;

			if (jigoku.peek() == '-') {
				jigoku.ignore(1); // proverit vvodi cherez huinyu
				jigoku >> dei;
			}

			else {
				throw std::invalid_argument(g2);
			}
		}

		else {
			throw std::invalid_argument(g2);
		}
		if (mode == "Add") {
			if (jigoku.peek() != ' ') {
				throw std::invalid_argument(g2);
			}
		}
		if (yer == 21458 || mons == 21458 || dei == 21458) { // mojet vyvoditsa 0, huyevo budet

			throw std::invalid_argument(g2);
		}
		// validnost mons, dei +
		year = yer;

		if (mons <= 0 xor mons >= 13) {
			throw std::runtime_error(std::to_string(mons));
		}

		else if (dei <= 0 xor dei >= 32) {
			throw std::domain_error(std::to_string(dei));
		}

		else {
			month = mons;
			day = dei;
		}
	}

	int GetYear() const {
		return year;
	}

	int GetMonth() const {
		return month;
	}

	int GetDay() const {
		return day;
	}

	std::string gC() const {
		return pedrilla;
	}
private:
	int year{ 0 }, month{ 0 }, day{ 0 };
	std::string pedrilla;
};

bool operator<(const Date& lhs, const Date& rhs) {
	if (lhs.GetYear() != rhs.GetYear()) {
		return lhs.GetYear() < rhs.GetYear();
	}

	else {
		if (lhs.GetMonth() != rhs.GetMonth()) {
			return lhs.GetMonth() < rhs.GetMonth();
		}

		else {
			return lhs.GetDay() < rhs.GetDay();
		}
	}

}

class Database {
public:
	void AddEvent(const Date& date, const std::string& evento) {
		real_db[date].insert(evento);
	}

	void DeleteEvent(const Date& date, const std::string& evento) {
		if (std::count(real_db[date].begin(), real_db[date].end(), evento)) {
			real_db[date].erase(evento);
			std::cout << "Deleted successfully" << '\n';
		}

		else {
			std::cout << "Event not found" << '\n';
		}
	}

	void  DeleteDate(const Date& date) {
		std::cout << "Deleted " << real_db[date].size() << " events" << '\n';
		real_db[date].clear();
	}

	void Find(const Date& date) const {
		if (real_db.count(date)) {
			for (const auto& i : real_db.at(date)) {
				std::cout << i << '\n';
			}
		}
	}

	void Print() const {
		for (const auto& i : real_db) {
			if (i.first.GetYear() >= 0) {
				for (const auto& j : i.second) {
					std::cout << std::setw(4) << std::setfill('0') << i.first.GetYear() << '-'
						<< std::setw(2) << std::setfill('0') << i.first.GetMonth() << '-'
						<< std::setw(2) << std::setfill('0') << i.first.GetDay() << ' ';
					std::cout << j << '\n';
				}
			}
		}
	}

private:
	std::map<Date, std::set<std::string>> real_db;
};

/*
	Koroche, ostalos:
	Print;
	Udalenie DeleteDate ispravit;
	proverit kakiye tam esche exceptions na vsyak
	yesli prinimat ne budet to smotret naverh v Date konstrukte.
*/
int main() {
	Database db;

	std::string command;

	while (std::getline(std::cin, command)) {
		if (command.size() > 0) {
			try {
				std::stringstream jigoku{ command };
				std::string oper_ru, evento;
				jigoku >> oper_ru;

				if (oper_ru == "Add") {
					Date kok_t{ jigoku, "Add" };
					jigoku >> evento;
					db.AddEvent(kok_t, evento);
				}

				else if (oper_ru == "Del") {
					Date kok_t{ jigoku, "Del" };
					if (!(jigoku >> evento)) { // detektit
						db.DeleteDate(kok_t);
					}

					else {
						//jigoku >> evento;
						db.DeleteEvent(kok_t, evento);
					}
				}

				else if (oper_ru == "Find") {
					Date kok_t{ jigoku, "Find" };
					db.Find(kok_t);
				}

				else if (oper_ru == "Print") {
					db.Print();
				}

				else {
					std::cout << "Unknown command: " << oper_ru << '\n';
				}
			}
			catch (std::invalid_argument& e) {
				std::cout << "Wrong date format: " << e.what() << '\n';
			}

			catch (std::domain_error& e) {
				std::cout << "Day value is invalid: " << e.what() << '\n';
			}

			catch (std::runtime_error& e) {
				std::cout << "Month value is invalid: " << e.what() << '\n';
			}
		}
	}

	return 0;
}