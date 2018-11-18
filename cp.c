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
    printf("usage : %s <pathFromSource> <pathFromTarget> \n",name);
    exit(84);
}

void errorPrint() {
    fprintf(stderr,"ERROR: %s\n", strerror(errno));
    exit(84);
}

void copyFile(char * source, char * target) {
    printf("Source : %s \n Target : %s \n", source, target);
    int fdSource = open(source,O_RDONLY);
    if ( fdSource < 0 )
        errorPrint();
    // si il y a eut une erreur, on l'affiche et on sort

    struct stat stats;          
    int test = fstat(fdSource, &stats);
    if (test < 0){
	printf("Echec stat source \n");
	errorPrint();
    }

    
    int fdTarget = open(target,O_CREAT|O_WRONLY|O_TRUNC, stats.st_mode);
    if ( fdTarget <0){
	printf("Echec target \n");
        errorPrint();
    }

    printf(" 1 fd target : %d \n",fdTarget);
    char * buffer = malloc(4096 * sizeof(char));
    printf(" 2 fd target : %d \n",fdTarget);
    printf(" Buffer Avant la boucle : %s \n",buffer);
    while(1) {
	printf(" 3 fd target : %d \n",fdTarget);
	int readByts = read(fdSource, &buffer, 4096);
	//printf("Buffer après la boucle : %s \n",buffer);
	printf(" 4 fd target : %d \n",fdTarget);
        if (readByts <0) {
	    printf("Echec Lecture \n");
            errorPrint();
	}
	printf(" 5 fd target : %d \n",fdTarget);

	if (readByts == 0)
		break;

	printf(" 6 fd target : %d \n",fdTarget);
        int writenByts = write(fdTarget, &buffer, 4096);
	
        if (writenByts < 0) {
            printf("Echec Ecriture \n");
            errorPrint();
	}
    }
}

void copyDir(char * source, char * target) {
    struct dirent *pDirent;
    DIR *pDir;
    pDir = opendir(source);

    if (pDir == NULL)
        errorPrint();

    while ((pDirent = readdir(pDir)) != NULL) {
	char * filename = pDirent->d_name;
	if(strcmp(filename, "..") == 0) continue;
        if(strcmp(filename, ".") == 0) continue;
	char * sourceFile = malloc(strlen(filename) + strlen(source) + 1);
	char * targetFile = malloc(strlen(filename) + strlen(target) +1);
	strcat(sourceFile,source);
	strcat(sourceFile,filename);
	strcat(targetFile,target);
	strcat(targetFile,filename);
        copyFile(sourceFile,targetFile);

    }
	printf("Avant pdir");
	closedir(pDir);
	printf("Après pdir");
}

int main (int argc, char** argv) {
    if (argc != 3 )
        usage(argv[0]);
    // si on a pas une cible et une source, on sort et on rappelle à l'utilisateur comment utiliser le programme

    copyDir(argv[1],argv[2]);
    printf("Done \n");
    return 0;
}
