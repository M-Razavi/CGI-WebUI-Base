/*
 * File:   main.cpp
 * Author: Mahdi Razavi...................(Razavi.Dev@gmail.com)
 *
 * Created on: 	Apr 22, 2015
 * Edit Date :	
 * 				Apr 22, 2015	
 */
 
#include <fstream>
#include <string>
#include <iostream>
#include <sstream>

#include <linux/stddef.h>   // NULL
#include <stdio.h>          // printf
#include <stdlib.h>         // exit
#include <string.h>         // snprintf
#include <stdint.h>
#include <arpa/inet.h>
#include <regex.h>
#include <algorithm>
#include <functional>
#include <cctype>
#include <locale>
#include <map>
#include <cmath>
#include <vector>

#include <time.h>


#include "cgi_interface.h"
#include "RzvUtility.h"
#include "Page.h"
#include "PageFactory.h"


using namespace std;

/**
 * map staucture for save POST and GET parameter
 */
map<string, string> CgiVariables;


/**
 * count of maximum filed number in the web interface.
 */
//const int MAXITEMS = 48;


/**
 * array for save POST or GET parameter
 */
//string FetchItems[48];

/**
 * Checking if request is post back
 */
bool isPostBack;

/**
 * Convert number to string
 * Usage: NumberToString ( Number );
 * @param Number
 * @return

template<typename T>
string NumberToString(T Number) {
  ostringstream ss;
  ss << Number;
  return ss.str();
}

*
 * Convert string to number
 * Usage: StringToNumber<Type> ( String );
 * @param Text
 * @return

template<typename T>
T StringToNumber(const string &Text) {
  istringstream ss(Text);
  T result;
  return ss >> result ? result : 0;
}*/



/**
 * Show error message in HTML format.
 * @param message
 */
void showErrorMessage(string message) {
  cout
      << "</br><span style=\"background-color:#ff0000;\">" + message
          + "</span></br>";
}






void validateInputParameters() {
  int validateResult = 0;
  for (map<string, string>::iterator ii = CgiVariables.begin();
      ii != CgiVariables.end(); ++ii) {
//    string debug = "-----item[" + (*ii).first + "] ";
//          debug.append((*ii).second);
//
//          writeDebugLog(debug, validateResult, __func__, __LINE__, false);

    if ((validateResult = validateInput(witAlphaNumericDashDot, (*ii).second))
        == 0) {
      (*ii).second = trim((*ii).second);
    } else {
      string debugMessage = "Error: invalid input. item[" + (*ii).first + "] ";
      debugMessage.append((*ii).second);

      writeDebugLog(debugMessage, validateResult, __func__, __LINE__, false);
      (*ii).second = "INVALID";
    }
  }
}

/**
 * Main function
 * @return
 */
int main(int argc, char *argv[]) {
  if (argc > 1) {
    if (!strcmp(argv[1], "-v"))  // Show WebInterface Version
                {
      cout << "CGI WebInterface Ver 1.4.0 - compile time:" << __DATE__ << " "
           << __TIME__ << endl;
      return 0;
    } else if (!strcmp(argv[1], "-s"))  // Run WebInterface through OS shell
                       {
      cout << "Shell Mode.\n";
      int setEnvResult = setenv("REQUEST_METHOD", "GET", 1);
      if (setEnvResult == 0) {
        setEnvResult = setenv("QUERY_STRING", argv[2], 1);
        if (setEnvResult == 0) {
          cout << "Method=GET. Parameters=" << argv[2] << endl;
        }
      }

      if (setEnvResult != 0) {
        cout << "Can not set environment variable.";
        return -1;
      }
    } else if (!strcmp(argv[1], "-h"))  // Show Help
                       {
      cout
          << "CGI Web Interface.\n -v : show version.\n -s : shell mode. put QUERY_STRING after -s. like \" ./index.cgi -s page=channels\\&xyz=123 > /share/pgChannels.log \"\n";
      return 0;
    }
  }

  CGI_Interface *cgiInstance = new CGI_Interface(true);
  int autoLoadRes = cgiInstance->AutoLoad();
  if (autoLoadRes < 0) {
    cout << "Error in CGI Variables Load." << autoLoadRes;
  }

  // Get Value Of Post And Get Method
  //fetch map of request parameter (POST, GET)
  cgiInstance->FetchAllVariable(CgiVariables);

  isPostBack = cgiInstance->IsPostBack();

  // If parameters has no value , the value return "NULL" as string
  /**
   * ========================================================= Validate Input Parameters ====================================================
   */
  validateInputParameters();



  /**
   * ========================================================= Initialize DataBase ==========================================================
   */

    //db = Db_Open_Database();


  /**
   * ========================================================= print page content to output =================================================
   */


  string pageFileName = CgiVariables["page"];
  Page* page = PageFactory::createPageClass(pageFileName, CgiVariables, isPostBack,cgiInstance);

  page->Show();

  /**
   * ========================================================= Check Input =================================================
   */

  // for debug
  // cgiInstance->ShowEnviromentVariables(true);
  // cgiInstance->ShowEnviromentVariables(false);
  // cgiInstance->PrintDump();


  Db_Close_Database(db);
  delete cgiInstance;
  return 1;
}
