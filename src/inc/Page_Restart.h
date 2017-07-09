/*
 * File:   Page_Restart.h
 * Author: Mahdi Razavi...................(Razavi.Dev@gmail.com)
 *
 * Created on: 	Apr 27, 2015
 * Edit Date :	
 * 				Apr 27, 2015	
 */

#ifndef PAGE_RESTART_H_
#define PAGE_RESTART_H_


#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <string.h>
#include <cmath>

#include "Page.h"

class Page_Restart : public Page
{
public:
  Page_Restart(string pageName, map<string, string> &CgiVariables, bool isPostBack);
  Page_Restart(CGI_Interface* cgiInstance);
  ~Page_Restart();

   virtual string Get_Input_Value(string inputName);
   virtual string Get_Select_Value(string name, string value);

private:
};


#endif /* PAGE_RESTART_H_ */
