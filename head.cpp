#include <cstdlib>
#include <cstdio>
#include <cstring>
#include <iostream>
#include <unistd.h>
#include <getopt.h>

#define LINELEN 256

using namespace std;

void copy_file(FILE *fin, FILE* fout, int lineLimit);

int main(int argc, char *argv[])
{
    int lineLimit = 10;
    char cpr[]= "-n";
    FILE *fp;
    int c = 0;
  
    if(argc==1)
    {
        char* fil = 0;
        cin >> fil;
        if((fp= fopen(fil, "r"))!=NULL)
        {
            copy_file(fp, stdout, lineLimit);
            fclose(fp);
            exit(1);
        }
        copy_file(fp, stdout, lineLimit);
        exit(1);
    }
    
    if(argc > 2){
       lineLimit = 0;
       while ((c = getopt (argc, argv, "123456789")) != -1){
          switch (c)
          {
          case '1':
            lineLimit = (lineLimit*10)+ 1;
            break;
          case '2':
            lineLimit = (lineLimit*10)+ 2;
            break;
          case '3':
            lineLimit = (lineLimit*10)+ 3;
            break;
          case '4':
            lineLimit = (lineLimit*10)+ 4;
            break;
          case '5':
            lineLimit = (lineLimit*10)+ 5;
            break;
          case '6':
            lineLimit = (lineLimit*10)+ 6;
            break;
          case '7':
            lineLimit = (lineLimit*10)+ 7;
            break;
          case '8':
            lineLimit = (lineLimit*10)+ 8;
            break;
          case '9':
            lineLimit = (lineLimit*10)+ 9;
            break;
          case '0':
            lineLimit = (lineLimit*10)+ 0;
            break;
          
          default:
            abort ();
          }        
       }
    }
    cout << lineLimit<< endl;
    if((fp= fopen(argv[2], "r"))!=NULL)
    {
        copy_file(fp, stdout, lineLimit);
        fclose(fp);
        exit(1);
    }
   return 0;
}

void copy_file(FILE *fin, FILE* fout, int lineLimit)
{
    char line[LINELEN];
    int lineno = 1;

    while ((fgets(line, LINELEN, fin))&&(lineno<(lineLimit+1)))
    {
        if (fputs(line, fout) == EOF)
        {
            perror("Write to stdout failed.");
            return;
        }

        ++lineno;
    }
}