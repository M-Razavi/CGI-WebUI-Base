/*
 * File:   Page_Restart.cpp
 * Author: Mahdi Razavi...................(Razavi.Dev@gmail.com)
 *
 * Created on: 	Apr 27, 2015
 * Edit Date :	
 * 				Apr 27, 2015	
 */

#include <stdio.h>
#include <stdlib.h>

#include "Page_Restart.h"

Page_Restart::Page_Restart(string pageName, map<string, string> &CgiVariables,
                           bool isPostBack)
    : Page(pageName, CgiVariables, isPostBack) {
  this->pageFileName = pageName;
  //this->CgiVariables = CgiVariables;
}

Page_Restart::~Page_Restart() {

}

/**
 * Get input tag name then return tag value or perform a operation based on.
 * this function manage input value
 * !name.compare("[name of select tag]") ==> this code detect now you manage this input
 * FetchItems[i].compare("NULL") ==> this code detect FetchItems[i] has value or not
 * @param name
 * @return
 */
string Page_Restart::Get_Input_Value(string inputName) {

  //if reboot button pressed
  if (CgiVariables.find("reboot") != CgiVariables.end())    // if found
      {
    if (CgiVariables.find("lanip") != CgiVariables.end())  // if found and if IP address changed
        {
      string javaScriptRedirect =
          "<script language=\"JavaScript\">setTimeout( window.location.href =\"http://"
              + CgiVariables["lanip"] + "\", 10000 );</script>";
      cout << javaScriptRedirect;
    }
    systemExec("sh /www/cgi-bin/rebootDevice.sh;");
  }

  return "";
}

/**
 * Get select tag, radio group tag name then return tag value or perform a operation based on.
 * this function manage select value
 * !name.compare("[name of select tag]") ==> this code detect now you manage this select
 * FetchItems[i].compare("NULL") ==> this code detect FetchItems[i] has value or not
 * @param name
 * @param value
 * @return
 */
string Page_Restart::Get_Select_Value(string name, string value) {

  if (!name.compare("reboot")) {
    if (CgiVariables.find("reboot") != CgiVariables.end())
        {
      systemExec("sh /www/cgi-bin/rebootDevice.sh;");
    }
  }

  return " ";
}




