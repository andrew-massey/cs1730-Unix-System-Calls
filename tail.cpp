#include <unistd.h>
#include <iostream>
#include <cstdlib>
#include <cstring>
#include <fcntl.h>
#include <limits.h>

bool inputArg = false;
bool lineArg = true;
bool lastFile = false; //For the -f argument
int tailHandler(int number, char * fileName);
int numFiles = 0; //Just for printing format shenanigans

/*
 * Main entry point of tail.
 */
int main(int argc, char * argv[])
{
  int opt;
  int number = 10;
  char * rawNumber;
  bool argFound = false;
  setvbuf(stdout, NULL, _IONBF, BUFSIZ);  //Set the IO to unbuffered
  
  //Check for arguments
  while ((opt = getopt(argc, argv, "fc:n:")) != -1)
    {
      switch(opt)
	{
	case 'f':
	  inputArg = true;
	  break;
	case 'c':
	  lineArg = false;
	  rawNumber = optarg;
	  argFound = true;
	  break;
	case 'n':
	  lineArg = true;
	  rawNumber = optarg;
	  argFound = true;
	  break;
	default:
	  return EXIT_FAILURE;
	  break;
	}
    }

  //Have an argument, make sure it's right
  if (argFound)
    {
      for (int i = 0; rawNumber[i] != '\0'; i++)
	{
	  if (i == 0 && rawNumber[i] == '-')
	    continue;
	  else if (!isdigit(rawNumber[i]))
	    { //It's a bad argument, get out of here
	      printf("%s: %s: invalid number of %s \n", argv[0], rawNumber, lineArg ? "lines" : "bytes");
	      return EXIT_FAILURE;	  
	    }
	}
      
      //They tried to put in a negative number, get rid of it!
      if (rawNumber[0] == '-')
	number = atoi(++rawNumber);
      else //Otherwise, just put in the number
	number = atoi(rawNumber);
    }

  numFiles = (argc - optind); //Calculate number of files for printing aesthetics

  //Start passing stuff to tail
  for (int i = optind; i < argc; i++)
    {
      if (inputArg && i == argc - 1) //We're on the last file
	lastFile = true; //This will tell us to watch it for changes
      
      if (tailHandler(number, argv[i]) == -1)
	printf("%s: cannot open `%s' for reading: %s\n", argv[0], argv[i], strerror(errno));
    }
  
  return EXIT_SUCCESS;
}

/*
 * Handles our output of file stuff, taking the number of things
 * to parse and the file to be parsed.
 * @param number Number of stuffs to parse.
 * @param fileName File to be parsed
 * @return -1 if something is bad, 1 if it all went well
 */
int tailHandler(int number, char * fileName)
{
  int fd;
  int n;
  char buffer[20480];
  
  if (*fileName == '-')
    fd = STDIN_FILENO; //It's a hyphen, check standard input
  else if ((fd = open(fileName, O_RDONLY)) < 0) //Not a hyphen, open file
    return -1;

  int sizeOfFile = 0;
  char sizeCheck[1];
  while ((n = read(fd, sizeCheck, 1)) > 0)
    sizeOfFile++;
  
  lseek(fd, 0, SEEK_SET);

  if (numFiles >= 2) //We have a lot of files, print fancy stuff to separate them
    printf("==> %s <==\n", fileName);

  if (lineArg && *fileName != '-')
    { //Not standard input and we're checking lines
      int lineCounter = 0;
      int crawler = 0;

      //Go until we reach the # of lines
      while (lineCounter < number && crawler < sizeOfFile)
	{
	  char miniBuffer[1]; //Our mini buffer to go backwards with
	  // lseek(fd, 0 - crawler, SEEK_END);  //Move backwards from the end
	  if ((n = read(fd, miniBuffer, 1)) < 0)
	    return -1;

	  if (miniBuffer[0] == '\n') //We found a line!
	    lineCounter++;
	  crawler++; //Increment so we crawl back another byte
	  lseek(fd, 0 - crawler, SEEK_END);
	}

      while ((n = read(fd, buffer, 20480)) > 0) //Print the lines we found
	printf("%s\n", buffer);
    }
  else if (*fileName != '-')
    { //Not standard input and we're checking characters
      lseek(fd, 0 - number, SEEK_END); //Go to the end minus the number of bytes

      if ((n = read(fd, buffer, number)) < 0)
	return -1;
      
      printf("%s\n", buffer); //Print what we found
 
    }
  
  if (*fileName == '-') //Read out standard input
    {
      while ((n = read(fd, buffer, 20480)) > 0)
	continue;
    }

  else if (lastFile) //We're watching this file for more input!
    {
      while ((n = read(fd, buffer, 20480)) > -1)
	write(STDOUT_FILENO, buffer, n);
    }
  close(fd);
  return 1;
}
