/*
    Austin Brooks           cssc 1975
    Alex Kondan             cssc 1959
    Cameron Yokoi           cssc 1928
    Cristian Hernandez      cssc 1940

    dxe.cpp
*/

#include <iostream>
#include <String>
#include <fstream>
#include <sstream>
#include <math.h>

#define DebugMessage

using namespace std;

void fileWriter(ostream& sicFile, ostream& listFile, istream& objFile, istream& symFile);
string findInstruct(string objCode);
string convertHex(int num);
string spaces(string text);
int convert(string length);
int nFlag(string objCode);
int iFlag(string objCode);
int xFlag(string objCode);
int bFlag(string objCode);
int pFlag(string objCode);
int eFlag(string objCode);

/*
    fileWriter will use the parameters given for the .sic, .lis, and .obj file to write the information from the .obj file
    to create both the .lis and .sic file calling other methods depending on the type of record the line read is
*/
void fileWriter(ostream& sicFile, ostream& listFile, istream& objFile, istream& symFile)
{
    string symbolFlags[50], symbolNames[50], symbolValues[50], literalNames[50], literalLengths[50], literalAdds[50];
    string line;
    int lineNum = 1;
    int loc = 0;
    int m = 0;

    listFile << "Line" + spaces("Line") + "Loc" + spaces("Loc") + "Source Statement\n";

    std::getline(symFile, line);                                                    //Retrieves the first two lines as they do not contain any information necessary for the Symtab
    std::getline(symFile, line);

    while (!symFile.eof())                                  //Gets the symbol table
    {
        std::getline(symFile, line);

        if (line[0] == NULL)
            break;

        symbolNames[m] = line.substr(0, 6);                 //Stores the symbol name
        symbolValues[m] = line.substr(8, 6);                //Stores the symbol value
        symbolFlags[m] = line.substr(16, 1);                //Stores the symbol flags

        m++;
    }

    std::getline(symFile, line);                                                    //Retrieves the first two lines as they do not contain any information necessary for the Littab
    std::getline(symFile, line);

    m = 0;

    while (!symFile.eof())                                  //Gets the Literal table
    {
        std::getline(symFile, line);

        if (line[0] == NULL)
            break;

        literalNames[m] = line.substr(8, 6);                //Stores the literal name
        literalLengths[m] = line.substr(19, 1);             //Stores the literal value
        literalAdds[m] = line.substr(24, 6);                //Stores the literal flags

        m++;
    }

    while (!objFile.eof())
    {
        line.clear();
        getline(objFile, line);

        if (line[0] == 'H')                                                                     //Header Record
        {
            string progName = line.substr(1, 6);                                                //program name
            string startAdd = line.substr(7, 6);                                                //starting address (hex)
            string length = line.substr(13, 6);                                                 //length of object program in bytes (hex)
            string header;

            for (int i = 0; i <= 50 && symbolValues[i] != ""; i++)
            {
                if (symbolValues[i] == startAdd)                                                                    //If a symbol is the same as the starting address then it will replace it in the line to be printed
                {
                    header = progName + spaces(progName) + "START" + spaces("START") + symbolNames[i] + '\n';
                    break;
                }
                else
                    header = progName + spaces(progName) + "START" + spaces("START") + to_string(convert(startAdd)) + '\n';
            }

            loc = convert(startAdd);                                                                                //The location counter is assigned the value of the starting address

            listFile << ((lineNum) * 5) << spaces(to_string(lineNum * 5)) << convertHex(loc) << spaces(convertHex(loc)) << header;        //Prints out the lines needed for the listing and sic files
            sicFile << header;
            lineNum++;
        }
/*
        for(string vals : symbolValues)
        {
            if (convert(vals) == loc)
            {
                if(loc - convert(vals) % 3 == 0)
                    "RESW"
                else
                    "RESB"
            }

            loc += 3;
        }
*/
        if (line[0] == 'T')                                                                    //Text Record
        {
            string startAdd = line.substr(1, 5);                                                    //starting address in this record (hex)
            string length = line.substr(7, 2);                                                      //length of object code in this record in bytes (hex)
            string objCode[10];

            for (int a = 0; a < convert(length) / 3; a++)                                               //Retrieves all of the object codes in a line maximum of 10 object codes in a given line
                objCode[a] = line.substr((9 + (6 * a)), 6);

            for (string obj : objCode)
            {
                if (obj == "")                                                                      //Ignores any empty object codes
                    break;

                int n = nFlag(obj);                                                                 //Finds the nixbpe flags for each of the object codes
                int i = iFlag(obj);
                int x = xFlag(obj);
                int b = bFlag(obj);
                int p = pFlag(obj);
                int e = eFlag(obj);
                string instruct = findInstruct(obj);                                                //Returns instruction
                string address = obj.substr(2, 4);                                                  //Gets the address that is targeted
                string spacing = spaces(instruct);
                string label = "";                                                                  //Used for the symbols and literals to replace in the line
                string text;

                int convAdd = convert(address);

                for (int i = 0; i <= 50 && symbolValues[i] != ""; i++)
                {
                    if (convert(symbolValues[i]) == convAdd)                                        //Will check the symbols with the address in order to replace it
                    {
                        address = symbolNames[i];
                        break;
                    }
                    else if (literalAdds[i] != "" && convert(literalAdds[i]) == convAdd)            //Will check the literals with the address in order to replace it
                    {
                        address = literalNames[i];
                        break;
                    }
                }

                if (n == 1 && i == 1 || n == 0 && i == 0)                                               //Simple Addressing mode
                {                                                                                               //How target address was calculated goes down from here on page 499 of textbook
                    if (x == 0 && b == 0 && p == 0 && e == 0)                                                   //disp
                        text = instruct + spacing + address;
                    else if (x == 0 && b == 0 && p == 0 && e == 1)                                              //addr
                        text = "+" + instruct + spacing.substr(0, spacing.size() - 1) + address;
                    else if (x == 0 && b == 0 && p == 1 && e == 0)                                              //(PC) + disp
                        text = instruct + spacing + address;
                    else if (x == 0 && b == 1 && p == 0 && e == 0)                                              //(B) + disp
                        text = instruct + spacing + address;
                    else if (x == 1 && b == 0 && p == 0 && e == 0)                                              //disp + (X)
                    {
                        address = address + ",X";
                        text = instruct + spacing + address;
                    }
                    else if (x == 1 && b == 0 && p == 0 && e == 1)                                              //addr + (X)
                    {
                        address = address + ",X";
                        text = "+" + instruct + spacing.substr(0, spacing.size() - 1) + address;
                    }
                    else if (x == 1 && b == 0 && p == 1 && e == 0)                                              //(PC) + disp + (X)
                    {
                        address = address + ",X";
                        text = instruct + spacing + address;
                    }
                    else if (x == 1 && b == 1 && p == 0 && e == 0)                                              //(B) + disp + (X)
                    {
                        address = address + ",X";
                        text = instruct + spacing + address;
                    }
                }

                else if (n == 1 && i == 0)                                                              //Indirect Addressing Mode
                {
                    if (x == 0 && b == 0 && p == 0 && e == 0)                                                   //disp
                        text = instruct + spacing + "@" + address;
                    else if (x == 0 && b == 0 && p == 0 && e == 1)                                              //addr
                    {
                        address = "@" + address;
                        text = "+" + instruct + spacing.substr(0, spacing.size() - 1) + address;
                    }
                    else if (x == 0 && b == 0 && p == 1 && e == 0)                                              //(PC) + disp
                    {
                        address = "@" + address;
                        text = instruct + spacing + address;
                    }
                    else if (x == 0 && b == 1 && p == 0 && e == 0)                                              //(B) + disp
                    {
                        address = "@" + address;
                        text = instruct + spacing + address;
                    }
                }

                else if (n == 0 && i == 1)                                                              //Immediate Addressing Mode
                {
                    if (x == 0 && b == 0 && p == 0 && e == 0)                                                   //disp
                    {
                        address = "#" + address;
                        text = instruct + spacing + address;
                    }
                    else if (x == 0 && b == 0 && p == 0 && e == 1)                                              //addr
                    {
                        address = "#" + address;
                        text = "+" + instruct + spacing.substr(0, spacing.size() - 1) + address;
                    }
                    else if (x == 0 && b == 0 && p == 1 && e == 0)                                              //(PC) + disp
                    {
                        address = "#" + address;
                        text = instruct + spacing + address;
                    }
                    else if (x == 0 && b == 1 && p == 0 && e == 0)                                              //(B) + disp
                    {
                        address = "#" + address;
                        text = instruct + spacing + address;
                    }
                }

                for (int i = 0; i <= 50; i++)
                {
                    if (symbolValues[i] == "")                                      //To avoid using the null values
                        break;

                    if (convert(symbolValues[i]) == loc)                            //Checks symbols to see if it should be printed at the beginning of the line based on the location counter
                    {
                        label = symbolNames[i];
                        break;
                    }
                }

                listFile << ((lineNum) * 5) << spaces(to_string(lineNum * 5)) << convertHex(loc) << spaces(convertHex(loc)) << label << spaces(label) << text << spaces(address) << obj << '\n';          //Writes the information needed to the listing file
                sicFile << label << spaces(label) << text << '\n';                                                                                                                                      //Writes the information needed to the sic file
                lineNum++;                                                                                          //Increments both lineNum and location counter to be used to print out each of the lines
                loc += 3;
            }
        }

        if (line[0] == 'E')                                                                //End Record
        {
            string symbol = "";
            string address = line.substr(1, 6);                                                 //address of first executable instruction (hex)
            string end;

            for (int i = 0; i <= 50; i++)                                                       //Checks all of the symbols with the address to see if it should be replaced with a symbol rather than a raw address
            {
                if (symbolValues[i] == address)                                                
                {
                    end = "END" + spaces("END") + symbolNames[i];
                    break;
                }
                else
                    end = "END" + spaces("END") + address;
            }

            sicFile << spaces("") << end;                                                                                                                       //Prints out the information for both the sic file and listing file
            listFile << ((lineNum) * 5) << spaces(to_string(lineNum * 5)) << convertHex(loc) << spaces(convertHex(loc)) << symbol << spaces(symbol) << end;
        }
    }
}

/*
    Will be used to convert the int to a hex string specifically
    for the location counter
*/
string convertHex(int num)
{
    char returnVal[8];
    sprintf_s(returnVal, "%X", num);
    return returnVal;
}

/*
    Converts given strings that are in Hex into the decimal representation
*/
int convert(string num)
{
    return std::stoi(num, nullptr, 16);
}

/*
    Adds whitespace to the string so that all of the strings will be formatted correctly for
    the .lis and .sic files
*/
string spaces(string text)
{
    string spaces = "";

    for (int i = text.size(); i < 8; i++)                           //Adding in spaces to the name of the file to fill out the whitespace that is needed
        spaces.append(" ");

    return spaces;
}

/*
    Given the object code it will return the instruction that is associated with it
*/
string findInstruct(string objCode)
{
    string instruct = objCode.substr(0, 2);

    if (instruct == "18" || instruct == "19" || instruct == "1A" || instruct == "1B")
        return "ADD";
    else if (instruct == "58" || instruct == "59" || instruct == "5A" || instruct == "5B")
        return "ADDF";
    else if (instruct == "90" || instruct == "91" || instruct == "92" || instruct == "93")
        return "ADDR";
    else if (instruct == "40" || instruct == "41" || instruct == "42" || instruct == "43")
        return "AND";
    else if (instruct == "B4" || instruct == "B5" || instruct == "B6" || instruct == "B7")
        return "CLEAR";
    else if (instruct == "28" || instruct == "29" || instruct == "2A" || instruct == "2B")
        return "COMP";
    else if (instruct == "88" || instruct == "89" || instruct == "8A" || instruct == "8B")
        return "COMPF";
    else if (instruct == "A0" || instruct == "A1" || instruct == "A2" || instruct == "A3")
        return "COMPR";
    else if (instruct == "24" || instruct == "25" || instruct == "26" || instruct == "27")
        return "DIV";
    else if (instruct == "64" || instruct == "65" || instruct == "66" || instruct == "67")
        return "DIVF";
    else if (instruct == "9C" || instruct == "9D" || instruct == "9E" || instruct == "9F")
        return "DIVR";
    else if (instruct == "C4" || instruct == "C5" || instruct == "C6" || instruct == "C7")
        return "FIX";
    else if (instruct == "C0" || instruct == "C1" || instruct == "C2" || instruct == "C3")
        return "FLOAT";
    else if (instruct == "F4" || instruct == "F5" || instruct == "F6" || instruct == "F7")
        return "HIO";
    else if (instruct == "3C" || instruct == "3D" || instruct == "3E" || instruct == "3F")
        return "J";
    else if (instruct == "30" || instruct == "31" || instruct == "32" || instruct == "33")
        return "JEQ";
    else if (instruct == "34" || instruct == "35" || instruct == "36" || instruct == "37")
        return "JGT";
    else if (instruct == "38" || instruct == "39" || instruct == "3A" || instruct == "3B")
        return "JLT";
    else if (instruct == "48" || instruct == "49" || instruct == "4A" || instruct == "4B")
        return "JSUB";
    else if (instruct == "00" || instruct == "01" || instruct == "02" || instruct == "03")
        return "LDA";
    else if (instruct == "68" || instruct == "69" || instruct == "6A" || instruct == "6B")
        return "LDB";
    else if (instruct == "50" || instruct == "51" || instruct == "52" || instruct == "53")
        return "LDCH";
    else if (instruct == "70" || instruct == "71" || instruct == "72" || instruct == "73")
        return "LDF";
    else if (instruct == "08" || instruct == "09" || instruct == "0A" || instruct == "0B")
        return "LDL";
    else if (instruct == "6C" || instruct == "6D" || instruct == "6E" || instruct == "6F")
        return "LDS";
    else if (instruct == "74" || instruct == "75" || instruct == "76" || instruct == "77")
        return "LDT";
    else if (instruct == "04" || instruct == "05" || instruct == "06" || instruct == "07")
        return "LDX";
    else if (instruct == "D0" || instruct == "D1" || instruct == "D2" || instruct == "D3")
        return "LPS";
    else if (instruct == "20" || instruct == "21" || instruct == "22" || instruct == "23")
        return "MUL";
    else if (instruct == "60" || instruct == "61" || instruct == "62" || instruct == "63")
        return "MULF";
    else if (instruct == "98" || instruct == "99" || instruct == "9A" || instruct == "9B")
        return "MULR";
    else if (instruct == "C8" || instruct == "C9" || instruct == "CA" || instruct == "CB")
        return "NORM";
    else if (instruct == "44" || instruct == "45" || instruct == "46" || instruct == "47")
        return "OR";
    else if (instruct == "D8" || instruct == "D9" || instruct == "DA" || instruct == "DB")
        return "RD";
    else if (instruct == "AC" || instruct == "AD" || instruct == "AE" || instruct == "AF")
        return "RMO";
    else if (instruct == "4C" || instruct == "4D" || instruct == "4E" || instruct == "4F")
        return "RSUB";
    else if (instruct == "A4" || instruct == "A5" || instruct == "A6" || instruct == "A7")
        return "SHIFTL";
    else if (instruct == "A8" || instruct == "A9" || instruct == "AA" || instruct == "AB")
        return "SHIFTR";
    else if (instruct == "F0" || instruct == "F1" || instruct == "F2" || instruct == "F3")
        return "SIO";
    else if (instruct == "EC" || instruct == "ED" || instruct == "EE" || instruct == "EF")
        return "SSK";
    else if (instruct == "0C" || instruct == "0D" || instruct == "0E" || instruct == "0F")
        return "STA";
    else if (instruct == "78" || instruct == "79" || instruct == "7A" || instruct == "7B")
        return "STB";
    else if (instruct == "54" || instruct == "55" || instruct == "56" || instruct == "57")
        return "STCH";
    else if (instruct == "80" || instruct == "81" || instruct == "82" || instruct == "83")
        return "STF";
    else if (instruct == "D4" || instruct == "D5" || instruct == "D6" || instruct == "D7")
        return "STI";
    else if (instruct == "14" || instruct == "15" || instruct == "16" || instruct == "17")
        return "STL";
    else if (instruct == "7C" || instruct == "7D" || instruct == "7E" || instruct == "7F")
        return "STS";
    else if (instruct == "E8" || instruct == "E9" || instruct == "EA" || instruct == "EB")
        return "STSW";
    else if (instruct == "84" || instruct == "85" || instruct == "86" || instruct == "87")
        return "STT";
    else if (instruct == "10" || instruct == "11" || instruct == "12" || instruct == "13")
        return "STX";
    else if (instruct == "1C" || instruct == "1D" || instruct == "1E" || instruct == "1F")
        return "SUB";
    else if (instruct == "5C" || instruct == "5D" || instruct == "5E" || instruct == "5F")
        return "SUBF";
    else if (instruct == "94" || instruct == "95" || instruct == "96" || instruct == "97")
        return "SUBR";
    else if (instruct == "B0" || instruct == "B1" || instruct == "B2" || instruct == "B3")
        return "SVC";
    else if (instruct == "E0" || instruct == "E1" || instruct == "E2" || instruct == "E3")
        return "TD";
    else if (instruct == "F8" || instruct == "F9" || instruct == "FA" || instruct == "FB")
        return "TIO";
    else if (instruct == "2C" || instruct == "2D" || instruct == "2E" || instruct == "2F")
        return "TIX";
    else if (instruct == "B8" || instruct == "B9" || instruct == "BA" || instruct == "BB")
        return "TIXR";
    else if (instruct == "DC" || instruct == "DD" || instruct == "DE" || instruct == "DF")
        return "WD";

    return NULL;
}

/*
    Returns an integer eith 0 or 1 to say that the flag is either
    not set or is set from the object code for indirect addressing
*/
int nFlag(string objCode)
{

    if (objCode.substr(1, 1) == "B" || objCode.substr(1, 1) == "3" || objCode.substr(1, 1) == "7" || objCode.substr(1, 1) == "F")           //n and i bits triggered
        return 1;
    else if (objCode.substr(1, 1) == "A" || objCode.substr(1, 1) == "2" || objCode.substr(1, 1) == "6" || objCode.substr(1, 1) == "E")      //n bit triggered
        return 1;
    else
        return 0;
}

/*
    Returns an integer eith 0 or 1 to say that the flag is either
    not set or is set from the object code for immediate addressing
*/
int iFlag(string objCode)
{
    if (objCode.substr(1, 1) == "B" || objCode.substr(1, 1) == "3" || objCode.substr(1, 1) == "7" || objCode.substr(1, 1) == "F")           //n and i bits triggered
        return 1;
    else if (objCode.substr(1, 1) == "9" || objCode.substr(1, 1) == "1" || objCode.substr(1, 1) == "5" || objCode.substr(1, 1) == "D")          //i bit triggered
        return 1;
    else
        return 0;
}

/*
    Returns an integer eith 0 or 1 to say that the flag is either
    not set or is set from the object code for index addressing
*/
int xFlag(string objCode)
{
    switch (objCode[3])
    {
    case '0':
        return 0;
    case '1':
        return 0;
    case '2':
        return 0;
    case '3':
        return 0;
    case '4':
        return 0;
    case '5':
        return 0;
    case '6':
        return 0;
    case '7':
        return 0;

    case '8':
        return 1;
    case '9':
        return 1;
    case 'A':
        return 1;
    case 'B':
        return 1;
    case 'C':
        return 1;
    case 'D':
        return 1;
    case 'E':
        return 1;
    case 'F':
        return 1;
    }

    return NULL;
}

/*
    Returns an integer eith 0 or 1 to say that the flag is either
    not set or is set from the object code for base-relative addressing
*/
int bFlag(string objCode)
{
    switch (objCode[3])
    {
    case '0':
        return 0;
    case '1':
        return 0;
    case '2':
        return 0;
    case '3':
        return 0;
    case '8':
        return 0;
    case '9':
        return 0;
    case 'A':
        return 0;
    case 'B':
        return 0;

    case '4':
        return 1;
    case '5':
        return 1;
    case '6':
        return 1;
    case '7':
        return 1;
    case 'C':
        return 1;
    case 'D':
        return 1;
    case 'E':
        return 1;
    case 'F':
        return 1;
    }

    return NULL;
}

/*
    Returns an integer eith 0 or 1 to say that the flag is either
    not set or is set from the object code for PC-relative addressing
*/
int pFlag(string objCode)
{
    switch (objCode[3])
    {
    case '0':
        return 0;
    case '1':
        return 0;
    case '4':
        return 0;
    case '5':
        return 0;
    case '8':
        return 0;
    case '9':
        return 0;
    case 'C':
        return 0;
    case 'D':
        return 0;

    case '2':
        return 1;
    case '3':
        return 1;
    case '6':
        return 1;
    case '7':
        return 1;
    case 'A':
        return 1;
    case 'B':
        return 1;
    case 'E':
        return 1;
    case 'F':
        return 1;
    }

    return NULL;
}

/*
    Returns an integer eith 0 or 1 to say that the flag is either
    not set or is set from the object code for extended format
*/
int eFlag(string objCode)
{
    switch (objCode[3])
    {
    case '0':
    case '2':
    case '4':
    case '6':
    case '8':
    case 'A':
    case 'C':
    case 'E':
        return 0;

    case '1':
        return 1;
    case '3':
        return 1;
    case '5':
        return 1;
    case '7':
        return 1;
    case '9':
        return 1;
    case 'B':
        return 1;
    case 'D':
        return 1;
    case 'F':
        return 1;
    }

    return NULL;
}

int main(int argc, char* argv[])
{
    ifstream objFile;
    ifstream symFile;
    string filename = argv[1];

    int i = 0;
    for (; filename[i] != '.'; i++);                                                        //Finds file extension and replaces deletes it

    filename = filename.substr(0, i);

                                                                                            //open object file. If not present, throw an error and exit
    objFile.open(filename + ".obj", ios::in);                                               
    if (!objFile)
    {
        DebugMessage("Error has occurred. No object file detected");
        cout << "Error has occurred. No object file detected";
        return 0;
    }


                                                                                            //open symbol file. If not present, throw an error and exit
    symFile.open(filename + ".sym", ios::in);                                              
    if (!symFile)
    {
        DebugMessage("Error has occurred. No symbol file detected");
        cout << "Error has occurred. No symbol file detected";
        return 0;
    }

    ofstream sicFile(filename + ".sic", ios::binary);                                       //Opens .sic file to write to
    ofstream listFile(filename + ".lis", ios::binary);                                      //Opens .lis file to write to

    fileWriter(sicFile, listFile, objFile, symFile);

    //close the files
    objFile.close();
    symFile.close();
    sicFile.close();
    listFile.close();
    return 0;
}