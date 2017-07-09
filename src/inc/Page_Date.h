/*
 * File:   Page_Date.h
 * Author: Mahdi Razavi...................(Razavi.Dev@gmail.com)
 *
 * Created on: 	Apr 27, 2015
 * Edit Date :	
 * 				Apr 27, 2015	
 */

#ifndef PAGE_DATE_H_
#define PAGE_DATE_H_



#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <string.h>
#include <cmath>

#include "Page.h"

class Page_Date : public Page
{
public:
  Page_Date(string pageName, map<string, string> &CgiVariables, bool isPostBack);
  Page_Date(CGI_Interface* cgiInstance);
  ~Page_Date();

   virtual string Get_Html_Value(string name);
   virtual string Get_Input_Value(string inputName);

private:
};

#endif /* PAGE_DATE_H_ */
