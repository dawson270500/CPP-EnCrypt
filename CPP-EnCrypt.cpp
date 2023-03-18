#include <iostream>
#include <time.h> 
#include <string>
#include "FileHandle.h"

const string version = "1.1";

using namespace std;
using namespace file_handler;

/// <summary>
/// Convert a string to lower case
/// </summary>
/// <param name="LowerCase">String to make lower case</param>
/// <returns>A lower case string</returns>
string StringToLower(string LowerCase) {
    string ret = "";
    for (int i = 0; i < LowerCase.length(); i++) {
        ret += tolower(LowerCase[i]);
    }
    return ret;
}

/// <summary>
/// A very basic encryption program 
/// 
/// CPP-EnCrypt.exe -f#<file> -p#<password> [-encrypt / -decrypt] {-o#<output file>}
/// Output file is required when using decrypt mode
/// </summary
/// <returns>0 for success, 1 for failure</returns>
int main(int argc, char* argv[])
{
    cout << "EnCrypt version " + version << endl;
    string in_file = "";
    string out_file = "";
    bool enMode = NULL;
    bool enModeSet = false;
    string password = "";

    for (int i = 1; i < argc; i++) {
        string curArg = StringToLower(argv[i]);
        if (curArg.substr(0, 3) == "-f#" && in_file == "") {
            in_file = curArg.substr(curArg.find("#") + 1, curArg.length());
        }
        else if (curArg.substr(0, 3) == "-p#" && password == "") {
            password = curArg.substr(curArg.find("#") + 1, curArg.length());
        }
        else if (curArg == "-encrypt" && !enModeSet) {
            enMode = true;
            enModeSet = true;
        }
        else if (curArg == "-decrypt" && !enModeSet) {
            enMode = false;
            enModeSet = true;
        }
        else {
            cout << "Invalid arguments" << endl;
            return 1;
        }
    }

    if (in_file == "") {
        cout << "Invalid arguments, missing input file" << endl;
        return 1;
    }
    if (enModeSet == false) {
        cout << "Invalid arguments, mode missing" << endl;
        return 1;
    }
    if (password == "") {
        cout << "Invalid arguments, mode missing" << endl;
        return 1;
    }

    if (enMode && out_file == "") {
        out_file = in_file.substr(0, in_file.find_last_of(".")) + ".enc";
        cout << "Output file is " << out_file << endl;
    }

    int salt = 0;
    for (int i = 0; i < password.length(); i++) {
        salt += password[i];
    }
    cout << "Password converted to salt" << endl;

    FileHandle input = FileHandle(in_file, FileMode::read);
    cout << input.fileLen << " bytes to process" << endl;
    if (!input.open) {
        cout << "Error with input file, see above" << endl;
        return 1;
    }
    if (!enMode) {
        string hold = input.getLine_file();
        if (hold.find("=-FILE-=") != string::npos) {
            out_file = hold.substr(hold.find_last_of("=")+1, hold.length() - 1);
            cout << "Output file is " << out_file << endl;
        }
        else {
            cout << "Invalid input file for decrypting";
            return 1;
        }
    }
    
    FileHandle output = FileHandle(out_file, FileMode::write);
    
    if (!output.open) {
        cout << "Error with output file, see above" << endl;
        return 1;
    }
    if(enMode) {
        output.writeLine("=-FILE-=" + in_file);
    }
    time_t start = std::time(nullptr);
    for (int i = 0; i < input.fileLen; i++) {
        if (i % 4 == 0) {
            cout << "Completed " << i << " bytes" << endl;
        }
        char out = 0;
        if (!input.getbyte(&out)) {
            if (input.CurFilePos == -1) { // If we hit end of file
                break;
            }

            cout << "Failed to read from file at byte " << i << endl;
            input.close();
            output.close();
            return 1;
        }
        if(enMode)
            out = (int)out + salt;
        else
            out = (int)out - salt;
        if (!output.writebyte(out)) {
            cout << "Failed to write to file at byte " << i << endl;
            input.close();
            output.close();
            return 1;
        }
    }
    time_t end = time(0);
    input.close();
    output.close();
    cout << "Successfully ran, took " << start - end << " seconds" << endl;
    return 0;
}