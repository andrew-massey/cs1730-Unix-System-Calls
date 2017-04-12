#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <time.h>
#include <pwd.h>
#include <grp.h>
#include<dirent.h>
#include<iostream>
#include <cstring>
#include <string>
#include <cstdlib>

using namespace std;

bool l;
bool a;

void defaultLS(int argc, char* argv[]);
/**
* main(int argc,char *argv[]) - the driver for the program
*
* @params:
*  argc - num of args
*  argv - arguments
*/
int main(int argc, char* argv[])
{
  a = false;
  l = false;

    if(argc == 1){
      defaultLS(argc, argv);
      return EXIT_SUCCESS;
    }
    string optcheck = argv[1];
       char *point = &optcheck.at(0);
       char optcheckchar = '-';
       char *optcheckptr = &optcheckchar;
    
    if(strcmp(point, optcheckptr)){
    char opt;
    for(int i = 0; i<3; i++){
      opt = getopt(argc, argv, "la:");
      switch(opt)
      {
        case 'l':
            l = true;
            break;
        case 'a':
            a = true;
            break;
      }
    }
    if(a&&!l){defaultLS(argc,argv);}
    
    } 

if(a&&!l){defaultLS(argc,argv);}

if(l){    
    //Defining the different components of the program
        //The directory: it's the folder we're browsing (we'll use an argument (argv) in order to identify it)
    DIR *directory;
        //The file: when a file is found in the directory readdir loop, it's going to be called this way.
    struct dirent *file;
        //The stat: It's how we'll retrieve the stats associated to the file. 
    struct stat stats;
        //will be used to determine the file owner & group
    struct passwd *tf; 
    struct group *gf;

    //Creating a placeholder for the string. 
    //We create this so later it can be properly adressed.
    //It's reasonnable here to consider a 512 maximum lenght, as we're just going to use it to display a path to a file, 
    //but we could have used a strlen/malloc combo and declared a simple buf[] at this moment
    char buf[512];

    //It's time to assign directory to the argument: this way the user will be able to browse any folder simply by mentionning it 
    //when launching the lsd program.
    char cwd[1024];
    if(argc == 2){
      
      chdir("/path/to/change/directory/to");
      getcwd(cwd, sizeof(cwd));
      directory = opendir(cwd);
    }
    else{directory = opendir(argv[2]);}
    //If a file is found (readdir returns a NOT NULL value), the loop starts/keep going until it has listed all of them. 
    
    if(!a){
        //Then we use stat function in order to retrieve information about the file
        stat("..", &stats);

        // [file type]
        switch (stats.st_mode & S_IFMT) {
            case S_IFBLK:  printf("b "); break;
            case S_IFCHR:  printf("c "); break; 
            case S_IFDIR:  printf("d "); break; //It's a (sub)directory 
            case S_IFIFO:  printf("p "); break; //fifo
            case S_IFLNK:  printf("l "); break; //Sym link
            case S_IFSOCK: printf("s "); break;
            //Filetype isn't identified
            default:       printf("- "); break;
                }
        //[permissions]
        
        printf( (stats.st_mode & S_IRUSR) ? " r" : " -");
        printf( (stats.st_mode & S_IWUSR) ? "w" : "-");
        printf( (stats.st_mode & S_IXUSR) ? "x" : "-");
        printf( (stats.st_mode & S_IRGRP) ? "r" : "-");
        printf( (stats.st_mode & S_IWGRP) ? "w" : "-");
        printf( (stats.st_mode & S_IXGRP) ? "x" : "-");
        printf( (stats.st_mode & S_IROTH) ? "r" : "-");
        printf( (stats.st_mode & S_IWOTH) ? "w" : "-");
        printf( (stats.st_mode & S_IXOTH) ? "x" : "-");

        // [number of hard links] 
        
        printf("\t%d ", (int)stats.st_nlink);

        //[owner] 
        tf = getpwuid(stats.st_uid);
        printf("\t%s ", tf->pw_name);

        //[group]
        gf = getgrgid(stats.st_gid);
        printf("\t%s ", gf->gr_name);

        //[size in bytes] [time of last modification] [filename]
        printf("%zu",stats.st_size);
        printf(" ..");
        printf(" %s", ctime(&stats.st_mtime));   
        //Then we use stat function in order to retrieve information about the file
        stat(".", &stats);

        // [file type]
        switch (stats.st_mode & S_IFMT) {
            case S_IFBLK:  printf("b "); break;
            case S_IFCHR:  printf("c "); break; 
            case S_IFDIR:  printf("d "); break; //It's a (sub)directory 
            case S_IFIFO:  printf("p "); break; //fifo
            case S_IFLNK:  printf("l "); break; //Sym link
            case S_IFSOCK: printf("s "); break;
            //Filetype isn't identified
            default:       printf("- "); break;
                }
        //[permissions]
        
        printf( (stats.st_mode & S_IRUSR) ? " r" : " -");
        printf( (stats.st_mode & S_IWUSR) ? "w" : "-");
        printf( (stats.st_mode & S_IXUSR) ? "x" : "-");
        printf( (stats.st_mode & S_IRGRP) ? "r" : "-");
        printf( (stats.st_mode & S_IWGRP) ? "w" : "-");
        printf( (stats.st_mode & S_IXGRP) ? "x" : "-");
        printf( (stats.st_mode & S_IROTH) ? "r" : "-");
        printf( (stats.st_mode & S_IWOTH) ? "w" : "-");
        printf( (stats.st_mode & S_IXOTH) ? "x" : "-");

        // [number of hard links] 
        
        printf("\t%d ", (int)stats.st_nlink);

        //[owner] 
        tf = getpwuid(stats.st_uid);
        printf("\t%s ", tf->pw_name);

        //[group]
        gf = getgrgid(stats.st_gid);
        printf("\t%s ", gf->gr_name);

        //[size in bytes] [time of last modification] [filename]
        printf("%zu",stats.st_size);
        printf(" .");
        printf(" %s", ctime(&stats.st_mtime));
    }
    
    
    
    while((file = readdir(directory)) != NULL) 
    {   
        //We sprint "directory/file" which defines the path to our file 
        if(argc == 2){sprintf(buf, "%s/%s", cwd, file->d_name);}
        else{sprintf(buf, "%s/%s", argv[2], file->d_name);}
        //Then we use stat function in order to retrieve information about the file
        stat(buf, &stats);

        // [file type]
        switch (stats.st_mode & S_IFMT) {
            case S_IFBLK:  printf("b "); break;
            case S_IFCHR:  printf("c "); break; 
            case S_IFDIR:  printf("d "); break; //It's a (sub)directory 
            case S_IFIFO:  printf("p "); break; //fifo
            case S_IFLNK:  printf("l "); break; //Sym link
            case S_IFSOCK: printf("s "); break;
            //Filetype isn't identified
            default:       printf("- "); break;
                }
        //[permissions]
        
        printf( (stats.st_mode & S_IRUSR) ? " r" : " -");
        printf( (stats.st_mode & S_IWUSR) ? "w" : "-");
        printf( (stats.st_mode & S_IXUSR) ? "x" : "-");
        printf( (stats.st_mode & S_IRGRP) ? "r" : "-");
        printf( (stats.st_mode & S_IWGRP) ? "w" : "-");
        printf( (stats.st_mode & S_IXGRP) ? "x" : "-");
        printf( (stats.st_mode & S_IROTH) ? "r" : "-");
        printf( (stats.st_mode & S_IWOTH) ? "w" : "-");
        printf( (stats.st_mode & S_IXOTH) ? "x" : "-");

        // [number of hard links] 
        
        printf("\t%d ", (int)stats.st_nlink);

        //[owner] 
        tf = getpwuid(stats.st_uid);
        printf("\t%s ", tf->pw_name);

        //[group]
        gf = getgrgid(stats.st_gid);
        printf("\t%s ", gf->gr_name);

        //[size in bytes] [time of last modification] [filename]
        printf("%zu",stats.st_size);
        printf(" %s", file->d_name);
        printf(" %s", ctime(&stats.st_mtime));
    }
    closedir(directory);
}
}

/**
*defaultLS(int argc, char *argv[]) - prints out the most basic version of ls, aside from the -a option
*
* @params:
*  argc - num of args
*  argv - args
*/

void defaultLS(int argc, char *argv[]){
    DIR *dp;
	  dirent *d;
    if(a) {argc = 1;}
    //failsafe if my code goes bonkers and it took another folder as a param
    if(argc>1){
    	if((dp = opendir(argv[1])) != NULL)
    		perror("opendir");
    }
    //everything is fine
    else{
      char cwd[1024];
      chdir("/path/to/change/directory/to");
      getcwd(cwd, sizeof(cwd));
      dp = opendir(cwd);
    }
	while((d = readdir(dp)) != NULL)
	{
		if((!strcmp(d->d_name,".") || !strcmp(d->d_name,".."))&a) //excludes '.' files
			continue;

		cout << d->d_name << endl;
	}
}
