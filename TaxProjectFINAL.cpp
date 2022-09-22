/*
Author: Emil Kosic
File: TaxProjectFINAL.cpp
Date: 4-30-22
Description: A program that takes into user information for crypto investments
and calculates how much would be taxed form.
*/

// Include stuff
#include "PrivateData.h" // for privateData class
#include <iostream>
#include <iomanip>
#include <fstream>
#include <string>
#include <vector>
using namespace std;
// Enumerated Data Type for *userData*

enum months { // Used for date calculation
    jan = 1, feb = 2, mar = 3, apr = 4, may = 5, june = 6,
    july = 7, aug = 8, sept = 9, octo = 10, nov = 11, dece = 12
};
// Structure 

struct csvData { // Variable that holds specific user data
    string assetName = ""; int receivedMonth = 0; int receivedDay = 0;
    int receivedYear = 0; double purchasePrice = 0;
    int soldMonth = 0; int soldDay = 0; int soldYear = 0;
    double soldPrice = 0;
    months mts;
};

struct form8949Data { // variable that holds 8949 data variables
    string assetName = ""; int receivedMonth = 0; int receivedDay = 0;
    int receivedYear = 0; double purchasePrice = 0; int soldMonth = 0;
    int soldDay = 0; int soldYear = 0; double soldPrice = 0;
    double gainOrLoss = 0;
};

// PROTOTYPES (ALOT)
int getFromFile(string file_name, vector<csvData>& csv_Vector); // FUNCTION TO READ FROM FILE
void displayVector(const vector<csvData> vector_array); // DEBUG FUNCTION 
void calculateGainLength(const vector<csvData>csv_vector, vector<csvData>&csv_shortvector, // 8949 RELATED FUNCTIONS**
    vector<csvData>&csv_longvector);
void calculate8949(vector<csvData>csv_shortvector, vector<csvData>& csv_longvector,
    vector<form8949Data>& form_8489short, vector<form8949Data>& form_8489long);
void menuDisplayFunction(); // MENU FUNCTIONS**
void menuDisplayFunctionTwo(void* calculate89, vector<csvData>csv_short, vector<csvData>csv_long,
    vector<form8949Data>form_short, vector<form8949Data>form_long);
Private GetPrivData(); // CLASS FUNCTION
void DisplayPrivData(Private s); // CLASS FUNCTION
int* createArray(int size); // POINTER FUNCTION
int totalFunction(int* forms8949, const int size); // POINTER FUNCTION

int main() {
    //vectors 
    csvData c;
    vector<csvData> shortGainData; vector<csvData> longGainData; vector<csvData> csvData; // csv data vectors
    vector<form8949Data> short8949; vector<form8949Data> long8949; // form 8949 vectors
    int calculateSize = 2; // pointer array size (only 2 8949s possible)
    fstream inFile;
    string fileNameOne;
    Private st1 = Private();
    cout << "Enter 2nd Data set" << endl;
    Private st2 = Private();
     // ENTER DATA
    menuDisplayFunction();
    st1 = GetPrivData();
    st2 = GetPrivData();
    cout << "Enter CSV file name: ";
    getline(cin, fileNameOne);
    getFromFile(fileNameOne, csvData);
    cout << endl << endl;
    calculateGainLength(csvData, shortGainData, longGainData);
    menuDisplayFunctionTwo(&calculate8949, shortGainData, longGainData, short8949, long8949);
    cout << "****Private Data****" << endl;
    DisplayPrivData(st1);
    DisplayPrivData(st2);

    int* total8949 = createArray(calculateSize); // Create a pointer for students and then inputting it into the function
    cout << "****Total 8949 amount****" << endl;
    cout << totalFunction(total8949, calculateSize);
    
// displayVector(csvData); // display vector
//displayVector(shortGainData);
//displayVector(longGainData);
}
// getFromFile
// Function: Open File
// Parameters: string filename 
// Returns: count
// Description: gets all data from the CSV file
int getFromFile(string file_name, vector<csvData>& csv_Vector)
{
    ifstream inputFile;
    inputFile.open(file_name);

    if (inputFile.fail()) {
        cout << "Error opening file" << endl;
        exit(-2);
    }
        else
    {
        string tempData; // stores temp data to convert from string to int
        csvData tempcsvData; // makes the variable for movieData
        getline(inputFile, tempData, '\n');

        while (!inputFile.eof()) {
              inputFile >> tempcsvData.assetName; getline(inputFile, tempData, '/');
            tempcsvData.receivedMonth = stoi(tempData); getline(inputFile, tempData, '/');
            tempcsvData.receivedDay = stoi(tempData); inputFile >> tempData;
            tempcsvData.receivedYear = stoi(tempData); inputFile >> tempData;
            tempcsvData.purchasePrice = stod(tempData); getline(inputFile, tempData, '/');
            tempcsvData.soldMonth = stoi(tempData); getline(inputFile, tempData, '/');
            tempcsvData.soldDay = stoi(tempData); inputFile >> tempData;
            tempcsvData.soldYear = stoi(tempData); inputFile >> tempData;
            tempcsvData.soldPrice = stod(tempData);

             csv_Vector.push_back(tempcsvData);
         }
        inputFile.close();
        return csv_Vector.size();
    }
}

// Function: displayVector
// Parameters: const vector
// Returns: the entire vector
// Description:
//  Displays the entire vector when called
// (USED FOR TESTING)
void displayVector(const vector<csvData> vector_array) {
    cout << "running";
    for (int i = 0; i < vector_array.size(); i++) {

        cout << "Assest Name : " << vector_array[i].assetName; cout << " Month : " << vector_array[i].receivedMonth;
        cout << " Day: " << vector_array[i].receivedDay; cout << " Year: " << vector_array[i].receivedYear;
        cout << " Purchase Price: " << vector_array[i].purchasePrice; cout << " Month: " << vector_array[i].soldMonth;
        cout << " Day: " << vector_array[i].soldDay; cout << " Year: " << vector_array[i].soldYear;
        cout << " Sold Price: " << vector_array[i].soldPrice; cout << endl;
    }
}
// Function: calculateGainLength
// Parameters: vector 1, vector 2, vector 3 
// Returns: 2 seperate vectors 
// Description: gets all data from the main CSV data vector and calculates wether it is
// long or short term capital gains. 

void calculateGainLength(const vector<csvData>csv_vector,vector<csvData>&csv_shortvector,
    vector<csvData>&csv_longvector)  {
    int scout = 0;
    for (int i = 0; i < csv_vector.size(); i++) {

        int tempMonthR = 0; int tempDayR = 0; int tempYearR = 0;
        int tempMonthS = 0; int tempDayS = 0; int tempYearS = 0;
        int totalR = 0; int totalS = 0; int year = 365;
        int totalTime; csvData tempcsvData;
        
         // display appropriate class // ENUM STUFF
         // received month sorter
        switch (csv_vector[i].receivedMonth) {
        case jan: tempMonthR = 31; break;
        case feb: tempMonthR = 28; break;
        case mar: tempMonthR = 31; break;
        case apr: tempMonthR = 30; break;
        case may: tempMonthR = 31; break;
        case june:tempMonthR = 30; break;
        case july:tempMonthR = 31; break;
        case aug:tempMonthR = 30; break;
        case sept:tempMonthR = 30; break;
        case octo:tempMonthR = 31; break;
        case nov:tempMonthR = 30; break;
        case dece:tempMonthR = 31; break;
        }
        // sold month sorter
        switch (csv_vector[i].soldMonth) {
        case jan: tempMonthS = 31; break;
        case feb: tempMonthS = 28; break;
        case mar: tempMonthS = 31; break;
        case apr: tempMonthS = 30; break;
        case may: tempMonthS = 31; break;
        case june: tempMonthS = 30; break;
        case july: tempMonthS = 31; break;
        case aug: tempMonthS = 31; break;
        case sept: tempMonthS = 30; break;
        case octo: tempMonthS = 31; break;
        case nov: tempMonthS = 30; break;
        case dece: tempMonthS = 31; break;
        }
        // Day Calculation
        tempDayR = csv_vector[i].receivedDay;
        tempDayS = csv_vector[i].soldDay;

        // Year Calculation
        tempYearR = csv_vector[i].receivedYear;
        tempYearS = csv_vector[i].soldYear;

        totalR = (tempYearR * year) + tempDayR + tempMonthR;
        totalS = (tempYearS * year) + tempDayS + tempMonthS;

        totalTime = totalS - totalR;
        int lcout = 0;
         // Sort into whichever vector
        if (totalTime <= year) {
            scout++;
            csv_shortvector.push_back(csv_vector[i]); // pushback shortDataVector
           }
        else {
            csv_longvector.push_back(csv_vector[i]);
               
             }
    }
 }
// Function:   calculate8949
// Parameters: both csv vectors, both form8949 vectors
// Returns:    nothin
// Descr:
// This calculates everything from the vectors and displays it as how it is on a 
// real 8949 form (used my real one as reference and checking)
void calculate8949(vector<csvData>csv_shortvector, vector<csvData>& csv_longvector,
    vector<form8949Data>& form_8489short, vector<form8949Data>& form_8489long) {
    form8949Data tempcsvData;
    int ttlPShort = 0, ttlSShort = 0, ttlPLong = 0, ttlSLong = 0;
    int ttlGainShort = 0, ttlGainLong = 0;
    // int check = 0; // function to see if the vectors even contain data
    if (csv_shortvector.size() > 0) {
        for (int i = 0; i < csv_shortvector.size(); i++) {
            string assetN = csv_shortvector[i].assetName;
            int receivedD = csv_shortvector[i].receivedDay;
            int receivedM = csv_shortvector[i].receivedMonth;
            int receivedY = csv_shortvector[i].receivedYear;
            double purchaseP = csv_shortvector[i].purchasePrice;
            int soldD = csv_shortvector[i].soldDay;
            int soldM = csv_shortvector[i].soldMonth;
            int soldY = csv_shortvector[i].soldYear;
            double soldP = csv_shortvector[i].soldPrice;
            double gainLoss = soldP - purchaseP;
            ttlPShort = ttlPShort + purchaseP;
            ttlSShort = ttlSShort + soldP;
            ttlGainShort = ttlSShort - ttlPShort;

            tempcsvData.assetName = assetN;
            tempcsvData.receivedDay = receivedD; tempcsvData.receivedMonth = receivedM;
            tempcsvData.receivedYear = receivedY; tempcsvData.purchasePrice = purchaseP;
            tempcsvData.soldDay = soldD; tempcsvData.soldMonth = soldM; tempcsvData.soldYear = soldY;
            tempcsvData.soldPrice = soldP; tempcsvData.gainOrLoss = gainLoss;

            form_8489short.push_back(tempcsvData);
      }

        cout << endl << endl << endl;
        cout << "Enter this information of form 8949 sheet (page one)." << endl;
        cout << "_____________________________________________________" << endl;
        cout << "Check box C above table" << endl << endl;

        cout << setw(13) << "(a)" << setw(21) << "(b)" << setw(16) << "(c)" << setw(16) << "(d)" <<
            setw(13) << "(e)" << setw(18) << "(f)" << setw(17) << "(g)" << setw(18) << "(h)" << endl;
       
        cout << "Description of property" << setw(15) << "Date Aquired" << setw(15) << "Date Sold" << setw(15) <<
            "Proceeds" << setw(16) << "Cost or other" << setw(16) << "Codes from" <<
            setw(16) << "Amount of" << setw(21) << "Gain or Loss" << endl;
        cout << setw(80) << "basis" << setw(21) << "instructions" << setw(16) << "adjustment" << endl;
      
        cout << "_______________________" << setw(15) << "____________" << setw(15) << "__________" << setw(15) <<
            "________" << setw(16) << "_____________" << setw(18) << "______________" <<
            setw(16) << "____________" << setw(19) << "____________" << endl << endl;
        
        for (int i = 0; i < form_8489short.size(); i++) {
            
            cout <<  setw(13) << form_8489short[i].assetName;
            cout << setw(16) << form_8489short[i].receivedMonth << "/";
            if (form_8489short[i].receivedDay < 10) {
                cout << "0" << form_8489short[i].receivedDay << "/";
            }
            else {
                cout << form_8489short[i].receivedDay << "/";
            }
             cout <<   form_8489short[i].receivedYear;
            cout << setw(8) << form_8489short[i].soldMonth << "/";

            if (form_8489short[i].soldDay < 10) {
                cout << "0" << form_8489short[i].soldDay << "/";
            }
            else {
                cout << form_8489short[i].soldDay << "/";
            }
           cout  << form_8489short[i].soldYear;
            cout << setw(16) << form_8489short[i].soldPrice;
            cout << setw(13) << form_8489short[i].purchasePrice;
            cout << setw(18) << "---";
            cout << setw(17) << "---";
            cout << setw(18) << form_8489short[i].gainOrLoss;
           
            cout << endl << endl;
        }
        cout << "2. Enter Totals:" << setw(52) << ttlSShort << setw(13) << ttlPShort << setw(53) << ttlGainShort;
    }
    else {
        cout << "Skip *Short term capital gains* page on 8949" << endl;
    }
      if (csv_longvector.size() > 0) {
        for (int i = 0; i < csv_longvector.size(); i++) {
            string assetN = csv_longvector[i].assetName;
            int receivedD = csv_longvector[i].receivedDay;
            int receivedM = csv_longvector[i].receivedMonth;
            int receivedY = csv_longvector[i].receivedYear;
            double purchaseP = csv_longvector[i].purchasePrice;
            int soldD = csv_longvector[i].soldDay;
            int soldM = csv_longvector[i].soldMonth;
            int soldY = csv_longvector[i].soldYear;
            double soldP = csv_longvector[i].soldPrice;
            double gainLoss = soldP - purchaseP;
            ttlPLong = ttlPLong + purchaseP;
            ttlSLong = ttlSLong + soldP;
            ttlGainLong = ttlSLong - ttlPLong;

            tempcsvData.assetName = assetN;
            tempcsvData.receivedDay = receivedD; tempcsvData.receivedMonth = receivedM;
            tempcsvData.receivedYear = receivedY; tempcsvData.purchasePrice = purchaseP;
            tempcsvData.soldDay = soldD; tempcsvData.soldMonth = soldM; tempcsvData.soldYear = soldY;
            tempcsvData.soldPrice = soldP; tempcsvData.gainOrLoss = gainLoss;

            form_8489long.push_back(tempcsvData);
        }

        cout << endl << endl << endl;
        cout << "Enter this information of form 8949 sheet (page two)." << endl;
        cout << "_____________________________________________________" << endl;
        cout << "Check box C above table" << endl << endl;

        cout << setw(13) << "(a)" << setw(21) << "(b)" << setw(16) << "(c)" << setw(16) << "(d)" <<
            setw(13) << "(e)" << setw(18) << "(f)" << setw(17) << "(g)" << setw(18) << "(h)" << endl;

        cout << "Description of property" << setw(15) << "Date Aquired" << setw(15) << "Date Sold" << setw(15) <<
            "Proceeds" << setw(16) << "Cost or other" << setw(16) << "Codes from" <<
            setw(16) << "Amount of" << setw(21) << "Gain or Loss" << endl;
        cout << setw(80) << "basis" << setw(21) << "instructions" << setw(16) << "adjustment" << endl;

        cout << "_______________________" << setw(15) << "____________" << setw(15) << "__________" << setw(15) <<
            "________" << setw(16) << "_____________" << setw(18) << "______________" <<
            setw(16) << "____________" << setw(19) << "____________" << endl << endl;

        for (int i = 0; i < form_8489long.size(); i++) {

            cout << setw(13) << form_8489long[i].assetName;
            cout << setw(16) << right << form_8489long[i].receivedMonth << "/"; if (form_8489short[i].receivedDay < 10) {
                cout << "0" << form_8489short[i].receivedDay << "/";
            }
            else {
                cout << form_8489short[i].receivedDay << "/";
            }
            cout << form_8489short[i].receivedYear;

            cout << setw(8) << form_8489short[i].soldMonth << "/";

            if (form_8489short[i].soldDay < 10) {
                cout << "0" << form_8489short[i].soldDay << "/";
            }
            else {
                cout << form_8489short[i].soldDay << "/";
            }
            cout << form_8489short[i].soldYear;
            cout << setw(16) << form_8489long[i].soldPrice;
            cout << setw(13) << form_8489long[i].purchasePrice;
            cout << setw(18) << "---";
            cout << setw(17) << "---";
            cout << setw(18) << form_8489long[i].gainOrLoss;
            cout << endl << endl;
        }
        cout << "2. Enter Totals:" << setw(52) << ttlSLong << setw(13) << ttlPLong << setw(53) << ttlGainLong;
      }
    else {
        cout << "Skip *Long term capital gains* page of 8949" << endl;
    }
}
// Function: menuDisplayFunction
// Parameters: nothing
// Returns: nothing
// Description:
//  Displays first part of menu (happens only once)
void menuDisplayFunction() {
    cout << "*******************************************" << endl;
    cout << "       Welcome to Fast Tax Software" << endl;
    cout << "*******************************************" << endl << endl;
    cout << "Please enlarge program to this size:" << endl;
    cout << "-----------------------------------------------------------" << 
            "-----------------------------------------------------------" <<
            "---------------------------" << endl;
  }
// Function: menuDisplayFunctionTwo
// Parameters: the calculate8949 function
// Returns: nothing
// Description:
//  Essentially a function that displays a function displaying the 8949 form , was going to add other forms
// but was too time intensive given the circumstances
void menuDisplayFunctionTwo(void* calculate89, vector<csvData>csv_short, vector<csvData>csv_long,
    vector<form8949Data>form_short, vector<form8949Data>form_long) {
    int yes = 0;
    int userEnter = -1;
    cout << "*********************" << endl <<
        "    Menu Options" << endl <<
        "*********************" << endl << endl;
    cout << "Tax forms: " << endl;
    cout << "___________" << endl; cout << "1 - Form 8949 (Use once)" << endl << "2 - Form Schedule D (Not funcitonal)" << endl << endl;
    cout << "Enter number to view Form: ";

    while (userEnter != 3) {
        
        cin >> userEnter;
        if (userEnter == 1) {
            calculate8949(csv_short, csv_long, form_short, form_long);

            cout << endl << endl;
            cout << "******************************************************" <<
                "******************************************************" <<
                "***********************************"; cout << endl << endl;
            cout << "5 - Go back to Menu";
        }
     //   else if (userEnter == 2) {
       //     cout << endl << endl;
       //    cout << "Schedule D";
        //    cout << endl << endl;
       //     cout << "5 - Go back to Menu";
      //  }
        else if (userEnter == 3) {
            cout << endl << endl;
            cout << "Exiting Program...." << endl;
        }
       
        
        else if (userEnter == 5) {
            cout << endl << endl;
            cout << "*********************" << endl <<
                "    Menu Options" << endl <<
                "*********************" << endl << endl;
            cout << "Tax forms: " << endl;
            cout << "___________" << endl; cout << "1 - Form 8949 (Use once)" << endl << "2 - Form Schedule D" << endl << endl;
            cout << "Enter number to view Form: ";

        }
        else {
            cout << endl << endl;
            cout << "Please enter correct option number" << endl;
            cout << "Enter number to view Form: ";
            }
    }
}
// Function:   GetPrivData
// Parameters: none
// Returns:    Private object with stored data
// Descr:
// Prompt and read data from user to store in
// private class
Private GetPrivData() {
    Private p;
    string name;
    string addy, city, state;
    int bday, bmonth, byear, ssn;
    int zipCode;
    int income;
    cout << "**Personal Details**" << endl << endl;
    cout << "Enter Full Name: ";
    cin >> name;
    cout << "Enter birthday [Month/Day/Year(0000)]: "; cout << endl; cout << "Month: ";
    cin >> bmonth;  cout << "Day: ";
    ; cin >> bday; cout << "Year: "; cin >> byear; cout << endl;
    cout << "Enter Address: " << "[Address/City/State]" << endl; cout << "Address: "; cin >> addy; cout << endl;
    cout << "City: "; cin >> city; cout << "Enter Zip Code: "; cin >> zipCode;
    cout << "State: "; cin >> state; cout << endl;
    cout << "Enter Social Security Number: "; cin >> ssn;
    cout << "Enter Year's income from job: "; cin >> income;

   // set object values using mutators (aka setters)
    p.setName(name);
    p.setBday(bday);
    p.setBMonth(bmonth);
    p.setBYear(byear);
    p.setAddress(addy);
    p.setCity(city);
    p.setState(state);
    p.setZip(zipCode);
    p.setSSN(ssn);
    p.setIncome(income);
    
    // CLEAR OUT TRAILING NEWLINE (NEED IT)
    cin.ignore();
    return p;
}

// Function:   DisplayPriData
// Parameters: object of Private class
// Returns:    nothing
// Descr:
// Displays all private data and calculation values
// to standard output.
void DisplayPrivData(Private s) {
    cout << fixed << setprecision(3)
        << "Name: "
        << s.getName()
        << endl
        << "Birthday : "
        << s.getBDay() << "/" << s.getBMonth() << "/" << s.getBYear() << endl
        << "Address: " << s.getAddress() << " , " << s.getCity() << " , " << s.getState() << endl
        << "SSN: " << s.getSSN() << endl
        << "Balance after Federal Income Tax: " << s.getFedIncome()
        << endl
        << endl;
    return;
}

// Function:   CreateArray
// Parameters: array size
// Returns:    pointer to the allocated and initialized array
// Descr:      uses loop to display all elements to 
//             allocates pointer memory with new and reads in values for each

int* createArray(int size) {
    // allocate the memory for the pointer
    int* target = new int[size];
    int check;
    // store values in array
    for (int i = 0; i < size; i++) {
        cout << "8949 Total " << i + 1 << ": ";
        cin >> check; // make sure there are no negative inputs 
        while (check < 0) {
            cout << "Error, negative number inputted, please enter positive number";
            cout << "Enter an integer: ";
            cin >> check;
        }
        target[i] = check;
    }

    return target;
}

// Function:   totalFunction
// Parameters: Array (form8949 total value, size of array (how many forms there are)
// Returns:    total calculation
// Descr:      adds all values from array to get total amount

int totalFunction(int* forms8949, const int size) {
  
        double total = 0;
    for (int i = 0; i < size; i++) {
        total = total + forms8949[i];
    }

    // for function adds elements through array
    // divide by size
   
    return total;
    delete forms8949; // deletes previous pointer data
}


