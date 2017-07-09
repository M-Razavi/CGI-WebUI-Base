/*
 * File:   PageFactory.h
 * Author: Mahdi Razavi...................(Razavi.Dev@gmail.com)
 *
 * Created on: 	Apr 22, 2015
 * Edit Date :	
 * 				Apr 22, 2015	
 */

#ifndef PAGEFACTORY_H_
#define PAGEFACTORY_H_

#include "Page.h"
/*
 *
 */
class PageFactory {
 public:
  PageFactory();
  static Page* createPageClass(string name, map<string, string> &CgiVariables, bool isPostBack);
  static Page* createPageClass(string name,map<string, string> &CgiVariables, bool isPostBack, CGI_Interface* cgiInstance);
  virtual ~PageFactory();
};

#endif /* PAGEFACTORY_H_ */
