/******************************************************************************
-   yang.cpp
-   Prog Tools for Scits Engrs - CIVL 8903 Homework III
-
-   Discription:
-   This program is to total up the number of events recorded per instruments
-   and per station of California Seismic Network.
-
-   Version:
-   02/07/2015    Yang Yang    Initial Code
******************************************************************************/

#include <iostream>
#include <fstream>
#include <cstring>
#include <cstdlib>

using namespace std;

// get the index of a string in a string array
int indexInArray(string const &str, string * const &strArray, int length);

// main function
int main() {

    string inputFileName;       // enter from the terminal
    ifstream inputFile;         // input file handle
    ofstream outputFile;        // output file handle
    ofstream errorFile;         // error file handle
    int numberOfStations;       // read from input file
    int numberOfInStruments;    // read from input file
    string *stationList;        // array to store all the staion names
    string *instrumentList;     // array to store all the instrument names
    int **table;                // keep track os the recorded events
    int *eventsPerStation;      // total number of events per station
    int *eventsPerInstrument;   // total number of events per instrument
    int entryNumber = 0;        // record number of entry read from file
    int correctEntryNumber = 0; // record the number of the correct entry
    string station;             // station name read from each line
    string instrument;          // instrument read from each line
    int numberOfEvents;         // event read from each line
    int indexOfStation;         // index of a station in the stationList
    int indexOfInstrument;      // index of an instrument in the instrumentList

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
    outputFile.open("yang.cpp");
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

    // allocate corresponding memory for the arrays
    stationList = new string[numberOfStations];
    instrumentList = new string[numberOfInStruments];
    eventsPerStation = new int[numberOfStations];
    eventsPerInstrument = new int[numberOfInStruments];

    // allocate memory for 2d array table
    table = new int*[numberOfStations];
    for (int i = 0; i < numberOfStations; i++)
        table[i] = new int[numberOfInStruments];

    // read the station list and initialize eventsPerStation
    for (int i = 0; i < numberOfStations; i++) {
        inputFile >> stationList[i];
        eventsPerStation[i] = 0;
    }

    // read the instrument list and initialize eventsPerInstrument
    for (int i = 0; i < numberOfInStruments; i++) {
        inputFile >> instrumentList[i];
        eventsPerInstrument[i] = 0;
    }

    cout << "Station read" << endl;
    cout << "Instruments read" << endl;

    // initialize the table
    for (int i = 0; i < numberOfStations; i++)
        for (int j = 0; j < numberOfInStruments; j++)
            table[i][j] = 0;

    // read the entry line by line
    cout << "Reading entries" << endl;
    while (inputFile >> station) {
        // then read the instrument and number of events
        inputFile >> instrument >> numberOfEvents;

        // get the index in the list
        indexOfStation = indexInArray(station, stationList, numberOfStations);
        indexOfInstrument = indexInArray(instrument, instrumentList, numberOfInStruments);

        // check whether this entry is existed
        if (indexOfStation >= 0) {
            if (indexOfInstrument >= 0) {
                //update the table
                table[indexOfStation][indexOfInstrument]++;

                // check whether the number of event is a non-negative number
                if (numberOfEvents >= 0) {
                    // update number of events of each station
                    eventsPerStation[indexOfStation] += numberOfEvents;

                    // update number of events of each type of instrument
                    eventsPerInstrument[indexOfInstrument] += numberOfEvents;

                    // increment of the corrent number of entry
                    correctEntryNumber++;
                } else {
                    // if number of event is a negative number
                    // print the error message to terminal
                    cout << "Entry #" << entryNumber << ": " << station << " " << instrument
                         << " " << numberOfEvents << " -- Warning: Negative events" << endl;

                    // print the error message to error file
                    errorFile << "Entry #" << entryNumber << ": " << station << " " << instrument
                              << " " << numberOfEvents << " -- Warning: Negative events" << endl;

                }

                // if the instrument does not exist
                // print the error message to terminal
                cout << "Entry #" << entryNumber << ": " << station << " " << instrument
                     << " " << numberOfEvents << " -- Warning: instrument does not exist" << endl;

                // print the error message to error file
                errorFile << "Entry #" << entryNumber << ": " << station << " " << instrument
                          << " " << numberOfEvents << " -- Warning: Instrument does not exist" << endl;
            }

            // if the station does not exist
            // print the error message to terminal
            cout << "Entry #" << entryNumber << ": " << station << " " << instrument
                 << " " << numberOfEvents << " -- Warning: Station does not exist" << endl;

            // print the error message to error file
            errorFile << "Entry #" << entryNumber << ": " << station << " " << instrument
                      << " " << numberOfEvents << " -- Warning: Station does not exist" << endl;
        }

        // increment of entry
        entryNumber++;
    }

    // delete the allocated memory
    delete [] stationList;
    delete [] instrumentList;
    delete [] eventsPerStation;
    delete [] eventsPerInstrument;

    // delete the 2d array table
    for (int i = 0; i < numberOfStations; i++)
        delete [] table[i];
    delete [] table;

    // close the input file
    inputFile.close();

    // close the output file
    outputFile.close();

    // close the error file
    errorFile.close();

    return 0;
    
}

// get the index of a string in a string array
int indexInArray(string const &str, string * const &strArray, int length) {

    // compare the string with each element in the string array in order
    for (int i = 0; i < length; i++) {
        // if find the string, return the index
        if (!str.compare(strArray[i])) {
            return i;
        }
    }

    // if the string is not in this array, return -1
    return -1;

}