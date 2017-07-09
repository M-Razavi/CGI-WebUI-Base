/*
 * File:   RzvUtility.cpp
 * Author: Mahdi Razavi...................(Mahdi Razavi@mail.com)
 *
 * Created on: 	Jun 23, 2013
 * Edit Date :	
 * 				Jun 23, 2013	
 */

#include <string>
#include <iostream>
#include <sstream>
#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <algorithm>
#include <iterator>
#include <regex.h>

#include <RzvUtility.h>

using namespace std;

/**
 * Split input string 's' into string vector by string 'delim' delimiter
 * @param s	input string
 * @param delim delimiter string
 * @param keep_empty keep empty element
 * @return
 */
vector< string> split( const string& s, const string& delim,
						const bool keep_empty)
{
	vector< string> result;
	if( delim.empty())
	{
		result.push_back( s);
		return result;
	}
	string::const_iterator substart = s.begin(), subend;
	while( true)
	{
		subend = search( substart, s.end(), delim.begin(), delim.end());
		string temp( substart, subend);
		if( keep_empty || !temp.empty())
		{
			result.push_back( temp);
		}
		if( subend == s.end())
		{
			break;
		}
		substart = subend + delim.size();
	}
	return result;
}

/**
 * ConvertStringToChar
 */
char* ConvertStringToChar(string str,int chrSize)
{
	char chrOut[chrSize];
	return strncpy(chrOut, str.c_str(), chrSize);
}

void ConvertStringToChar(string str, char chr[], int chrSize)
{
	//char chrOut[chrSize];
	strncpy(chr, str.c_str(), chrSize);
	//memcpy( chr, str.c_str(), strlen( str.c_str()) + 1);
}

/**
 * Get Current Date and Time in format yyyy-mm-dd hh:mm
 * @return
 */
string GetCurrentDateTime() {
  time_t theTime = time(NULL);
  struct tm *aTime = localtime(&theTime);

  int day = aTime->tm_mday;
  int month = aTime->tm_mon + 1;  // Month is 0 - 11, add 1 to get a jan-dec 1-12 concept
  int year = aTime->tm_year + 1900;  // Year is # years since 1900

  stringstream tmpDateTime;
  tmpDateTime << year << "-" << month << "-" << day << " " << aTime->tm_hour
              << ":" << aTime->tm_min;
  return tmpDateTime.str();
}

/**
 * Write debug message to log file.
 * @param debugStrMessage
 * @param code
 * @param functionName
 * @param lineNumber
 * @param isDbLog
 */
void writeDebugLog(string debugStrMessage, int code, string functionName,
                   int lineNumber, bool isDbLog) {
  string debugFilePath = "/www/log/webInterfaceDebug.log";

  ofstream log_file(debugFilePath.c_str(), ios_base::out | ios_base::app);
  log_file << GetCurrentDateTime() << "\tFunction=" << functionName << "\tLine="
           << lineNumber << "\t" << debugStrMessage << "\tResult=" << code;
  if (isDbLog) {
    log_file << "Db-Desc=" << db_errstr(code);
  }
  log_file << "\n";
  log_file.close();
}

/**
 * Load a HTML file into engine
 * @param file
 * @return
 */
string read_txt_file(const char *file) {
  ifstream ifs(file);
  string content((istreambuf_iterator<char>(ifs)),
                 (istreambuf_iterator<char>()));
  return content;
}

/**
 * Search in the string and replace with another string
 * @param String
 * @param searchString
 * @param replaceString
 * @param pos
 * @return
 */
string search_replace(string String, string searchString, string replaceString,
                      string::size_type pos) {
  while ((pos = String.find(searchString, pos)) != string::npos) {
    String.replace(pos, searchString.size(), replaceString);
    pos += replaceString.size();
  }
  return String;
}

/**
 * Execute a system command in OS shell.
 * @param cmd
 * @return
 */
int systemExec(const char* cmd) {
  int cmdResult = system(cmd);
  writeDebugLog(cmd, cmdResult, __func__, __LINE__);
  return cmdResult;
}

/**
 * Execute a system command in OS shell and return result of command.
 * @param cmd
 * @return
 */
string exec(char* cmd) {
  FILE* pipeCmd = popen(cmd, "r");

  if (pipeCmd == NULL) {
    writeDebugLog(
        strcat(const_cast<char *>("Error execute command. cmd="), cmd), -1,
        __func__, __LINE__);
    return "ERROR";
  }
  char buffer[128];
  string result = "";
  while (!feof(pipeCmd)) {
    while (fgets(buffer, sizeof(buffer), pipeCmd) != NULL) {
      result.append(buffer);
    }
  }
  pclose(pipeCmd);
  return result;
}

/**
 * Check and validate input value
 * if inputType==witNumeric then only numeric value with +,- sign is accepted.
 * if inputType==witAlphabetic then only upper case or lower case alphabetic value is accepted.
 * if inputType==witAlphaNumeric then numeric or upper case or lower case alphabetic value without +,- sign is accepted.
 * if inputType==witAlphaNumericDashDot then numeric or upper case or lower case alphabetic value with +,-,_,. sign is accepted.
 * if inputType==witIpAddress then only IP address v4 value is accepted.
 * if inputType==witPassword then only strong alhpanumeric password value with these condition is accepted, password must at last has( 6 char length, 1 numeric, 1 upper case alphabetic, 1 lower case alphabetic).
 * @param inputType Determine input type
 * @param inputValue
 * @return
 */
int validateInput(WebInputType inputType, string inputValue) {
  string pattern = "";
  if (inputType == witNumeric) {
    pattern = "^\\+*-*[0-9]+$";
  } else if (inputType == witNumericDot) {
    pattern = "^\\+*-*[0-9\\.]+$";
  } else if (inputType == witAlphaNumeric) {
    pattern = "^[a-zA-Z0-9]+$";
  } else if (inputType == witAlphabetic) {
    pattern = "^[a-zA-Z]+$";
  } else if (inputType == witAlphaNumericDashDot) {
    pattern = "^[a-z0-9A-Z\\._-]+$";
  } else if (inputType == witIpAddress) {
    pattern =
        "^((25[0-5]|2[0-4][0-9]|1[0-9]{2}|[0-9]{1,2})\\.){3}(25[0-5]|2[0-4][0-9]|1[0-9]{2}|[0-9]{1,2})$";
  } else if (inputType == witPassword) {
    //pattern = "^(?=.*[A-Z])(?=.*[a-z])(?=.*\\d)(?!.*(.)\\1\\1)[a-zA-Z0-9]{6,}$"; // Error REG_BADRPT occur
    pattern = "^[a-z0-9A-Z]{6,}$";
  } else if (inputType == witHexNumeric2Digit) {
    pattern = "^([A-Fa-f0-9]{1,2})$";
  } else {
    return REG_BADPAT;
  }

  regex_t preg;
  int regResult;
  size_t nmatch = 2;
  regmatch_t pmatch[2];

  if ((regResult = regcomp(&preg, pattern.c_str(), REG_EXTENDED)) == 0) {
    if (0
        == (regResult = regexec(&preg, inputValue.c_str(), nmatch, pmatch, 0))) {
      return regResult;
    } else {
      return regResult;
    }
  }
  regfree(&preg);
  return regResult;
}







/**
 * Check and validate IP address value
 * @param ipAddress
 * @return
 */
bool validateIpAddress(const string ipAddress) {
  struct sockaddr_in sa;
  int result = inet_pton(AF_INET, ipAddress.c_str(), &(sa.sin_addr));
  return result != 0;
}

