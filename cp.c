#include <stdio.h>
#include <stdlib.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <dirent.h>

#include <errno.h>
#include <unistd.h>
#include <string.h>
 

void usage(char * name) {
    printf("usage : %s <pathFromSource> <pathFromTarget \n",name);
    exit(84);
}

void errorPrint() {
    fprintf(stderr,"ERROR: %s\n", strerror(errno));
    exit(84);
}

void copyFile(char * source, char * target) {
    int fdSource = open(source,O_RDONLY);
    if ( fdSource == -1 )
        errorPrint();
    // si il y a eut une erreur, on l'affiche et on sort

    struct stat stats;          
    int test = fstat(fdSource, &stats);
    if (test == -1)
	errorPrint();

    int fdTarget = open(target+,O_CREAT|O_WRONLY|O_TRUNC, stats.st_mode);
    if ( fdTarget == -1 )
        errorPrint();

    int * buffer = malloc(4096);

    while(1) {
	int readByts = read(fdSource, &buffer, 4096);
        if (readByts == -1)
            errorPrint();
	
	if (readByts == 0)
		break;
        int writenByts = write(fdTarget, &buffer, 4096);
        if (writenByts == -1)
            errorPrint();
    }
}

void copyDir(char * source, char * target) {
    struct dirent *pDirent;
    DIR *pDir;

    pdir = opendir(source);
    if (pdir == NULL)
        errorPrint();

    while ((pDirent = readdir(pDir)) != NULL) {
        char * filename = pDirent->d_name;
        copyFile(source+"/"+filename,target+"/"+filename);

    }
}

int main (int argc, char** argv) {
    if (argc != 3 )
        usage(argv[0]);
    // si on a pas une cible et une source, on sort et on rappelle à l'utilisateur comment utiliser le programme

    copyDir(argv[1],argv[2]);
    printf("Done \n");
    return 0;
}
