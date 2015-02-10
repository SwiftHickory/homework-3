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
#include <cstring>
#include <cstdlib>

// main function

int main() {

    string inputFileName;	// enter from the terminal
    ifstream inputFile;		// input file handle

    // prompt user for input file
    cout << "Please Enter input file: ";
    cin >> inputFileName;

    // open input file and perform sanity check it
    inputFile.open(inputFileName.c_str());
    if(!inputFile.is_open()) {
    	cerr << "Cannot open input file: "
    	     << inputFileName
    	     << endl;
    	exit(EXIT_FAILURE);
    }
    
}