/*
 * File:   RzvUtility.h
 * Author: Mahdi Razavi...................(Mahdi Razavi@mail.com)
 *
 * Created on: 	Jun 23, 2013
 * Edit Date :	
 * 				Jun 23, 2013	
 */

#ifndef RZVUTILITY_H_
#define RZVUTILITY_H_

#include <string>
#include <iostream>
#include <sstream>
#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <algorithm>
#include <iterator>

#include <fstream>
#include <string>
#include <iostream>
#include <sstream>

#include <linux/stddef.h>   // NULL
#include <stdio.h>          // printf
#include <stdlib.h>         // exit
#include <string.h>         // snprintf
#include "db/rdb.h"
#include "db/db.h"
#include <stdint.h>

#include <string.h>

using namespace std;

/**
 * Web input types
 */
enum WebInputType {
  witNumeric,  //!< witNumeric Determine numeric values.
  witNumericDot,  //!< witNumericDot Determine integer and decimal numeric values.
  witAlphabetic,  //!< witAlphabetic Determine alphabetic values.
  witAlphaNumeric,  //!< witAlphaNumeric Determine alpha,numeric values.
  witAlphaNumericDashDot,  //!< witAlphaNumericDashDot Determine alpha,numeric,dash,dot,underline values.
  witIpAddress,  //!< witIpAddress Determine alphabetic values.
  witPassword,  //!< witPassword Determine password values.
  witHexNumeric2Digit  //!< witHexNumeric Determine hexadecimal values.
};


/**
 * convert string to char*
 * NOT TESTED
 */
char* ConvertStringToChar(string str,int chrSize);

/**
 * convert string to char[]
 * NOT TESTED
 */
void ConvertStringToChar(string str, char chr[], int chrSize);

vector<string> split(const string& s, const string& delim, const bool keep_empty = true);

string GetCurrentDateTime();

void writeDebugLog(string debugStrMessage, int code, string functionName,
                   int lineNumber, bool isDbLog = true);

string read_txt_file(const char *file);

string search_replace(string String, string searchString, string replaceString, string::size_type pos = 0);

int systemExec(const char* cmd);

string exec(char* cmd);

int validateInput(WebInputType inputType, string inputValue);

/**
 * Convert number to string
 * Usage: NumberToString ( Number );
 * @param Number
 * @return
 */
template<typename T>
string NumberToString(T Number) {
  ostringstream ss;
  ss << Number;
  return ss.str();
}

/**
 * Convert string to number
 * Usage: StringToNumber<Type> ( String );
 * @param Text
 * @return
 */
template<typename T>
T StringToNumber(const string &Text) {
  istringstream ss(Text);
  T result;
  return ss >> result ? result : 0;
}



/**
 * trim from start
 * @param s
 * @return
 */
inline string &ltrim(string &s) {
  s.erase(
      s.begin(),
      find_if(s.begin(), s.end(),
                   not1(ptr_fun<int, int>(isspace))));
  return s;
}

/**
 * trim from end
 * @param s
 * @return
 */
inline string &rtrim(string &s) {
  s.erase(
      find_if(s.rbegin(), s.rend(),
                   not1(ptr_fun<int, int>(isspace))).base(),
      s.end());
  return s;
}

/**
 *  trim from start and end
 * @param s
 * @return
 */
inline string &trim(string &s) {
  return ltrim(rtrim(s));
}



bool validateIpAddress(const string ipAddress);


#endif /* RZVUTILITY_H_ */
