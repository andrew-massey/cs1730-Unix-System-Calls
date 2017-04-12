#include <time.h>
#include <cstdlib>
#include <cstdio>
#include <iostream>
#include <iomanip>
#include <string>
#include <cstring>
#include "cal.h"

using namespace std;

bool yearCal;
int offset;

int getMonth(int month, int argc, char *argv[]);
int getYear(int year, int argc, char *argv[]);
int computeOffset(int year, int month);
int numDaysYear(int year);
int numDaysMonth(int year, int month);
bool isLeapYear(int year);
void display(int year, int month, int offset, int argc);

int main(int argc, char *argv[])
{
   yearCal = false;
   int numDays;
   int offset;
   int month;
   int year;

   month = getMonth(month, argc, argv);

   year = getYear(year, argc, argv);

   offset = computeOffset(year, month);

   display(year, month, offset, argc);

   return 0;
}


int getMonth(int month, int argc, char *argv[])
{
  time_t rawtime;
  struct tm * timeinfo;
  time (&rawtime);
  timeinfo = localtime (&rawtime);
  
  char month4[10];
  char* month1;
  
  if(argc == 1){
    strftime(month4, sizeof(month1), "%m", timeinfo);
    month = strtol(month4,&month1,10);
    cout << "passed"<<endl;
  }
  else if(argc == 2){
    month = 1;
    yearCal = true;
  }
  else{
    month1 = argv[1];
    month = stoi(month1);
  }
  if(month<1||month>13){
    cout << "Illegal use: month must be between 1-12" << endl;
    return EXIT_FAILURE;
    }
   return month;
}

int getYear(int year, int argc, char *argv[])
{
  time_t rawtime;
  struct tm * timeinfo;
  time (&rawtime);
  timeinfo = localtime (&rawtime);
  
  char year4[10];
  char* year1;
  
  if(argc == 1){
    strftime(year4, sizeof(year1), "%G", timeinfo);
    year = strtol(year4,&year1,10);
    cout << "passed"<<endl;
  }
  
  else if(argc == 2){
    year1 = argv[1];
    year = stoi(year1);
  }
  else{
    year1 = argv[2];
    year = stoi(year1);
  }
  if(year>9999||year<1){
    cout << "Illegal use: year must be between 1-9999" << endl;
    return EXIT_FAILURE;
  }
   return year;
}


int computeOffset(int year, int month)
{
   int offset = 0;
   int count;
   if(year>1752){count = year - 1753;}
   else{count = year+3;}
   for ( int iYear = 0; iYear < count; iYear++)
   {
      offset = ( offset + 365 + isLeapYear(year)) % 7;
   }

   for ( int iMonth = 1; iMonth < month; iMonth++)
   {
      offset = ( offset + numDaysMonth(year, iMonth)) % 7;
   }
    offset+=1;
   return offset;
}



int numDaysYear(int year)
{
   int daysYear = 365 + isLeapYear(year);
   return daysYear;
}

int numDaysMonth(int year, int month)
{
   int daysMonth;

   if ( month == 1)
      daysMonth = 31;
   else if ( month == 2)
   {
      if (isLeapYear(year) == true)
         daysMonth = 29;
      else
         daysMonth = 28;
   }
   else if ( month == 3)
      daysMonth = 31;
   else if ( month == 4)
      daysMonth = 30;
   else if ( month == 5)
      daysMonth = 31;
   else if ( month == 6)
      daysMonth = 30;
   else if ( month == 7)
      daysMonth = 31;
   else if ( month == 8)
      daysMonth = 31;
   else if ( month == 9)
      daysMonth = 30;
   else if ( month == 10)
      daysMonth = 31;
   else if ( month == 11)
      daysMonth = 30;
   else if ( month == 12)
      daysMonth = 31;

   return daysMonth;
}

bool isLeapYear(int year)
{
  if(year<1752){
    if(year%4==0){return true;}
    else{return false;}
  }
  else{  
    if ( year % 4 == 0 && year % 100 != 0 || year % 400 == 0)
      return true;
    else
      return false;
  }
}
void display(int year, int month, int offset, int argc)
 {
   int dayCal = 0;
   if(yearCal){
      cout<<"\t     ";
      cout<<year<<endl;
    }
    if(argc==1){
      time_t rawtime;
      struct tm * timeinfo;
      time (&rawtime);
      timeinfo = localtime (&rawtime);
      char day2[10];
      char* day1;
      strftime(day2, sizeof(day1), "%d", timeinfo);
      dayCal = strtol(day2,&day1,10); 
      
    }
    
    
 do{
    if(offset == 7) offset = 0;
    int dayOfWeek;
    int day;
    if(!yearCal) cout << endl;
    cout<<"\t   ";
    if ( month == 1)
       cout << "January";
    else if ( month == 2)
       cout << "February";
    else if ( month == 3)
       cout << "March";
    else if ( month == 4)
       cout << "April";
    else if ( month == 5)
       cout << "May";
    else if ( month == 6)
       cout << "June";
    else if ( month == 7)
       cout << "July";
    else if ( month == 8)
       cout << "August";
    else if ( month == 9)
       cout << "September";
    else if ( month == 10)
       cout << "October";
    else if ( month == 11)
       cout << "November";
    else if ( month == 12)
       cout << "December";


    if(!yearCal) cout << " " << year << "\n";
    else cout<<endl;
    // Display month header
    cout << "  Su  Mo  Tu  We  Th  Fr  Sa\n";

    // Gets the correct offset width and end the line on the right
    //day of the week
    if (offset == 0)
    {
       day = 2;
       cout << setw(6);
    }
    else if (offset == 1)
    {
       day = 3;
       cout << setw(10);
    }
    else if (offset == 2)
    {
       day = 4;
       cout << setw(14);
    }
    else if (offset == 3)
    {
       day = 5;
       cout << setw(18);
    }
    else if (offset == 4)
    {
       day = 6;
       cout << setw(22);
    }
    else if (offset == 5)
    {
       day = 7;
       cout << setw(26);
    }
    else if (offset == 6)
    {
       day = 1;
       cout << setw(2);
    }
    else
       cout << "Error offset must be >= 0 and <=6\n";

    // The loop for displaying the days and ending the line in the right place
    for ( dayOfWeek = 1; dayOfWeek <= numDaysMonth(year, month); dayOfWeek++ )
    {
       cout << "  " <<  setw(2);
       if((argc==1)&&(dayOfWeek==dayCal)){
         printf("%c[%dm", 0x1B, 30);
         printf("%c[%dm", 0x1B, 47);
         cout << dayOfWeek;
         printf("%c[%dm", 0x1B, 37);
         printf("%c[%dm", 0x1B, 40);
       }
       else cout << dayOfWeek;
       
       ++day;
       if (day == 8)
       {
          cout << "\n";
          day = 1;
       }
    }
    if ( day >= 2 && day <= 7)
       cout << "\n";
    month++;
    offset = computeOffset(year, month);
    if(offset ==7) offset = 0;
  }
  while((month<13)&&yearCal);
}