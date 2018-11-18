#include <stdio.h>
#include <stdlib.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include <errno.h>
#include <unistd.h>
#define 

void usage(char * name) {
    printf("usage : %s <pathFromSource> <pathFromTarget \n",name);
    exit 84;
}

void errorPrint() {
    fprintf(2,"ERROR: %s\n", strerror(errno));
    exit 84;
}

void copyFile(char * source, char * target) {
    int fdSource = open(source,O_RDONLY);
    if ( fdSource == -1 )
        errorPrint();
    // si il y a eut une erreur, on l'affiche et on sort

    struct stat stats;          
    stat(fdSource, &stats);

    int fdTarget = open(target,O_CREAT|O_WRONLY|O_TRUNC, stats.st_mode);
    if ( fdTarget == -1 )
        errorPrint();

    int * buffer = malloc(4096);

    while( (readByts = read(fdSource, &buffer, 4096)) != 0 ) {
        if (readByts == -1)
            errorPrint();

        int writenByts = write(fdTarget, &buffer, 4096);
        if (writenByts == -1)
            errorPrint();

    }
}

int main (int argc, char** argv) {
    if (argc != 3 )
        usage(argv[0]);
    // si on a pas une cible et une source, on sort et on rappelle à l'utilisateur comment utiliser le programme

    copyFile(argv[1],argv[2]);
    
}