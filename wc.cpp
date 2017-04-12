#include <iostream>
#include <cstdlib>
#include <cstring>
#include <unistd.h>
#include <fcntl.h>
#include <string>
#include <cwchar>

char * programName;

//All of our argument checks
bool byteArg = false;
bool lineArg = false;
bool charArg = false;
bool wordArg = false;

//All of our global counts
unsigned long totalLines = 0;
unsigned long totalWords = 0;
unsigned long totalChars = 0;
unsigned long totalBytes = 0; 

void fileReader(int argFile, const char * fileName);
void charReader(const char * ptr);

/*
 * Main entry point for wc.
 */
int main(const int argc, char * argv[])
{
  std::setlocale(LC_ALL, "en_US.utf8");
  programName = argv[0]; //Store the program name for other functions
  int opt;
  setvbuf(stdout, NULL, _IONBF, BUFSIZ);  //Set the IO to unbuffered

  //Check the arguments
  while ((opt = getopt(argc,argv, "clmw")) != -1)
    {
      switch(opt)
	{
	case 'c': //Byte arguments
	    byteArg = true;
	    break;
	case 'l': //Line argument
	    lineArg = true;
	    break;
	case 'm': //Character argument
	    charArg = true;
	    break;
	case 'w': //Word argument
	    wordArg = true;
	    break;
	default:
	  return EXIT_FAILURE;
	  break;
	}
    }

  //No arguments, set our defaults
  if (!byteArg && !lineArg && !charArg && !wordArg)
      byteArg = lineArg = wordArg = true;

  int fd;

  //Cycle through arguments
  for (int i = optind; i < argc; i++)
    {
      if (* argv[i] == '-')
	fd = STDIN_FILENO; //Argument is a hyphen, echo input
      else if (argv[i][0] != '-')
	fd = open(argv[i], O_RDONLY); //Not a hyphen or parameter, open the file
      fileReader(fd, argv[i]);
    }

  close(fd);
  
  //We have multiple files, so print out the totals
  if (argc - optind >= 2)
    {
      if (lineArg)
	printf("%ld\t", totalLines);
      if (wordArg)
	printf("%ld\t", totalWords);
      if (charArg)
	printf("%ld\t", totalChars);
      if (byteArg)
	printf("%ld\t", totalBytes);
      printf("total\n");
    }

  return EXIT_SUCCESS;
}

/*
 * Reads the file and counts characters, bytes, lines, and words.
 * @param argFile The file descriptor to be read
 * @param fileName The file name, for printing purposes
 */
void fileReader(int argFile, const char * fileName)
{
  unsigned long byteCount = 0;
  unsigned long lineCount = 0;
  unsigned long wordCount = 0;
  unsigned long charCount = 0;

  const int BUFF_SIZE = 1;
  int n = 0;
  char buffer[BUFF_SIZE];
  bool isInWord = false;

  //Go through the file
  while ((n = read(argFile, buffer, BUFF_SIZE)) > 0)
    { 
      byteCount++; //Must be a byte if we're here
      totalBytes++;

      //Multibyte character stuff
      wchar_t wide_char;
      int nz;
      std::mbstate_t state = std::mbstate_t();
      nz = std::mbrtowc (&wide_char, buffer, 4, &state);
      if (nz > -1)	
	{
	  charCount++;
	  totalChars++;
	}
      //It's not a space, so it must be a word
      if (!isspace(wide_char))
	{
	  if (!isInWord) //It wasn't already a word, count it
	    {	 
	      wordCount++;
	      totalWords++;
	      isInWord = true;
	      continue;
	    }
	}
      else //It is a space of some kind
	{
	  if (buffer[0] == '\n')
	    { //It's a line, count it
	      lineCount++;
	      totalLines++;
	    }
	  isInWord = false; //We're not in a word anymore
	}
      
    }

  //Print our local counts
  if (lineArg)
    printf("%ld\t", lineCount);
  if (wordArg)
    printf("%ld\t", wordCount);
  if (charArg)
    printf("%ld\t", charCount);
  if (byteArg)
    printf("%ld\t", byteCount);
  printf("%s\n", fileName);

  if (n == -1) //Filename invalid, cry about it
    std::cout << programName << ": "
	      << fileName << ": No such file or directory." << std::endl;
}
