/*--------------------------------------------------------
 CGI Variable Wrapper    http://www.purplepixie.org/cgi
 * Read the Licence *    dcutting [at] purplepixie.org

 cgi_interface.cpp       
 2.07	30/03/2007	
 2.08 	22/04/2015	Add multipart/form-data post handle M-Razavi Razavi.dev@gmail.com 
 --------------------------------------------------------*/

#ifndef CGI_MAIN
#define CGI_MAIN

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <fstream>
#include <iostream>
#include <sstream>

#include <vector>

#include <RzvUtility.h>

#include <stdint.h>

#include <utility>
#include "cgi_interface.h"

CGI_Variable::CGI_Variable( const char *iName, const char *iValue)
{
	strcpy( Name, iName);
	strcpy( Value, iValue);
	Next = 0;
}

char *CGI_Variable::Encode( void)
{
	int EncPtr = 0;
	int InpPtr = 0;
	int InpMax = strlen( Value);
	for( InpPtr = 0; InpPtr < InpMax; InpPtr++)
	{
		switch( Value[InpPtr])
		{
			case ' ':
				Encoded[EncPtr++] = '+';
				break;
			case '\"':
				Encoded[EncPtr++] = '%';
				Encoded[EncPtr++] = '2';
				Encoded[EncPtr++] = '2';
				break;
			case '%':
				Encoded[EncPtr++] = '%';
				Encoded[EncPtr++] = '2';
				Encoded[EncPtr++] = '5';
				break;
			case '&':
				Encoded[EncPtr++] = '%';
				Encoded[EncPtr++] = '2';
				Encoded[EncPtr++] = '6';
				break;
			case '?':
				Encoded[EncPtr++] = '%';
				Encoded[EncPtr++] = '3';
				Encoded[EncPtr++] = 'f';
				break;
			case '+':
				Encoded[EncPtr++] = '%';
				Encoded[EncPtr++] = '2';
				Encoded[EncPtr++] = 'b';
				break;
			default:
				Encoded[EncPtr++] = Value[InpPtr];
				break;
		}
	}

	return Encoded;
}

CGI_File::CGI_File(const char *iName, const char *iTempName, const char *iContentType)
{
	strcpy( Name, iName);
	strcpy( TempName, iTempName);
	strcpy( ContentType, iContentType);
	Next = 0;
}
CGI_Interface::CGI_Interface( bool pError)
{
	PrintError = pError;
	FirstVariable = 0;
	FirstFile = 0;
	ReturnValidPointer = false;
	for( int a = 0; a < LENMAX_VAR_NULLSTRING; a++)
	{
		NullString[a] = 0;
	}
}

/**
 * check REQUEST_METHOD and add all POST,GET variable to link list.
 * @return
 */
int CGI_Interface::AutoLoad( void)
{
	char *method = getenv( "REQUEST_METHOD");
	if( ( method == 0)
			|| ( strlen( method) < 3)
			|| ( strcmp( method, "(null)") == 0))
	{
		if( PrintError)
			printf( "CGI Error: No method specified: %s\n<br>\n", method);
		return -1;
	}
	if( strcmp( "GET", method) == 0)
	{
		return StringLoad( getenv( "QUERY_STRING"));
	}
	else if( strcmp( "POST", method) == 0)
	{
		//		std::string postDataFilePath = "/www/log/Post_Data.log";
		//		std::ofstream log_file( postDataFilePath.c_str(), std::ios_base::out | std::ios_base::app);

		std::string contentType = getenv( "CONTENT_TYPE");
		if( contentType.substr( 0, 20) == "multipart/form-data;") // In case of    multipart/form-data
		{
			int fieldStartIndex = 0;
			int fieldEndIndex = 0;

			std::string fieldName = "";
			std::string fieldValue = "";
			std::string fileName = "";
			std::string fileNameTemp = "";
			std::string fileContentType = "";
			//std::string fileData = "";

			std::string fieldNamePattern = "name=\"";
			std::string fileNamePattern = "filename=\"";
			std::string fileContentTypePattern = "Content-Type: ";
			std::string fileDataPattern = "\r\n\r\n";


			std::string boundary = contentType.substr( 30);
			boundary = "--" + boundary; //for compatibility

			//int count = 0;
			long lSize = 1024 * 1024; //1MB
			char buffer[lSize];
			//char byteBlock;

			//int readResult = 0;
			//int errResult = 0;
			int32_t readReasult;


			// copy the file into the buffer:
			readReasult = fread (buffer,1,lSize,stdin);
			if (readReasult <= 0)
			{
				return readReasult;
			}

			uint32_t delimiterStartPos[254];
			uint16_t boundaryPatternIndex = 0;
			uint16_t delimiterCounter=0;
			bool inDelimiterRange= false;
			for(int32_t i=0; i<readReasult; i++)
			{

				if(buffer[i]== boundary[boundaryPatternIndex])
				{
					inDelimiterRange = true;

					if(boundaryPatternIndex == boundary.size()-1)
					{
						delimiterStartPos[delimiterCounter++] = i;

						inDelimiterRange = false;
						boundaryPatternIndex = 0;
					}
					else if(boundaryPatternIndex == boundary.size())
					{
						inDelimiterRange = false;
						boundaryPatternIndex = 0;
					}
					else
					{
						boundaryPatternIndex++;
					}
				}
				else
				{
					inDelimiterRange = false;
					boundaryPatternIndex = 0;
				}
			}

			std::string postData[delimiterCounter];
			std::string buffVariableNameValue;

			for(int i=0; i<delimiterCounter-1; i++)
			{
				uint32_t tempDataSize = delimiterStartPos[i+1] - delimiterStartPos[i] - boundary.size();
				char* tempData;
				tempData = (char*) malloc(tempDataSize * sizeof(char));
				memcpy ( tempData, buffer + delimiterStartPos[i] + 1, tempDataSize );
				postData[i] = std::string(tempData);

				fieldStartIndex = postData[i].find(fieldNamePattern);
				if(fieldStartIndex > 0)
				{
					fieldEndIndex = postData[i].find("\"\r\n\r\n",fieldStartIndex);
					if(fieldEndIndex > 0)
					{
						fieldName = postData[i].substr(fieldStartIndex + fieldNamePattern.size(), fieldEndIndex - fieldStartIndex - fieldNamePattern.size());

						fieldStartIndex = fieldEndIndex + std::string("\"\r\n\r\n").size();
						fieldEndIndex = postData[i].find("\r\n", fieldStartIndex);
						if(fieldEndIndex > 0)
						{
							fieldValue = postData[i].substr(fieldStartIndex, postData[i].size() - fieldStartIndex - 2);


							buffVariableNameValue =  fieldName + "=";
							buffVariableNameValue += fieldValue;
							StringLoad( buffVariableNameValue.c_str());
						}
					}
					else  //input file
					{
						fieldEndIndex = postData[i].find("\"; ",fieldStartIndex);
						if(fieldEndIndex > 0)
						{
							fieldName = postData[i].substr(fieldStartIndex + fieldNamePattern.size(), fieldEndIndex - fieldStartIndex - fieldNamePattern.size());

							fieldStartIndex = postData[i].find(fileNamePattern);
							if(fieldStartIndex > 0)
							{
								fieldEndIndex = postData[i].find("\"\r\n",fieldStartIndex);
								if(fieldEndIndex > 0)
								{
									fileName = postData[i].substr(fieldStartIndex + fileNamePattern.size(), fieldEndIndex - fieldStartIndex - fileNamePattern.size());
								}
							}

							fieldStartIndex = postData[i].find(fileContentTypePattern);
							if(fieldStartIndex > 0)
							{
								fieldEndIndex = postData[i].find("\r\n\r\n",fieldStartIndex);
								if(fieldEndIndex > 0)
								{
									fileContentType = postData[i].substr(fieldStartIndex + fileContentTypePattern.size(), fieldEndIndex - fieldStartIndex - fileContentTypePattern.size());
								}
							}

							fieldStartIndex = fieldEndIndex;
							if(fieldStartIndex > 0)
							{
								fieldEndIndex = postData[i].size() - 4;
								if(fieldEndIndex > 0)
								{
									fileNameTemp = boundary.substr(29);
									fileNameTemp = "/tmp/" + fileNameTemp;
									std::ofstream tmpFileData( fileNameTemp.c_str(), std::ios_base::out | std::ios_base::binary);
									tmpFileData.write(tempData + fieldStartIndex + 4 , tempDataSize - fieldStartIndex - 5); //TODO: maybe must 6
									tmpFileData.close();

									AddFile(fileName.c_str(), fileNameTemp.c_str(), fileContentType.c_str());
									buffVariableNameValue =  fieldName + "=";
									buffVariableNameValue += fileName;
									StringLoad( buffVariableNameValue.c_str());
								}

							}
						}
					}
				}

				free(tempData);
			}

			return readReasult;
		}
		else // In case of     application/x-www-form-urlencoded    OR    text/plain
		{
			int count = 0;
			char buff[1024 * 32];
			fgets( buff, 1024 * 32, stdin);
			while( !feof( stdin))
			{
				count += StringLoad( buff);
				fgets( buff, 1024 * 32, stdin);
			}
			if( strlen( buff) > 0)
				count += StringLoad( buff);
			return count;

		}
	}
	else
	{
		if( PrintError)
			printf( "CGI Error: Unsupported Method %s\n <br>\n", method);
		return -1;
	}

}

int CGI_Interface::StringLoad( const char *input)
{
	if( input == 0)
		return -1;
	if( strlen( input) <= 0)
		return -1;
	int count = 0;
	char cname[LENMAX_VAR_NAME];
	char cvalue[LENMAX_VAR_VALUE];
	int pos = 0;
	bool name = true;
	long val = 0;
	char d[5];
	for( uint a = 0; a < strlen( input); a++)
	{
		if( name)
		{
			cname[pos] = 0;
			cname[pos + 1] = 0;
		}
		else
		{
			cvalue[pos] = 0;
			cvalue[pos + 1] = 0;
		}
		switch( input[a])
		{
			case '=':
				if( name)
				{
					cname[pos] = 0;
					pos = 0;
					name = false;
				}
				break;
			case '&':
				if( !name)
					cvalue[pos] = 0;
				else
				{
					cvalue[0] = 0;
					cname[pos] = 0;
				}
				SetVariable( cname, cvalue);
				cname[0] = 0;
				cvalue[0] = 0;
				count++;
				name = true;
				pos = 0;
				break;
			case '\%':
				strncpy( d, input + a + 1, 2);
				val = strtol( d, (char**)NULL, 16);
				if( name)
					cname[pos] = val;
				else
					cvalue[pos] = val;
				pos++;
				a = a + 2;
				break;
			case '+':
				if( name)
					cname[pos] = ' ';
				else
					cvalue[pos] = ' ';
				pos++;
				break;
			default:
				if( name)
					cname[pos] = input[a];
				else
					cvalue[pos] = input[a];
				pos++;
				break;
		}
	}
	if( strlen( cname) > 0)
	{
		SetVariable( cname, cvalue);
		count++;
	}
	return count;
}

int CGI_Interface::CookieLoad( void)
{// New in 2005 versions (2.06+) - might still not work properly :o(

	char ts[256];
	for( int a = 0; a < 256; a++)
		ts[a] = 0;

	char *cookies = getenv( "HTTP_COOKIE");

	int loaded = 0;

	int tptr = 0;
	for( uint cpos = 0; cpos < strlen( cookies); cpos++)
	{
		char n = cookies[cpos];
		if( ( n != ';') && ( n != ' '))
			ts[tptr++] = n;
		else if( n != ' ')
		{
			loaded += StringLoad( ts);
			tptr = 0;
			for( int z = 0; z < 256; z++)
				ts[z] = 0;
		}
	}

	if( strlen( ts) > 0)
		StringLoad( ts);

	return loaded;
}

bool CGI_Interface::AddVariable( const char *name, const char *value)
{
	CGI_Variable *nv = new CGI_Variable( name, value);
	if( FirstVariable == 0)
	{
		FirstVariable = nv;
		return true;
	}
	CGI_Variable *c = FirstVariable;
	while( c->Next != 0)
		c = c->Next;
	c->Next = nv;
	return true;
}

bool CGI_Interface::AddFile(const char *name, const char *TempName, const char *ContentType)
{
	CGI_File *nv = new CGI_File( name, TempName, ContentType);
	if( FirstFile == 0)
	{
		FirstFile = nv;
		return true;
	}
	CGI_File *c = FirstFile;
	while( c->Next != 0)
		c = c->Next;
	c->Next = nv;
	return true;
}

CGI_Interface::~CGI_Interface()
{
	if( FirstVariable == 0)
		return;
	CGI_Variable *c = FirstVariable;
	while( c->Next != 0)
	{
		CGI_Variable *d = c;
		c = c->Next;
		delete d;
	}
	delete c;

	if( FirstFile == 0)
		return;
	CGI_File *f = FirstFile;
	while( f->Next != 0)
	{
		CGI_File *n = f;
		f = f->Next;
		delete n;
	}
	delete f;
}

void CGI_Interface::PrintDump( void)
{
	printf( "CGI Interface Variable Dump: <p>\n\n");
	CGI_Variable *c = FirstVariable;
	int count = 0;
	while( c != 0)
	{
		printf( "%d: %s: %s <br>\n", count + 1, c->Name, c->Value);
		c = c->Next;
		count++;
	}
	printf( "\n<p>\nTotal of %d Variables Found\n<p>\n", count);
}

char* CGI_Interface::FetchVariable( const char *name, bool UseReturnValidPointer = true)
{
	CGI_Variable *c;
	c = FirstVariable;
	while( c != 0)
	{
		if( strcmp( c->Name, name) == 0)
		{
			return c->Value;
		}
		c = c->Next;
	}
	if( !UseReturnValidPointer)
		return 0;
	if( ReturnValidPointer)
		return NullString;
	return 0;
}

int CGI_Interface::FetchAllVariable(std::map<std::string, std::string> &cgiVariables)
{
	if(FirstVariable == 0)
	{
		return -1;
	}

	CGI_Variable *currentVariable = FirstVariable;
	int count = 0;
	while( currentVariable != 0)
	{
		cgiVariables[std::string(currentVariable->Name)] = std::string(currentVariable->Value);
		currentVariable = currentVariable->Next;
		count++;
	}
	return count;
}

/**
 * get uploaded file information
 * @param name
 * @param UseReturnValidPointer
 * @return FilePath
 */
char* CGI_Interface::FetchFileInfo( const char *name,char *contentType,  bool UseReturnValidPointer = true) //TODO: add ContentType to function
{
	CGI_File *c;
	c = FirstFile;
	while( c != 0)
	{
		if( strcmp( c->Name, name) == 0)
		{
			contentType = (char*)malloc(strlen(c->ContentType) * sizeof(char));
			memcpy(contentType, c->ContentType,strlen(c->ContentType));
			//contentType = c->ContentType;
			return c->TempName;
		}
		c = c->Next;
	}
	if( !UseReturnValidPointer)
		return 0;
	if( ReturnValidPointer)
		return NullString;
	return 0;
}

/**
 * Set varibale name and value in linklist. if found it, set new value. if not found, add to linklist.
 * @param name
 * @param value
 * @return
 */
int CGI_Interface::SetVariable( const char *name, const char *value)
{
	char *c = FetchVariable( name, false);
	if( c == 0)
	{
		AddVariable( name, value);
	}
	else
	{
		strcpy( c, value);
	}
	return 1;
}

int CGI_Interface::HiddenDump( void)
{
	CGI_Variable *c = FirstVariable;
	int count = 0;
	while( c != 0)
	{
		printf( "<input type=hidden name=\"%s\" value=\"", c->Name);
		for( uint a = 0; a < strlen( c->Value); a++)
		{
			if( c->Value[a] == '\"')
				printf( "%s", "%22");
			else
				printf( "%c", c->Value[a]);
		}
		printf( "\">\n");
		count++;
		c = c->Next;
	}
	return count;
}

int CGI_Interface::GetDump( void)
{
	CGI_Variable *c = FirstVariable;
	int count = 0;
	while( c != 0)
	{
		if( count > 0)
			printf( "&");
		printf( "%s=%s", c->Name, c->Encode());
		count++;
		c = c->Next;
	}
	return count;
}

int CGI_Interface::GetVariablesCount(void)
{
	CGI_Variable *currentVariable = FirstVariable;
	int count = 0;
	while( currentVariable != 0)
	{
		count++;
		currentVariable = currentVariable->Next;
	}
	return count;
}

/**
 * Show HTTP environment variables
 * @param writeToFile if true, write Enviroment variable to file /www/log/EnviromentVars.log
 */
void CGI_Interface::ShowEnviromentVariables( bool writeToFile = false)
{
	std::ostringstream strEnviromentVariables;
	std::string delimter = "</br>";

	if( writeToFile)
	{
		delimter = "\n";
	}

	//This environment variable contains the name and version of the software that your program is running under.
	strEnviromentVariables
	<< " SERVER_SOFTWARE="
	<< ( ( getenv( "SERVER_SOFTWARE")) ? getenv( "SERVER_SOFTWARE") : "")
	<< delimter;

	//This environment variable contains the domain name or IP address of the server machine.
	strEnviromentVariables
	<< " SERVER_NAME="
	<< ( ( getenv( "SERVER_NAME")) ? getenv( "SERVER_NAME") : "")
	<< delimter;

	//This environment variable contains the name and revision of the protocol being used by the client and server.
	strEnviromentVariables
	<< " SERVER_PROTOCOL="
	<< ( ( getenv( "SERVER_PROTOCOL")) ? getenv( "SERVER_PROTOCOL") : "")
	<< delimter;

	//This environment variable contains the number of the port to which this request was sent.
	strEnviromentVariables
	<< " SERVER_PORT="
	<< ( ( getenv( "SERVER_PORT")) ? getenv( "SERVER_PORT") : "")
	<< delimter;

	//This environment variable contains the directory under which the current program is executing, as defined in the server's configuration file.
	strEnviromentVariables
	<< " DOCUMENT_ROOT="
	<< ( ( getenv( "DOCUMENT_ROOT")) ? getenv( "DOCUMENT_ROOT") : "")
	<< delimter;

	//The value given to the ServerAdmin (for Apache) directive in the web server configuration file. If the script is running on a virtual host, this will be the value defined for that virtual host.
	strEnviromentVariables
	<< " SERVER_ADMIN="
	<< ( ( getenv( "SERVER_ADMIN")) ? getenv( "SERVER_ADMIN") : "")
	<< delimter;

	//This environment variable contains the revision of the CGI specification supported by the server software.
	strEnviromentVariables
	<< " GATEWAY_INTERFACE="
	<< ( ( getenv( "GATEWAY_INTERFACE")) ? getenv(
			"GATEWAY_INTERFACE") : "")
			<< delimter;

	//This environment variable contains the name of the method (defined in the HTTP protocol) to be used when accessing URLs on the server. When a hyperlink is clicked, the GET method is used. When a form is submitted, the method used is determined by the METHOD attribute to the FORM tag. CGI programs do not have to deal with the HEAD method directly and can treat it just like the GET method.
	strEnviromentVariables
	<< " REQUEST_METHOD="
	<< ( ( getenv( "REQUEST_METHOD")) ? getenv( "REQUEST_METHOD") : "")
	<< delimter;

	//The absolute pathname of the currently executing script.
	strEnviromentVariables
	<< " SCRIPT_FILENAME="
	<< ( ( getenv( "SCRIPT_FILENAME")) ? getenv( "SCRIPT_FILENAME") : "")
	<< delimter;

	//This environment variable contains the name of the virtual path to your program. If your program needs to refer the remote client back to itself, or needs to construct anchors in HTML referring to itself, you can use this variable.
	strEnviromentVariables
	<< " SCRIPT_NAME="
	<< ( ( getenv( "SCRIPT_NAME")) ? getenv( "SCRIPT_NAME") : "")
	<< delimter;

	//If a form is submitted with the POST method, then this environment variable contains the type of data being sent by the client. While clients normally only send "application/x-www-form-urlencoded," this variable can contain any MIME type. To transfer binary data to the your CGI program you must use "multipart/form-data".
	strEnviromentVariables
	<< " CONTENT_TYPE="
	<< ( ( getenv( "CONTENT_TYPE")) ? getenv( "CONTENT_TYPE") : "")
	<< delimter;

	//This environment variable contains the number of bytes being sent by the client. You use this variable to determine the number of bytes you need to read.
	strEnviromentVariables
	<< " CONTENT_LENGTH="
	<< ( ( getenv( "CONTENT_LENGTH")) ? getenv( "CONTENT_LENGTH") : "")
	<< delimter;

	//This environment variable contains information from an HTML page to your program in these three instances:1-When a page contains links with encoded queries 2-When a form was accessed with the GET method 3-When a page contains an ISINDEX tag and the user executes a search
	strEnviromentVariables
	<< " QUERY_STRING="
	<< ( ( getenv( "QUERY_STRING")) ? getenv( "QUERY_STRING") : "")
	<< delimter;

	//The Uniform Resource Identifier (URI) which was given in order to access the program. The URI points the server to the file that contains the CGI program you want to run (or the static document or image to be served).
	strEnviromentVariables
	<< " REQUEST_URI="
	<< ( ( getenv( "REQUEST_URI")) ? getenv( "REQUEST_URI") : "")
	<< delimter;

	//This environment variable contains the extra path information that the server derives from the URL that was used to access the CGI program.
	strEnviromentVariables
	<< " PATH_INFO="
	<< ( ( getenv( "PATH_INFO")) ? getenv( "PATH_INFO") : "")
	<< delimter;

	//This environment variable contains the actual fully-qualified file name that was translated from the URL. web servers distinguish between path names used in URLs, and file system path names. It is often useful to make your PATH_INFO a virtual path so that the server provides a physical path name in this variable. This way, you can avoid giving file system path names to remote client software.
	strEnviromentVariables
	<< " PATH_TRANSLATED="
	<< ( ( getenv( "PATH_TRANSLATED")) ? getenv( "PATH_TRANSLATED") : "")
	<< delimter;

	//If the CGI script is protected by any type of authorization, this environment variable contains the authorization type. Apache web servers support HTTP basic and digest access authorization.
	strEnviromentVariables
	<< " AUTH_TYPE="
	<< ( ( getenv( "AUTH_TYPE")) ? getenv( "AUTH_TYPE") : "")
	<< delimter;

	//This environment variable contains the host name of the remote client software. This is a fully-qualified domain name such as www.dbase.com (instead of just www, which you might type within your intranet). If no host name information is available, your program should use the REMOTE_ADDR variable instead.
	strEnviromentVariables
	<< " REMOTE_HOST="
	<< ( ( getenv( "REMOTE_HOST")) ? getenv( "REMOTE_HOST") : "")
	<< delimter;

	//This environment variable contains the IP address of the remote host. This information is guaranteed to be present.
	strEnviromentVariables
	<< " REMOTE_ADDR="
	<< ( ( getenv( "REMOTE_ADDR")) ? getenv( "REMOTE_ADDR") : "")
	<< delimter;

	//This environment variable is set to the name of the local HTTP user of the person using the browser software only if access authorization has been activated for this URL. Note that this is not a way to determine the user name of any person accessing your program.
	strEnviromentVariables
	<< " REMOTE_USER="
	<< ( ( getenv( "REMOTE_USER")) ? getenv( "REMOTE_USER") : "")
	<< delimter;

	//This environment variable identifies the browser software being used to access your program.Contents of the User_Agent: header from the current request, if there is one. This is a string denoting the browser software being used to view the current page.
	strEnviromentVariables
	<< " HTTP_USER_AGENT="
	<< ( ( getenv( "HTTP_USER_AGENT")) ? getenv( "HTTP_USER_AGENT") : "")
	<< delimter;

	//Contents of the Host: header from the current request, if there is one.
	strEnviromentVariables
	<< " HTTP_HOST="
	<< ( ( getenv( "HTTP_HOST")) ? getenv( "HTTP_HOST") : "")
	<< delimter;

	//This environment variable enumerates the types of data the client can accept. For most client software, this protocol feature has become a bit convoluted and the information isn't always useful.
	strEnviromentVariables
	<< " HTTP_ACCEPT="
	<< ( ( getenv( "HTTP_ACCEPT")) ? getenv( "HTTP_ACCEPT") : "")
	<< delimter;

	//Contents of the Accept-Charset: header from the current request, if there is one.
	strEnviromentVariables
	<< " HTTP_ACCEPT_CHARSET="
	<< ( ( getenv( "HTTP_ACCEPT_CHARSET")) ? getenv(
			"HTTP_ACCEPT_CHARSET") : "")
			<< delimter;

	//Contents of the Accept-Language: header from the current request, if there is one. This value can be changed on the client browser options, when choosing preferred language.
	strEnviromentVariables
	<< " HTTP_ACCEPT_LANGUAGE="
	<< ( ( getenv( "HTTP_ACCEPT_LANGUAGE")) ? getenv(
			"HTTP_ACCEPT_LANGUAGE") : "")
			<< delimter;

	//Contents of the Connection: header from the current request, if there is one.
	strEnviromentVariables
	<< " HTTP_CONNECTION="
	<< ( ( getenv( "HTTP_CONNECTION")) ? getenv( "HTTP_CONNECTION") : "")
	<< delimter;

	//The address of the page (if any) which referred the browser to the current page. This is set by the user's browser; not all browsers will set this.
	strEnviromentVariables
	<< " HTTP_REFERER="
	<< ( ( getenv( "HTTP_REFERER")) ? getenv( "HTTP_REFERER") : "")
	<< delimter;

	//	while( ( pos = strEnviromentVariables.str().find( "", pos)) != string::npos)
	//	{

	if( writeToFile)
	{
		std::string enviromentVarsFilePath = "/www/log/enviromentVars.log";

		std::ofstream log_file( enviromentVarsFilePath.c_str(),
		                        std::ios_base::out | std::ios_base::app);
		log_file
		<< __DATE__
		<< delimter
		<< __TIME__
		<< delimter
		<< strEnviromentVariables.str()
		<< delimter
		<< delimter;
		log_file.close();
	}
	else
	{
		std::cout << strEnviromentVariables.str();
	}
}


bool CGI_Interface::IsPostBack()
{
	char *method = getenv( "REQUEST_METHOD");
	if( ( method == 0)  || ( strlen( method) < 3)  || ( strcmp( method, "(null)") == 0))
	{
		if( PrintError)
			printf( "CGI Error: No method specified: %s\n<br>\n", method);
		return false;
	}

	if( strcmp( "POST", method) == 0)
	{
		return true;
	}
	return false;
}

#endif
