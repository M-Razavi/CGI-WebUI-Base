/*
 * File:   Page_AdminPass.cpp
 * Author: Mahdi Razavi...................(Razavi.Dev@gmail.com)
 *
 * Created on: 	Apr 27, 2015
 * Edit Date :	
 * 				Apr 27, 2015	
 */


#include <stdio.h>
#include <stdlib.h>

#include "Page_AdminPass.h"

Page_AdminPass::Page_AdminPass(string pageName, map<string, string> &CgiVariables,
                         bool isPostBack)
    : Page(pageName, CgiVariables, isPostBack) {
  this->pageFileName = "admin";
}

Page_AdminPass::~Page_AdminPass() {

}

/**
 * Get input tag name then return tag value or perform a operation based on.
 * this function manage input value
 * !name.compare("[name of select tag]") ==> this code detect now you manage this input
 * FetchItems[i].compare("NULL") ==> this code detect FetchItems[i] has value or not
 * @param name
 * @return
 */
string Page_AdminPass::Get_Input_Value(string inputName) {
  /*--------------------------------------------------newpasswd, confirmpasswd input---------------------------------------------------------------------------------------*/
    if (!inputName.compare("newpasswd"))  // || !name.compare("confirmpasswd")
        {
      if (CgiVariables.find("newpasswd") != CgiVariables.end()
          && CgiVariables.find("confirmpasswd") != CgiVariables.end())  //FetchItems[17].compare( "NULL") && FetchItems[18].compare( "NULL"))
              {
        if (!CgiVariables["newpasswd"].compare(CgiVariables["confirmpasswd"])) {
          if (CgiVariables["newpasswd"].compare("")) {
            //          int isAlphaNum = 1;
            //          int strToMatchSize = 0;
            //          //          while (strToMatch[strToMatchSize] != '\0' && isalnum(strToMatch[strToMatchSize++]))
            //          //          {
            //          //            isAlphaNum = 0;
            //          //          }
            int rc;
            if ((rc = validateInput(witPassword, CgiVariables["newpasswd"]))
                == 0) {
              //            //              htPasswd << "su -c \"rm -rf /www/.htpasswd;/sbin/htpasswd -c /www/.htpasswd admin " << FetchItems[17] << ";\"";
              string htPasswd =
                  "su -c \"rm -rf /www/.htpasswd;/sbin/htpasswd -c /www/.htpasswd admin "
                      + CgiVariables["newpasswd"] + ";\"";
              int cmdResult = systemExec(htPasswd.c_str());
              writeDebugLog("Change password=", cmdResult, __func__, __LINE__);
              return CgiVariables["newpasswd"];
            } else {
              string debugMessage = "Invalid characters in password="
                  + CgiVariables["newpasswd"];
              writeDebugLog(debugMessage, -1, __func__, __LINE__);
              return "Error : Invalid characters!.";
            }
            return "ok";
          } else {
            writeDebugLog("Error : Password not be empty!", -1, __func__,
                          __LINE__);
            return "Error : Password not not be empty!";
          }
        } else {
          writeDebugLog("Error : Password not equal confirm!", -1, __func__,
                        __LINE__);
          return "Error : Password not equal confirm!";
        }
      } else {
        return "";
      }
    }
  return "";
}





