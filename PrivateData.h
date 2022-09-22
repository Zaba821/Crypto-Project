/*
Author:  Emil Kosic
Date:    04/30/2022
File:    PrivateData.h
Descr:
Specification file for the Private Data class.
*/

#include <string>

using namespace std;

#ifndef STUDENT_H  //conditional compilation (aka include guard)
#define STUDENT_H
// class declaration
class Private {
private:
	string name;    // private data members
	int ssn;
	string address;
	string city;
	string state;
	int zipCode;
	int birthDay;
	int birthMonth;
	int birthYear;
	int totalIncome;
public:
	Private(); // default constructor
	//Private(string, int, int); // parameter constructor
	~Private(); // destructor
	void setName(string);   // public mutators (aka setters)
	void setBday(int);
	void setBMonth(int);
	void setBYear(int);
	void setAddress(string);
	void setCity(string);
	void setState(string);
	void setZip(int);
	void setSSN(int);
	void setIncome(int);
	
	int getFedIncome(); // calculation (also getter)
	
	string getName() const { return name; } // public accessors (aka getters) inline
	int getBDay() const { return birthDay; }
	int getBMonth() const { return birthMonth; }
	int getBYear() const { return birthYear;  }
	int getSSN() const { return ssn; }
	string getAddress() const { return address;  }
	string getCity() const { return city;  }
	string getState() const { return state; }
	
};
#endif