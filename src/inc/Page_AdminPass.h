/*
 * File:   Page_AdminPass.h
 * Author: Mahdi Razavi...................(Razavi.Dev@gmail.com)
 *
 * Created on: 	Apr 27, 2015
 * Edit Date :	
 * 				Apr 27, 2015	
 */

#ifndef PAGE_ADMINPASS_H_
#define PAGE_ADMINPASS_H_



#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <string.h>
#include <cmath>

#include "Page.h"

class Page_AdminPass : public Page
{
public:
  Page_AdminPass(string pageName, map<string, string> &CgiVariables, bool isPostBack);
  Page_AdminPass(CGI_Interface* cgiInstance);
  ~Page_AdminPass();

   virtual string Get_Input_Value(string inputName);

private:
};

#endif /* PAGE_ADMINPASS_H_ */
