/*
 * File:   Page_Date.cpp
 * Author: Mahdi Razavi...................(Razavi.Dev@gmail.com)
 *
 * Created on: 	Apr 27, 2015
 * Edit Date :	
 * 				Apr 27, 2015	
 */


#include <stdio.h>
#include <stdlib.h>

#include "Page_Date.h"

Page_Date::Page_Date(string pageName, map<string, string> &CgiVariables,
                         bool isPostBack)
    : Page(pageName, CgiVariables, isPostBack) {
  this->pageFileName = pageName;
  //this->CgiVariables = CgiVariables;
}

Page_Date::~Page_Date() {

}

/**
 * Get input tag name then return tag value or perform a operation based on.
 * this function manage input value
 * !name.compare("[name of select tag]") ==> this code detect now you manage this input
 * FetchItems[i].compare("NULL") ==> this code detect FetchItems[i] has value or not
 * @param name
 * @return
 */
string Page_Date::Get_Input_Value(string inputName) {

  /*--------------------------------------------------ac_tiye input---------------------------------------------------------------------------------------*/
  if (!inputName.compare("ac_tiye")) {
    if (CgiVariables.find("ac_tiye") != CgiVariables.end()
        && CgiVariables.find("ac_timo") != CgiVariables.end()
        && CgiVariables.find("ac_tida") != CgiVariables.end()
        && CgiVariables.find("ac_tiho") != CgiVariables.end()
        && CgiVariables.find("ac_timi") != CgiVariables.end())

        {
      if (!validateInput(witNumeric, CgiVariables["ac_tiye"])
          && !validateInput(witNumeric, CgiVariables["ac_timo"])
          && !validateInput(witNumeric, CgiVariables["ac_tida"])
          && !validateInput(witNumeric, CgiVariables["ac_tiho"])
          && !validateInput(witNumeric, CgiVariables["ac_timi"])) {
        ostringstream setDateOS;
        setDateOS << "su -c \"date -s " << CgiVariables["ac_tiye"] << "."
                  << CgiVariables["ac_timo"] << "." << CgiVariables["ac_tida"]
                  << "-" << CgiVariables["ac_tiho"] << ":"
                  << CgiVariables["ac_timi"] << "\"";
        string setdatetmp = setDateOS.str();
        const char * sdt = setdatetmp.c_str();
        systemExec(sdt);

        ostringstream setDateApp;
        setDateApp << "su -c \"/bin/rtc -s " << CgiVariables["ac_tiye"] << " "
                   << CgiVariables["ac_timo"] << " " << CgiVariables["ac_tida"]
                   << " " << CgiVariables["ac_tiho"] << " "
                   << CgiVariables["ac_timi"] << " 0 " << "\"";
        const char * sdtApp = setDateApp.str().c_str();
        systemExec(sdtApp);

      } else {
        return " ";
      }
    }
    return " ";
  }

  return "";
}


/**
 * Get defined HTML tag name and generate a needed HTML structure based on.
 * this function manage html value.
 * !name.compare("[name of html generator tag]") ==> this code detect now you manage this html value
 * @param name
 * @return
 */
string Page_Date::Get_Html_Value(string name) {
  if (!name.compare("toyear")) {
    return exec(const_cast<char *>("date -R | cut -d\" \" -f4 "));
  } else if (!name.compare("tomonth")) {
    return exec(const_cast<char *>("date -R | cut -d\" \" -f3 "));
  } else if (!name.compare("today")) {
    return exec(const_cast<char *>("date -R | cut -d\" \" -f2 "));
  } else if (!name.compare("tohour")) {
    return exec(const_cast<char *>("date -R | cut -d\" \" -f5 | cut -d: -f1 "));
  } else if (!name.compare("tominute")) {
    return exec(const_cast<char *>("date -R | cut -d\" \" -f5 | cut -d: -f2 "));
  }

  return "";
}




