/*
  Copyright (c) 2009 Dave Gamble
 
  Permission is hereby granted, free of charge, to any person obtaining a copy
  of this software and associated documentation files (the "Software"), to deal
  in the Software without restriction, including without limitation the rights
  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
  copies of the Software, and to permit persons to whom the Software is
  furnished to do so, subject to the following conditions:
 
  The above copyright notice and this permission notice shall be included in
  all copies or substantial portions of the Software.
 
  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
  THE SOFTWARE.
*/
 
#include <stdio.h>
#include <stdlib.h>
#include "cJSON.h"
 
/* Parse text to JSON, then render back to text, and print! */
void doit(char *text)
{
	char *out;cJSON *json;
	
	json=cJSON_Parse(text);
	if (!json) {printf("Error before: [%s]\n",cJSON_GetErrorPtr());}
	else
	{
		out=cJSON_Print(json);
		cJSON_Delete(json);
		printf("%s\n",out);
		free(out);
	}
}
void create_objects()
{
	cJSON *root,*fmt,*img,*thm,*fld;char *out, *out1;int i;	/* declare a few. */
    cJSON *array;
	/* Here we construct some JSON standards, from the JSON site. */
	
	/* Our "Video" datatype: */
/*	root=cJSON_CreateObject();	
	cJSON_AddItemToObject(root, "name", cJSON_CreateString("Yuwc"));
	cJSON_AddItemToObject(root, "format", fmt=cJSON_CreateObject());
	cJSON_AddStringToObject(fmt,"type",		"rect");
	cJSON_AddNumberToObject(fmt,"width",		1920);
	cJSON_AddNumberToObject(fmt,"height",		1080);
	cJSON_AddFalseToObject (fmt,"interlace");
	cJSON_AddNumberToObject(fmt,"frame rate",	24);
		cJSON *format = cJSON_GetObjectItem(root,"format");   
	char *name = cJSON_GetObjectItem(root,"name")->valuestring;
	printf("name=[%s]\n", name); 
 
	int framerate = cJSON_GetObjectItem(format,"height")->valueint; 
	printf("%d\n", framerate); 
	char *type = cJSON_GetObjectItem(format,"type")->valuestring;
	printf("%s\n", type); */	
	root=cJSON_CreateObject();	
	cJSON_AddItemToObject(root, "总记录数", cJSON_CreateNumber(115));
	
	cJSON_AddItemToObject(root, "format", array=cJSON_CreateArray());
	for (i=0;i<2;i++)
	{
		cJSON_AddItemToArray(array,fmt=cJSON_CreateObject());
		
		cJSON_AddStringToObject(fmt,"xuhao",		"01");
		cJSON_AddStringToObject(fmt,"addr", 	"192.168.100.100");
		cJSON_AddStringToObject(fmt,"name", 	"系统温度");
		cJSON_AddNumberToObject(fmt,"value",		456);
		cJSON_AddNumberToObject(fmt,"qulities", 	123);
		cJSON_AddFalseToObject (fmt,"flag");
	}	
	
	out=cJSON_Print(root);	cJSON_Delete(root);	printf("%s\n",out);	free(out);
}
int main (int argc, const char * argv[])
{
	printf( "Content-type: application/text;charset=utf-8\n\n" );
    //char text1[]="{\n\"name\": \"Jack (\\\"Bee\\\") Nimble\", \n\"format\": {\"type\":       \"rect\", \n\"width\":      1920, \n\"height\":     1080, \n\"interlace\":  false,\"frame rate\": 24\n}\n}";	
	//string输出Json格式
	//doit(text1);
	create_objects();
}
