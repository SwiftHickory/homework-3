/******************************************************************************
-   yang.cpp
-   Prog Tools for Scits Engrs - CIVL 8903 Homework III
-
-   Discription:
-   This program is to total up the number of events recorded per instruments
-   and per station of California Seismic Network.
-
-   github repository: https://github.com/SwiftHickory/homework-3.git
******************************************************************************/

#include <iostream>
#include <fstream>
#include <cstring>
#include <cstdlib>
#include <iomanip>

using namespace std;

// get the index of a string in a string array
int indexInArray(const string &str, const string strArray[], int length);

// print the error message to terminal and error file
void errorMessage(int entryNumber, string station, string instrument,
    int numberOfEvents, string error, ofstream &errorFile);

// print the statistics of total and correct number of entries to both terminal and file
void entryMessage(int totalEntry, int correctEntry, ofstream &errorFile);

// print summary to the output file
void summaryPrint(const int table[][10], int numberOfStations, int numberOfInStruments,
    const int eventsPerStation[], const int eventsPerInstrument[], ofstream &outputFile,
    const string stationList[], const string instrumentList[]);

// find minima and maxima valuse in an integer array
void minmax(const int array[], int length, int &min, int &max);

// main function
int main() {

    string inputFileName;               // enter from the terminal
    ifstream inputFile;                 // input file handle
    ofstream outputFile;                // output file handle
    ofstream errorFile;                 // error file handle
    int numberOfStations;               // read from input file
    int numberOfInStruments;            // read from input file
    string stationList[100];            // array to store all the staion names
    string instrumentList[10];          // array to store all the instrument names
    int table[100][10] = {0};           // keep track os the recorded events
    int eventsPerStation[100] = {0};    // total number of events per station
    int eventsPerInstrument[10] = {0};  // total number of events per instrument
    int entryNumber = 0;                // record number of entry read from file
    int correctEntryNumber = 0;         // record the number of the correct entry
    string station;                     // station name read from each line
    string instrument;                  // instrument read from each line
    int numberOfEvents;                 // event read from each line
    int indexOfStation;                 // index of a station in the stationList
    int indexOfInstrument;              // index of an instrument in the instrumentList

    // prompt user for input file
    cout << "Please Enter input file: ";
    cin >> inputFileName;

    // open input file and perform sanity check it
    inputFile.open(inputFileName.c_str());
    if (!inputFile.is_open()) {
    	cerr << "Cannot open input file: "
    	     << inputFileName
    	     << endl;
    	exit(EXIT_FAILURE);
    }

    // open the output file and perform sanity checks
    // output file should always be yang.out
    outputFile.open("yang.out");
    if (!outputFile.is_open()) {
        cerr << "Cannot create output file: yang.cpp" << endl;
        exit(EXIT_FAILURE);
    }

    // open the error file and perform sanity checks
    // error file should always be yang.err
    errorFile.open("yang.err");
    if (!errorFile.is_open()) {
        cerr << "Cannot create error file: yang.err" << endl;
        exit(EXIT_FAILURE);
    }

    // read number fo stations and number of instruments from the input file
    inputFile >> numberOfStations >> numberOfInStruments;

    // print them to the terminal
    cout << "Number of stations: " << numberOfStations << endl;
    cout << "Number fo instruments: " << numberOfInStruments << endl;

    // read the station list
    cout << "Reading stations..." << endl;
    for (int i = 0; i < numberOfStations; i++) {
        inputFile >> stationList[i];
    }

    // read the instrument list
    cout << "Reading instruments..." << endl;
    for (int i = 0; i < numberOfInStruments; i++) {
        inputFile >> instrumentList[i];
    }

    // read the entry line by line
    cout << "Reading entries..." << endl;
    while (inputFile >> station >> instrument >> numberOfEvents) {
        // increment of entry
        entryNumber++;

        // get the index in the list
        indexOfStation = indexInArray(station, stationList, numberOfStations);
        indexOfInstrument = indexInArray(instrument, instrumentList, numberOfInStruments);

        // check whether this entry is existed
        if (indexOfStation >= 0) {
            if (indexOfInstrument >= 0) {
                // check whether the number of event is a non-negative number
                if (numberOfEvents >= 0) {
                    //update the table
                    table[indexOfStation][indexOfInstrument] += numberOfEvents;

                    // update number of events of each station
                    eventsPerStation[indexOfStation] += numberOfEvents;

                    // update number of events of each type of instrument
                    eventsPerInstrument[indexOfInstrument] += numberOfEvents;

                    // increment of the corrent number of entry
                    correctEntryNumber++;
                } else {
                    // if number of event is a negative number
                    errorMessage(entryNumber, station, instrument, numberOfEvents, "Negative events", errorFile);
                }
            } else {
                // if the instrument does not exist
                errorMessage(entryNumber, station, instrument, numberOfEvents, "instrument does not exist", errorFile);
            }
        } else {
            // if the station does not exist
            errorMessage(entryNumber, station, instrument, numberOfEvents, "station does not exist", errorFile);
        }
    }

    entryMessage(entryNumber, correctEntryNumber, errorFile);

    cout << "Generating report..." << endl;
    summaryPrint(table, numberOfStations, numberOfInStruments, eventsPerStation,
        eventsPerInstrument, outputFile, stationList, instrumentList);

    // close the input file
    inputFile.close();

    // close the output file
    outputFile.close();

    // close the error file
    errorFile.close();

    cout << "Completed" << endl;

    return 0;
    
}

// get the index of a string in a string array
int indexInArray(const string &str, const string strArray[], int length) {

    // sequence search the string
    for (int i = 0; i < length; i++) {
        // if find the string, return the index
        if (str == strArray[i]) {
            return i;
        }
    }

    // if the string is not in this array, return -1
    return -1;

}

// print the error message to terminal and error file
void errorMessage(int entryNumber, string station, string instrument,
    int numberOfEvents, string error, ofstream &errorFile) {

    // print the error message to terminal
    cout << "Entry #" << entryNumber << ": " << station << " " << instrument
         << " " << numberOfEvents << " -- Warning: " << error << endl;

    // print the error message to error file
    errorFile << "Entry #" << entryNumber << ": " << station << " " << instrument
         << " " << numberOfEvents << " -- Warning: " << error << endl;

    return;

}

// print the statistics of total and correct number of entries to both terminal and file
void entryMessage(int totalEntry, int correctEntry, ofstream &errorFile) {

    // print number of correct entries
    if (correctEntry < 2) {
        cout << "Total of " << correctEntry << " entry processed correctly" << endl;
        errorFile << "Total of " << correctEntry << " entry processed correctly" << endl;
    } else {
        cout << "Total of " << correctEntry << " entries processed correctly" << endl;
        errorFile << "Total of " << correctEntry << " entries processed correctly" << endl;
    }

    // print number of ingored entries
    int ignoredEntry = totalEntry - correctEntry;
    if ( ignoredEntry < 2) {
        cout << "Total of " << ignoredEntry << " entry ignored" << endl;
        errorFile << "Total of " << ignoredEntry << " entry ignored" << endl;
    } else {
        cout << "Total of " << ignoredEntry << " entries ignored" << endl;
        errorFile << "Total of " << ignoredEntry << " entries ignored" << endl;
    }

    return;

}

// print summary to the output file
void summaryPrint(const int table[][10], int numberOfStations, int numberOfInStruments,
    const int eventsPerStation[], const int eventsPerInstrument[], ofstream &outputFile,
    const string stationList[], const string instrumentList[]) {

    outputFile << "Seismic Events Summary Report" << endl << endl;

    // print the tabulated events data
    outputFile << setw(11) << "Station";
    for (int i = 0; i < numberOfInStruments; i++) {
        outputFile << setw(16) << instrumentList[i];
    }
    outputFile << endl;

    // print for each station
    for (int i = 0; i < numberOfStations; i++) {
        outputFile << setw(11) << stationList[i];

        for (int j = 0; j < numberOfInStruments; j++) {
            outputFile << setw(16) << table[i][j];
        }

        outputFile << endl;
    }
    outputFile << endl;

    // print total events per station
    outputFile << "Total number of events per station" << endl << endl;
    outputFile << setw(11) << "Station" << setw(10) << "Total" << endl;

    for (int i = 0; i < numberOfStations; i++) {
        outputFile << setw(11) << stationList[i] << setw(10) << eventsPerStation[i] << endl;
    }
    outputFile << endl;

    // print total events per instrument
    outputFile << "Total number of events per instrument" << endl << endl;
    outputFile << setw(16) << "Instrument" << setw(10) << "Total" << endl;

    for (int i = 0; i < numberOfInStruments; i++) {
        outputFile << setw(16) << instrumentList[i] << setw(10) << eventsPerInstrument[i] << endl;
    }
    outputFile << endl;

    // pring station(s) with most and least reocrd
    int min, max;
    bool isFirstPrint = true;
    minmax(eventsPerStation, numberOfStations, min, max);

    outputFile  << "Station(s) with the most records is: ";
    for (int i = 0; i < numberOfStations; i++) {
        if (eventsPerStation[i] == max) {
            if (isFirstPrint) {
                isFirstPrint = false;
            } else {
                outputFile << ", ";
            }
            outputFile  << stationList[i];
        }
    }
    outputFile  << " with " << max << " event(s)" << endl;

    outputFile  << "Station(s) with the least records is: ";
    isFirstPrint = true;
    for (int i = 0; i < numberOfStations; i++) {
        if (eventsPerStation[i] == min) {
            if (isFirstPrint) {
                isFirstPrint = false;
            } else {
                outputFile << ", ";
            }
            outputFile  << stationList[i];
        }
    }
    outputFile  << " with " << min << " event(s)" << endl << endl;

    // pring instrument(s) with most and least reocrd
    minmax(eventsPerInstrument, numberOfInStruments, min, max);

    outputFile  << "Instrument(s) with the most records is: ";
    isFirstPrint = true;
    for (int i = 0; i < numberOfInStruments; i++) {
        if (eventsPerInstrument[i] == max) {
            if (isFirstPrint) {
                isFirstPrint = false;
            } else {
                outputFile << ", ";
            }
            outputFile  << instrumentList[i];
        }
    }
    outputFile  << " with " << max << " event(s)" << endl;

    outputFile  << "Instrument(s) with the least records is: ";
    isFirstPrint = true;
    for (int i = 0; i < numberOfInStruments; i++) {
        if (eventsPerInstrument[i] == min) {
            if (isFirstPrint) {
                isFirstPrint = false;
            } else {
                outputFile << ", ";
            }
            outputFile  << instrumentList[i];
        }
    }
    outputFile  << " with " << min << " event(s)" << endl;

    return;

}

// find minima and maxima valuse in an integer array
void minmax(const int array[], int length, int &min, int &max) {

    min = array[0];
    max = array[0];

    for (int i = 1; i < length; i++) {
        if (array[i] < min) {
            // check for minima
            min = array[i];
        } else if (array[i] > max) {
            // check for maxima
            max = array[i];
        }
    }

    return;

}
