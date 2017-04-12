#include <iostream>
#include <cstdlib>
#include <time.h>
#include <unistd.h>
#include <cstring>

/*
 * Main entry point of program
 */
int main(const int argc, char * argv[])
{
  setvbuf(stdout, NULL, _IONBF, BUFSIZ);  //Set the IO to unbuffered
  time_t t  = time(NULL); 
  struct tm *tmp = localtime(&t);

  char outstr[400];
  const char * dateArgument;
  
  //No arguments entered, use a default value
  if (argc < 2)
    dateArgument = "+%a %b %e %X %Z %Y";
  else if (argc > 2) //Too many arguments, get out of here
    {
      printf("%s: extra operand `%s'\n", argv[0], argv[argc-1]);
      return EXIT_FAILURE;
    }
  else if (argv[1][0] != '+') //Format wrong, get out of here
    {
      printf("%s: invalid date `%s'\n", argv[0], argv[1]);
      return EXIT_FAILURE;
    }
  else //Everything good, this is our argument
    dateArgument = argv[1];

  //Pass it to the date function
  strftime(outstr, sizeof(outstr), ++dateArgument, tmp);
  printf("%s\n", outstr); //Print result


  return EXIT_SUCCESS;
}
