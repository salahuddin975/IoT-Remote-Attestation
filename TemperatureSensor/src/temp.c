/* gcc -ansi -Wall -pedantic temp.c -o temp
 */
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <time.h>
#include "ifttt.h"
#define PATH_MAX 50

extern int ifttt(char* , char* , char* , char* );
int main(int argc,char *argv[])
{
    char *ret, *s1;
    char holdval[10],serialDir[50],serialNum[50];
    char lowestStr[50], highestStr[50], currentStr[50];
    int len,newTemp,oldTemp,absTemp,highestTemp,lowestTemp;
    FILE *fp;
    char path[PATH_MAX];
	if(argc > 3)
        {
            printf("Please enter only one argument");
            return 1;
        }
    printf("%d\n", argc);
    printf("the parameter enterted %s\n", argv[1]);
    strcpy(serialDir,"/sys/bus/w1/devices/28-");
    strcat(serialDir, argv[1]);
    strcat(serialDir,"/w1_slave");
    printf("%s\n",serialDir);
    strcpy(serialNum,"/sys/bus/w1/devices/28-021312eefeaa/w1_slave");
    oldTemp = -10;
    while(1)
    {  
        fp = fopen(serialDir, "r");
        if (fp == NULL)
            {   
                printf("temperature sensor cannot be found\n");
                return 2;
            }   
        while (fgets(path, PATH_MAX, fp) != NULL)
            {   
                printf("%s", path);
                ret = strpbrk(path, "Yes");
                if(ret)
                        {
                                printf("Sensor working, will print the temp\n");
                        }
            }   
        
        len = strlen(path)- 6;
        s1 = path+len;
        strcpy(holdval,s1);
        newTemp = atoi(holdval);
        printf("this is the old value %d\n", oldTemp);
        printf("this is the new value %d\n\n", newTemp);
        fclose(fp);
        if((newTemp < 0 && oldTemp > 0))
            {
            	printf("we see a change 20 \n");
            	oldTemp = newTemp;
            	if(newTemp > highestTemp)
            		{
            			highestTemp = newTemp;
            		}
            	if(newTemp < lowestTemp)
            		{
            			lowestTemp = newTemp;
            		}
            	sprintf(lowestStr,"%d" ,lowestTemp);
            	sprintf(currentStr,"%d" ,newTemp);
            	sprintf(highestStr,"%d" ,highestTemp);
            	ifttt("https://maker.ifttt.com/trigger/TempChange/with/key/uQayuY3amwBonQr7RGtpk", highestStr, currentStr, lowestStr);
            }
        if((newTemp > 0 && oldTemp < 0))
            {
            	printf("we see a change 10\n");
            	oldTemp = newTemp;
            	if(newTemp > highestTemp)
                            {
                                    highestTemp = newTemp;
                            }
                if(newTemp < lowestTemp)
                            {
                                    lowestTemp = newTemp;
                            }
                sprintf(lowestStr,"%d" ,lowestTemp);
            	sprintf(currentStr,"%d" ,newTemp);
            	sprintf(highestStr,"%d" ,highestTemp);
            	ifttt("https://maker.ifttt.com/trigger/TempChange/with/key/uQayuY3amwBonQr7RGtpk", highestStr, currentStr, lowestStr);           
        	}        
		if(oldTemp > 0 && newTemp > 0)
            {
            	absTemp = abs(oldTemp - newTemp);
            	printf("both of them are positive\n");
            	if(absTemp >= 1000)
            	{
            		printf("Change in temp, both were positive\n");
            		if(newTemp > highestTemp)
                            {
                                    highestTemp = newTemp;
                            }
            	        if(newTemp < lowestTemp)
                            {
                                    lowestTemp = newTemp;
                            }
                sprintf(lowestStr,"%d" ,lowestTemp);
            	sprintf(currentStr,"%d" ,newTemp);
            	sprintf(highestStr,"%d" ,highestTemp);
            	ifttt("https://maker.ifttt.com/trigger/TempChange/with/key/uQayuY3amwBonQr7RGtpk", highestStr, currentStr, lowestStr);   
            	}
            }
        else
            {
            	absTemp = abs(abs(oldTemp)-abs(newTemp));
            	printf("both are negative\n");
            	if(absTemp >= 1000)
                    {
                        printf("Change in temp, both were negative\n");
            		    if(newTemp > highestTemp)
                            {
                                    highestTemp = newTemp;
                            }
            	        if(newTemp < lowestTemp)
                            {
                                    lowestTemp = newTemp;
                            }
                sprintf(lowestStr,"%d" ,lowestTemp);
            	sprintf(currentStr,"%d" ,newTemp);
            	sprintf(highestStr,"%d" ,highestTemp);
            	ifttt("https://maker.ifttt.com/trigger/TempChange/with/key/uQayuY3amwBonQr7RGtpk", highestStr, currentStr, lowestStr);     
                    }
            }
    }

return 0;
}
