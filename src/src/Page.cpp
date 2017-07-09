/*
 * File:   Page.cpp
 * Author: Mahdi Razavi...................(Razavi.Dev@gmail.com)
 *
 * Created on: 	Apr 22, 2015
 * Edit Date :	
 * 				Apr 22, 2015	
 */

#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <string.h>

#include "Page.h"

Page::Page(string pageName, map<string, string> &CgiVariables, bool isPostBack) {

  this->pageFileName = pageName;
  this->CgiVariables = CgiVariables;
  this->isPostBack = isPostBack;
}

Page::Page(CGI_Interface* cgiInstance) {
  this->cgiInstance = cgiInstance;
  cgiInstance->FetchAllVariable(this->CgiVariables);
  this->pageFileName = this->CgiVariables["page"];
  this->isPostBack = cgiInstance->IsPostBack();
}

Page::~Page() {

}

//Page::Page( Page & org){
//  this->pageFileName = org.pageFileName;
//}

string Page::loadPageFile(string strPageFileName) {

  //  string strPageFileName;
  int validateResult = 0;
  //  if (CgiVariables.find("page") != CgiVariables.end()) {
  //    strPageFileName = strPageFileName;
  //  } else {
  //    strPageFileName = "INVALID";
  //  }
  //  if (strPageFileName != "INVALID"
  //      && (validateResult = validateInput(witAlphaNumericDashDot, strPageFileName))
  //          == 0) {
  //  } else
  if (strPageFileName == "INVALID") {
    //strPageFileName = "INVALID";
    string debugMessage = "Error: invalid page name";
    debugMessage.append(strPageFileName);
    writeDebugLog(debugMessage, validateResult, __func__, __LINE__, false);
  }

  // load html file if ?page parameter has value
  // if ?page is Null or INVALID , "lan.htm" is load
  string output;
  //if (page != 0)
  if (strPageFileName != "INVALID") {
    string Page = "htmls/";
    Page.append(strPageFileName);
    Page.append(".htm");
    const char* pg = Page.c_str();
    output = read_txt_file(pg);
  } else {
    //TODO: if html file dose not exist, show 404 error page.
    output = read_txt_file("htmls/lan.htm");
  }
  return output;
}

/**
 * Detect input tag pattern in HTML content and Pass name of that to "get_inputvalue" function to get value or perform a operation based on.
 * default tag pattern is => #$=[NameOfInputTag]#$;
 * another defined pattern is => #$$[NameOfInputTag-value]$$;
 * @param html
 * @return
 */
string Page::generate_input(string html) {
  string sdelimiter = "#$=";
  string edelimiter = "#$;";
  string inpttag = "<input name=\"$NAME$\" type=\"text\" value=\"$VALUE$\" />";
  string passwordPattern = "passwd";
  uint passwordPatternSize = passwordPattern.size();

  string inptPatt;
  string inptName;
  string inpttagTemp;
  string inptvalue;
  unsigned sdelsize = sdelimiter.size();
  unsigned edelsize = edelimiter.size();
  unsigned pos = 0;
  unsigned endpos = 0;
  unsigned ENDpos = 0;
  unsigned inptNameIndx = 0;
  while ((pos = html.find(sdelimiter, pos)) != string::npos) {
    endpos = html.find(edelimiter, endpos);
    ENDpos = endpos + edelsize;
    inptPatt = html.substr(pos, ENDpos - pos);
    inptNameIndx = pos + sdelsize;
    inptName = html.substr(inptNameIndx, endpos - inptNameIndx);

    if (inptName.size() >= passwordPatternSize
        && inptName.substr(inptName.size() - passwordPatternSize,
                           inptName.size()) == passwordPattern) {
      inpttag = "<input name=\"$NAME$\" type=\"password\" value=\"$VALUE$\" />";
    }
    inpttagTemp = search_replace(inpttag, "$NAME$", inptName);
    inptvalue = Get_Input_Value(inptName);
    inpttagTemp = search_replace(inpttagTemp, "$VALUE$", inptvalue);
    html = search_replace(html, inptPatt, inpttagTemp);
    pos += inpttagTemp.size();
  }

  // generate input value
  sdelimiter = "#$$";
  edelimiter = "-value$$;";
  //"#$$pgmodeChnNo1-value$$;"
  //<input id="pgmodeChnNo1" name="pgmodeChnNo1" value="#$$pgmodeChnNo1-value$$;" type="text" />

  //inptPatt;
  //inptName;
  //inpttagTemp;
  //inptvalue;
  sdelsize = sdelimiter.size();
  edelsize = edelimiter.size();
  pos = 0;
  endpos = 0;
  ENDpos = 0;
  inptNameIndx = 0;
  while ((pos = html.find(sdelimiter, pos)) != string::npos) {
    endpos = html.find(edelimiter, endpos);
    ENDpos = endpos + edelsize;
    inptPatt = html.substr(pos, ENDpos - pos);
    inptNameIndx = pos + sdelsize;
    inptName = html.substr(inptNameIndx, endpos - inptNameIndx);

    inptvalue = Get_Input_Value(inptName);

    html = search_replace(html, inptPatt, inptvalue);
    //pos += inpttagTemp.size();
    pos += inptvalue.size();
  }
  return html;
}

/**
 * Detect Select tag in the HTML content and pass the name, value of that to "get_selectvalue" function to perform a action based on.
 * default tag pattern is => ~$=[NameOfSelect],[NameOfOption]~$;
 * another defined pattern is => ~$$[NameOfSelect],[options]$$;
 * @param html
 * @return
 */
string Page::generate_select(string html) {
  string selectPattern;
  string selectName;
  string selectResult = " ";

  string selectNameName;
  string selectNameValue;
  string word;

  string startDelimiter;
  string endDelimiter;

  startDelimiter = "~$$";
  endDelimiter = "-options$$;";

  unsigned startDelimiterSize = startDelimiter.size();
  unsigned endDelimiterSize = endDelimiter.size();
  unsigned pos = 0;
  unsigned endpos = 0;
  unsigned ENDpos = 0;
  unsigned selectNameIndex = 0;

  //"~$$chnPeripheral1-options$$;"
  //<select size=1 name="chnPeripheral1" id ="chnPeripheral1" onChange="selectChanged(this);">~$$chnPeripheral1-options$$;</select>

  string selectOptions;

  while ((pos = html.find(startDelimiter, pos)) != string::npos) {
    endpos = html.find(endDelimiter, endpos);
    ENDpos = endpos + endDelimiterSize;
    selectPattern = html.substr(pos, ENDpos - pos);

    selectNameIndex = pos + startDelimiterSize;
    selectNameName = html.substr(selectNameIndex, endpos - selectNameIndex);

    selectOptions = Get_Select_Options(selectNameName);
    html = search_replace(html, selectPattern, selectOptions);
    pos += selectOptions.size();
    //cout << " options=" << selectOptions <<"selectPattern=" << selectPattern <<  "selectOption.size()=" << selectOptions.size() << " pos=" << pos << "endpos" << endpos << " </br>";
  }

  // <option value="radio" ~$=chnPeripheral1,radio~$; > Radio </option>

  startDelimiter = "~$=";
  endDelimiter = "~$;";

  startDelimiterSize = startDelimiter.size();
  endDelimiterSize = endDelimiter.size();
  pos = 0;
  endpos = 0;
  ENDpos = 0;
  while ((pos = html.find(startDelimiter, pos)) != string::npos) {
    endpos = html.find(endDelimiter, endpos);
    ENDpos = endpos + endDelimiterSize;
    selectPattern = html.substr(pos, ENDpos - pos);
    selectNameIndex = pos + startDelimiterSize;
    selectName = html.substr(selectNameIndex, endpos - selectNameIndex);
    istringstream slctNameStream(selectName);
    int counter = 0;
    while (getline(slctNameStream, word, ',')) {
      if (counter == 0) {
        selectNameName = word.c_str();
      } else {
        selectNameValue = word.c_str();
      }
      counter++;
    }
    selectResult = Get_Select_Value(selectNameName, selectNameValue);
    html = search_replace(html, selectPattern, selectResult);
    pos += selectResult.size();
  }
  return html;
}

/**
 * Detect defined HTML tag and pass the name of that to "get_htmlvalue" function to generate a needed HTML structure based on.
 * default tag patern is => &$=[NameOfGeneratorTag]&$;
 * @param html
 * @return
 */
string Page::generate_html(string html) {
  string sdelimiter = "&$=";
  string edelimiter = "&$;";

  string hmlPatt;
  string hmlName;
  string hmlResult;
  unsigned sdelsize = sdelimiter.size();
  unsigned edelsize = edelimiter.size();
  unsigned pos = 0;
  unsigned endpos = 0;
  unsigned ENDpos = 0;
  unsigned hmlNameIndx = 0;
  while ((pos = html.find(sdelimiter, pos)) != string::npos) {
    endpos = html.find(edelimiter, endpos);
    ENDpos = endpos + edelsize;
    hmlPatt = html.substr(pos, ENDpos - pos);
    hmlNameIndx = pos + sdelsize;
    hmlName = html.substr(hmlNameIndx, endpos - hmlNameIndx);
    hmlResult = Get_Html_Value(hmlName);
    html = search_replace(html, hmlPatt, hmlResult);
    pos += hmlResult.size();
  }
  return html;
}

/**
 * Get input tag name then return tag value or perform a operation based on.
 * this function manage input value
 * !name.compare("[name of select tag]") ==> this code detect now you manage this input
 * FetchItems[i].compare("NULL") ==> this code detect FetchItems[i] has value or not
 * @param name
 * @return
 */
string Page::Get_Input_Value(string inputName) {
  return "";
}

/**
 * Get select options and generate option tag for select.
 * @param name
 * @return
 */
string Page::Get_Select_Options(string name) {
  return " ";
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
string Page::Get_Select_Value(string name, string value) {
  return " ";
}

/**
 * Get defined HTML tag name and generate a needed HTML structure based on.
 * this function manage html value.
 * !name.compare("[name of html generator tag]") ==> this code detect now you manage this html value
 * @param name
 * @return
 */
string Page::Get_Html_Value(string name) {
  return "";
}

/**
 * show HTTP header for send to browser
 * @return content
 */
void Page::Show_HttpHeader() {
  string strPageFileName = this->CgiVariables["page"];
  if (strPageFileName != "out_calllog" && strPageFileName != "out_cntlist") {
      printf("Content-Type: text/html\n\n");
    }

}

string Page::Check_Button()
{
  return "";
}

/**
 * Show custom html content which is not common implemented tag
 * Use this function when there is no html file to show
 * @return content
 */
void Page::Show_CustomHtml() {
}

void Page::Show() {

  Show_HttpHeader();

  string pageContent = this->loadPageFile(pageFileName);
  pageContent = generate_input(pageContent);
  pageContent = generate_select(pageContent);
  pageContent = generate_html(pageContent);

  string result = Check_Button();

  cout << pageContent<<result;

  Show_CustomHtml();
}

