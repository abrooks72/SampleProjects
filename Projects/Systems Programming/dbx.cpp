/*Austin Brooks
Cssc1975
CS530, Spring 2020
Assignment 1, DBX
Source code file, dbx.cpp*/
#include <iostream>
#include <iostream>
#include <fstream> 
#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <bitset>
using namespace std;
//Converts a regular file(binary or ascii values) to Hex or binary
//Binary Byte or Hex Byte?
string stringToHexOrByte(char input, bool toByte)
{	//Out string to be manipulated
	string output;

	if (toByte) {
		bitset<8> temp(input);
		output = temp.to_string();
	}
	else {
		static const char hex_digits[] = "0123456789ABCDEF";
		output.reserve(2);
		output.push_back(hex_digits[input >> 4]);
		output.push_back(hex_digits[input & 15]);
	}

	return output;
}

int main(int argc, char* argv[]) {
	// Create a text string, which is used to output the text file
	string initialInput;
	// char variable in order to read char by char from file
	char myChar;
	//bool value to know if we have the byte parameter called
	bool byteOption;
	// string which will retain the converted char to hex or byte
	string convChar;
	string fileName;
	//Check if we have -b byte Option
	if (argc > 2) {
		byteOption = true;
		fileName = argv[2];
	}
	else {
		byteOption = false;
		fileName = argv[1];
	}
	// Read from the text file
	ifstream myFile(fileName);
	// we will use this group variable to separate the chars printed as in the examples
	int group = 1;
	cout << &initialInput << ": ";

	while (myFile >> std::noskipws >> myChar) {

		// if the binary option is selected we will print all the chars separetely
		if (byteOption) {
			group = 0;
		}

		convChar = stringToHexOrByte(myChar, byteOption);
		cout << convChar;

		// here we use the group to get the output expected
		if (group) {
			group = 0;
		}
		else {
			cout << " ";
			group = 1;
		}

		//clear non-printable chars
		if ((int)myChar < 32) {
			myChar = '.';
		}

		// Append to input string
		initialInput.append(1, myChar);
	}
	// Close the file
	myFile.close();

	//print input
	cout << "  " << initialInput;

	return 0;
}


			
