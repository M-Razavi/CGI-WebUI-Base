/*
 * File:   PageFactory.cpp
 * Author: Mahdi Razavi...................(Razavi.Dev@gmail.com)
 *
 * Created on: 	Apr 22, 2015
 * Edit Date :	
 * 				Apr 22, 2015	
 */

#include "Page.h"
#include "PageFactory.h"
#include "Page_Date.h"
#include "Page_AdminPass.h"
#include "Page_Restart.h"

PageFactory::PageFactory() {

}

PageFactory::~PageFactory() {
}

Page* PageFactory::createPageClass(string name,
                                   map<string, string> &CgiVariables,
                                   bool isPostBack) {
  if (name == "date")
  {
    return new Page_Date("date", CgiVariables, isPostBack);
  }
  else if (name == "admin")
  {
    return new Page_AdminPass("admin", CgiVariables, isPostBack);
  }
  else if (name == "restart")
  {
    return new Page_Restart("restart", CgiVariables, isPostBack);
  }

 return new Page_Net("lan", CgiVariables, isPostBack);

}

Page* PageFactory::createPageClass(string name,map<string, string> &CgiVariables, bool isPostBack, CGI_Interface* cgiInstance) {

    if (name == "lan")
    {
      return new Page_Net("lan", CgiVariables, isPostBack);
    }
    else if (name == "date")
    {
      return new Page_Date("date", CgiVariables, isPostBack);
    }
    else if (name == "admin")
    {
      return new Page_AdminPass("admin", CgiVariables, isPostBack);
    }
    else if (name == "restart")
    {
      return new Page_Restart("restart", CgiVariables, isPostBack);
    }

  return new Page_Net("lan", CgiVariables, isPostBack);
}
