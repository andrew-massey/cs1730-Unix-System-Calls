// This is the lsd function, yet another C implement of the classic ls, using UNIX functions

// Featuring "stat", "opendir", and "readdir"
// Credits: Jalil Benayachi, ECE PARIS - under MIT license
// contact [at] thejals.com

// Also thanks to some contributors on Stackoverflow 

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <time.h>
#include <pwd.h>
#include <grp.h>

int main(int argc, char* argv[])
{

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
    if(argc == 1){
      char cwd[1024];
      chdir("/path/to/change/directory/to");
      getcwd(cwd, sizeof(cwd));
      directory = opendir(cwd);
    }
    else
    {
      directory = opendir(argv[1]);
    }
    //If a file is found (readdir returns a NOT NULL value), the loop starts/keep going until it has listed all of them. 
    while((file = readdir(directory)) != NULL) 
    {   
        //We sprint "directory/file" which defines the path to our file 
        sprintf(buf, "%s/%s", argv[1], file->d_name);
        //Then we use stat function in order to retrieve information about the file
        stat(buf, &stats);

        //Now, we can print a few things !
        // Here's the right order
        // [file type] [permissions] [number of hard links] [owner] [group] [size in bytes] [time of last modification] [filename]

        // [file type]
        //Let's start with the file type
        //The stat manual is pretty complete and gives details about st_mode and S_IFMT: http://manpagesfr.free.fr/man/man2/stat.2.html
        //
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
        //Same for the permissions, we have to test the different rights
        //READ http://linux.die.net/man/2/chmod 
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
        // Quoting: http://www.gnu.org/software/libc/manual/html_node/Attribute-Meanings.html
        // "This count keeps track of how many directories have entries for this file. 
        // If the count is ever decremented to zero, then the file itself is discarded as soon as no process still holds it open."
        printf("\t%d ", (int)stats.st_nlink);

        //[owner] 
        // http://linux.die.net/man/3/getpwuid
        tf = getpwuid(stats.st_uid);
        printf("\t%s ", tf->pw_name);

        //[group]
        // http://linux.die.net/man/3/getgrgid
        gf = getgrgid(stats.st_gid);
        printf("\t%s ", gf->gr_name);

        //And the easy-cheesy part
        //[size in bytes] [time of last modification] [filename]
        printf("%zu",stats.st_size);
        printf(" %s", file->d_name);
        printf(" %s", ctime(&stats.st_mtime));
    }
    closedir(directory);
}