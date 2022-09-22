/*
Author:  Emil Kosic
Date:    04/25/2022
File:    PrivateData.cpp
Descr:
Implementation file for the Private class.
*/
#include <iostream>
using namespace std;
#include "privateData.h"

// constructors
Private::Private() {
	name = "", address = "", city = "", state = "";
	birthDay = 0, birthMonth = 0, birthYear = 0;
	ssn = 0, zipCode = 0, totalIncome = 0;
	
}


// destructor
Private::~Private() {
	
}
// mutator (aka setter methods)
void Private::setName(string n) {
	name = n;
}

void Private::setBday(int n) {
	birthDay = n;
}

void Private::setBMonth(int n) {
	birthMonth = n;
}

void Private::setBYear(int n) {
	birthYear = n;
}

void Private::setAddress(string n) {
	address = n;
}

void Private::setCity(string n) {
	city = n;
}

void Private::setState(string n) {
	state = n;
}

void Private::setZip(int n) {
	zipCode = n;
}

void Private::setSSN(int n) {
	ssn = n;
}

void Private::setIncome(int n) {
	totalIncome = n;
}

// calculation methods
int Private::getFedIncome() {
	return totalIncome * 0.68;
}
