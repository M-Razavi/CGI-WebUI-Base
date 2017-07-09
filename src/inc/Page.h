/*
 * File:   Page.h
 * Author: Mahdi Razavi...................(Razavi.Dev@gmail.com)
 *
 * Created on: 	Apr 22, 2015
 * Edit Date :	
 * 				Apr 22, 2015	
 */

#ifndef PAGE_H_
#define PAGE_H_


#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <map>

#include <RzvUtility.h>
#include <cgi_interface.h>


using namespace std;

class Page
{
public:
  Page(string pageName, map<string, string> &CgiVariables, bool isPostBack);
  Page(CGI_Interface* cgiInstance);
  virtual ~Page();
  virtual void Show();

protected:
  map<string, string> CgiVariables;
  bool isPostBack;
  CGI_Interface* cgiInstance;
  string pageFileName;
  string loadPageFile(string fileName);

private:

  string generate_html(string html);
  string generate_select(string html);
  string generate_input(string html);

  /**
   * show HTTP header for send to browser
   * @return content
   */
  virtual void Show_HttpHeader();

  /**
   * Show custom html content which is not common  implemented tag
   * * Use this function when there is no html file to show
   * @return content
   */
  virtual void Show_CustomHtml();


  /**
   * Get select tag, radio group tag name then return tag value or perform a operation based on.
   * this function manage select value
   * !name.compare("[name of select tag]") ==> this code detect now you manage this select
   * FetchItems[i].compare("NULL") ==> this code detect FetchItems[i] has value or not
   * @param name
   * @param value
   * @return
   */
  virtual string Get_Select_Options(string name);

  /**
   * Get select options and generate option tag for select.
   * @param name
   * @return
   */
  virtual string Get_Select_Value(string name, string value);

  /**
   * Get defined HTML tag name and generate a needed HTML structure based on.
   * this function manage html value.
   * !name.compare("[name of html generator tag]") ==> this code detect now you manage this html value
   * @param name
   * @return
   */
  virtual string Get_Html_Value(string name);

  /**
   * Get input tag name then return tag value or perform a operation based on.
   * this function manage input value
   * !name.compare("[name of select tag]") ==> this code detect now you manage this input
   * FetchItems[i].compare("NULL") ==> this code detect FetchItems[i] has value or not
   * @param name
   * @return
   */
  virtual string Get_Input_Value(string inputName);

  virtual string Check_Button();
};


#endif /* PAGE_H_ */
