#include <stdio.h>
#include <unistd.h>
#include <dirent.h>

extern int optdin;

int main(int argc, char* argv[]) {

    DIR *dir;
    struct dirent *ent;
    if ((dir = opendir ("./BS_dir")) != NULL) {
    /* print all the files and directories within directory */
    while ((ent = readdir (dir)) != NULL) {
        printf ("%s\n", ent->d_name);
    }
    closedir (dir);
    } else {
    /* could not open directory */
    perror ("");
    return 1;
    }

    /*
    char c = getopt(argc, argv, "d");

    printf("OPTIND %d\n", optind);
    printf("c %c\n", c);

    if(c == 'd') {
        printf("Argument after -d: %s\n", argv[optind]);
    }
    */


    return 0;
}