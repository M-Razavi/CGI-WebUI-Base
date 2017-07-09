/*--------------------------------------------------------
 CGI Variable Wrapper    http://www.purplepixie.org/cgi
 * Read the Licence *    dcutting [at] purplepixie.org

 cgi_interface.h      
 2.07	30/03/2007	
 2.08 	22/04/2015	Add multipart/form-data post handle M-Razavi Razavi.dev@gmail.com 
 --------------------------------------------------------*/

#ifndef CGI_INTERFACE
#define CGI_INTERFACE

#define LENMAX_VAR_NAME 254
#define LENMAX_VAR_VALUE 1024

#define LENMAX_VAR_NULLSTRING 2

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <map>

//#include "cookie.h"

class CGI_Variable
{
public:
	CGI_Variable(const char *iName="", const char *iValue="");
	char Name[LENMAX_VAR_NAME];
	char Value[LENMAX_VAR_VALUE];
	CGI_Variable *Next;
	char Encoded[ (LENMAX_VAR_NAME + LENMAX_VAR_VALUE) *2 ];
	char *Encode(void);
};

class CGI_File
{
public:
	CGI_File(const char *iName="", const char *iTempName="", const char *iContentType="");
	char Name[LENMAX_VAR_NAME];
	char TempName[LENMAX_VAR_VALUE];
	char ContentType[LENMAX_VAR_NAME];
	CGI_File *Next;
};

class CGI_Interface
{
public:
	bool PrintError;
	bool ReturnValidPointer;
	char NullString[LENMAX_VAR_NULLSTRING];
	CGI_Interface(bool pError=false);
	int AutoLoad(void);
	int CookieLoad(void);
	void PrintDump(void);
	int StringLoad(const char *input);
	char* FetchVariable(const char *name, bool UseReturnValidPointer);
	int FetchAllVariable(std::map<std::string, std::string> &);
	char* FetchFileInfo(const char *name, char * contentType, bool UseReturnValidPointer);
	int SetVariable(const char *name, const char *value);
	//int SetFile(const char *name, const char *TempName, const char *ContentType);
	int HiddenDump(void);
	int GetDump(void);
	int GetVariablesCount(void);
	void ShowEnviromentVariables( bool);
	bool IsPostBack();
	~CGI_Interface();
private:
	bool AddVariable(const char *name, const char *value);
	bool AddFile(const char *name, const char *TempName, const char *ContentType);
	CGI_Variable *FirstVariable;
	CGI_File *FirstFile;
};

#endif

