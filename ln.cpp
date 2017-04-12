#include <cstdlib>
#include <iostream>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <cstring>

bool symLinkArg = false;

/*
 * Main entry point for ln.
 */
int main(const int argc, char * argv[])
{
  setvbuf(stdout, NULL, _IONBF, BUFSIZ);  //Set the IO to unbuffered
  int opt;

  //Check for arguments
  while ((opt = getopt(argc,argv, "s")) != -1)
    {
      switch(opt)
	{
	case 's': //Symlink argument
	  symLinkArg = true;
	  break;
	default:
	  return EXIT_FAILURE;
	  break;
	}
    }


  if (argc < 2 || optind >= argc) //Not enough valid arguments entered
    { 
      printf("%s: missing operand\n",argv[0]);    
      return EXIT_FAILURE;
    }
  else if (argc - optind > 2)
    {
      printf("%s: too many operands\n",argv[0]);
      return EXIT_FAILURE;
    }

  if (symLinkArg) //It's a sym link, do symlink stuff
    {
      if (symlink(argv[optind], argv[argc-1]) == -1)
	printf ("%s: creating symbolic link `%s':  %s\n", argv[0], argv[argc-1], strerror(errno));
    }
  else //It's a hard link, do hard link stuff
    {
      if (link(argv[optind], argv[argc-1]) == -1)
	printf ("%s: creating hard link `%s':  %s\n", argv[0], argv[argc-1], strerror(errno));
    }

  return EXIT_SUCCESS;
}
