#include <cstdlib>
#include <cstring>
#include <iostream>
#include <unistd.h>
#include <limits.h>
#include <sys/stat.h>

bool parentMode = false;
long chOctal = 0755; //Default octal

int directoryMaker(char * dirName);

/*
 * Main entry point of mkdir program.
 */
int main (const int argc, char * argv[])
{
  int opt;
  char * rawOctal;
  bool octalInput = false;
  setvbuf(stdout, NULL, _IONBF, BUFSIZ);  //Set the IO to unbuffered

  //Check for arguments
  while ((opt = getopt(argc,argv, "pm:")) != -1)
    {
      switch(opt)
	{
	case 'p':
	  parentMode = true;
	  break;
	case 'm':
	  rawOctal = optarg;
	  octalInput = true;
	  break;
	default:
	  return EXIT_FAILURE;
	  break;
	}
    }

  //If mode, check to make sure it's valid
  if (octalInput)
    {
      for (int i = 0; octalInput && rawOctal[i] != '\0'; i++)
	{
	  if (!isdigit(rawOctal[i]))
	    octalInput = false;
	  if (rawOctal[i] > 55 || rawOctal[i] < 48) //In 0-7
	    octalInput = false;
	}
      if (strlen(rawOctal) > 4) //Too long = insta invalid
	octalInput = false;
      if (!octalInput)
	{
	  printf("%s: invalid mode `%s'\n", argv[0], rawOctal);
	  return EXIT_FAILURE;
	}
      chOctal = strtol(rawOctal, NULL, 8);
    }

  //Not enough arguments, must be missing something
  if (optind >= argc)
    printf("%s: missing operand\n", argv[0]);
  else
    {
      for (int i = optind; i < argc; i++)
	{ //We're good, try to make that directory
	  if (directoryMaker(argv[i]) == -1)
	    printf("%s: cannot create directory '%s': %s\n", 
		   argv[0],argv[i],strerror(errno));
	}
    }

  return EXIT_SUCCESS;
}

/*
 * Creates a directory using the passed in argument. Returns sucess
 * status as an int.
 * @param dirName Argument to be made into a directory
 * @return 1 if all went well, -1 if miserable failure
 */
int directoryMaker(char * dirName)
{
  //Just making a directory without parent structure
  if (!parentMode && mkdir(dirName, chOctal) == -1)
      return -1;
  else //Parent structure, make the paths
    {  
      for (char * i = dirName + 1; *i; i++) //This is some path shenanigans
	{
	  if (*i == '/') //We've come across a seperator
	    {
	      *i = '\0'; //Temporarily remove it before making directory
	      
	      if (mkdir(dirName, chOctal) == -1 && errno != EEXIST)
		return -1;
	      chmod(dirName, chOctal);
	      *i = '/'; //Now put it back
	    }
	}
      //Make the stuff
      if (mkdir(dirName, chOctal) == -1 && errno != EEXIST)
	return -1;
    }
  chmod(dirName, chOctal);
  
return 1;
}
