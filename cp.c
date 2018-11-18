#include <stdio.h>
#include <stdlib.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <dirent.h>

#include <errno.h>
#include <unistd.h>
#include <string.h>
 
int testFn(const char *str,char toTest)
{
    return (str && *str && str[strlen(str) - 1] == toTest) ? 1 : 0;
}

void usage(char * name) {
    printf("usage : %s <pathFromSource> <pathFromTarget> \n",name);
    exit(84);
}

void errorPrint() {
    fprintf(stderr,"ERROR: %s\n", strerror(errno));
    exit(84);
}

void copyFile(char * source, char * target) {
    int fdSource = open(source,O_RDONLY);
    if ( fdSource < 0 ) {
        printf("Echec de l'ouverture de la source \n");
        errorPrint();
    }
        
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

    char * buffer = malloc(4096 * sizeof(char));
    while(1) {
	    int readByts = read(fdSource, &buffer, 4096);
    	if (readByts <0) {
            printf("Echec Lecture \n");
            errorPrint();
        }

        if (readByts == 0)
            break;


        int writenByts = write(fdTarget, &buffer, 4096);
	
        if (writenByts < 0) {
            printf("Echec Ecriture \n");
            errorPrint();
	    }
    }
    close(fdTarget);
    close(fdSource);
    printf("%s : done",source);

}

void copyDir(char * sourcePath, char * targetPath) {
    char * source;
    char * target;

    if ( ! testFn(sourcePath,'/')){
        source = malloc( srtlen(sourcePath)+1);
        strcat(source,sourcePath);
        strcat(source,"/");
    } else {
        source = sourcePath;
    }

    if ( ! testFn(targetPath,'/')){
        target = malloc( srtlen(targetPath)+1);
        strcat(target,targetPath);
        strcat(target,"/");
    } else {
        target = targetPath;
    }

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

        struct stat stats;          
        int test = fstat(sourceFile, &stats);

        if (S_ISDIR(stats.st_mode)){
            mkdir(sourceFile,stats.st_mode & ~unmask & 0777);
            copyDir(sourceFile,targetFile)
        } else {
            copyFile(sourceFile,targetFile);
        }   
    }
	closedir(pDir);
    printf("%s : Done ", source);
}

int main (int argc, char** argv) {
    if (argc != 3 )
        usage(argv[0]);
    // si on a pas une cible et une source, on sort et on rappelle à l'utilisateur comment utiliser le programme
    char * source;
    char * target;

    if ( ! testFn(argv[1],'/')){
        source = malloc( srtlen(argv[1])+1);
        strcat(source,argv[1]);
        strcat(source,"/");
    } else {
        source = argv[1];
    }

    if ( ! testFn(argv[2],'/')){
        target = malloc( srtlen(argv[2])+1);
        strcat(target,argv[2]);
        strcat(target,"/");
    } else {
        target = argv[2];
    }

    struct stat stats;          
    int test = fstat(source, &stats);

    if (S_ISDIR(stats.st_mode)){
        mkdir(source,stats.st_mode & ~unmask & 0777);
        copyDir(source,argv[2])
    } else {
        copyFile(source,argv[2]);
    }  
    printf("Done \n");
    return 0;
}
