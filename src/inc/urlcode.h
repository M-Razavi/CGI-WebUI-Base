/*
 * File:   urlcode.h
 * Author: Mahdi Razavi...................(Razavi.Dev@gmail.com)
 *
 * Created on: 	Apr 22, 2015
 * Edit Date :	
 * 				Apr 22, 2015	
 */
 
#ifndef URLCODE_HEADER
#define URLCODE_HEADER

#include <string.h>

bool char_encode(char src, char *dest)
{
char legal[128]="abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXY0123456789\0";
for (int a=0; a<strlen(legal); a++)
 if (legal[a]==src)
  {
  sprintf(dest,"%c\0",src);
  return false;
  }
}

#endif
