#include <sys/types.h>
#include <string>
#include <sys/stat.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <time.h>
#include <pwd.h>
#include <grp.h>

using namespace std;

int main(int argc, char* argv[]){
    //The directory: it's the folder we're browsing (we'll use an argument (argv) in order to identify it)
    DIR *dir;
    //The file: when a file is found in the directory readdir loop, it's going to be called this way.
    struct dirent *fil;
    //The stat: It's how we'll retrieve the stats associated to the file. 
    struct stat stats;
    //will be used to determine the file owner & group
    struct passwd *tf; 
    struct group *gf;
    
    string pathString = "";
    
    char buf[512];
    if(argc>1){
        dir = opendir(argv[1]);
        pathString = argv[1];
    }
    else
    {
      dir = opendir(".");
      pathString = ".";
    }
    while((fil = readdir(dir)) != NULL) 
    {   
        //defines the path to our file 
        sprintf(buf, "%s/%s", pathString, fil->d_name);
        //retrieve information about the file
        stat(buf, &stats);

        // [file type]
        switch (stats.st_mode & S_IFMT) {
            case S_IFBLK:  printf("b"); break;
            case S_IFCHR:  printf("c"); break; 
            case S_IFDIR:  printf("d"); break; //It's a (sub)directory 
            case S_IFIFO:  printf("p"); break; //fifo
            case S_IFLNK:  printf("l"); break; //Sym link
            case S_IFSOCK: printf("s"); break;
            //Filetype isn't identified
            default:       printf("-"); break;
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
        printf(" %s", fil->d_name);
        printf(" %s", ctime(&stats.st_mtime));
    }
    closedir(dir);
}