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
    printf("Copying : %s \n",source); 
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
	    printf("Echec target : %s \n", target);
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
    printf("%s : Done \n",source);

}

void copyDir(char * sourcePath, char * targetPath) {
	printf("Copying : %s \n",sourcePath);
    char * source;
    char * target;

    if ( ! testFn(sourcePath,'/')){
        source = malloc( strlen(sourcePath)+1);
        strcpy(source,sourcePath);
        strcat(source,"/");
    } else {
	source = malloc(strlen(sourcePath));
        source = sourcePath;
    }

    if ( ! testFn(targetPath,'/')){
        target = malloc( strlen(targetPath)+1);
        strcpy(target,targetPath);
        strcat(target,"/");
    } else {
	target = malloc(strlen(targetPath));
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
	//sourceFile = "\0";
	//targetFile = "\0";
	//printf("Buffer : %s\nsource : %s\nfilename : %s\n",sourceFile,source,filename);
        strcpy(sourceFile,source);
        strcat(sourceFile,filename);
	//printf("Buffer : %s\nsource : %s\nfilename : %s\n",sourceFile,source,filename);

	//printf("Buffer : %s\ntarget : %s\nfilename : %s\n",targetFile,target,filename);
        strcpy(targetFile,target);
        strcat(targetFile,filename);
	//printf("Buffer : %s\ntarget : %s\nfilename : %s\n",targetFile,target,filename);

        struct stat stats;          
        stat(sourceFile, &stats);

        if (S_ISDIR(stats.st_mode)){
            mkdir(targetFile,stats.st_mode);
            copyDir(sourceFile,targetFile);
        } else {
            copyFile(sourceFile,targetFile);
        }
	printf("SourceFile : %p\nTargetFile : %p\n",sourceFile,targetFile);
	free(sourceFile);
	free(targetFile);  
    }
	printf("source : %p\ntarget: %p\n",target,source);
	closedir(pDir);
    printf("%s : Done \n", source);
}

int main (int argc, char** argv) {
    if (argc != 3 )
        usage(argv[0]);
    // si on a pas une cible et une source, on sort et on rappelle à l'utilisateur comment utiliser le programme
    char * source;
    char * target;

    if ( ! testFn(argv[1],'/')){
        source = malloc( strlen(argv[1])+1);
        strcat(source,argv[1]);
        strcat(source,"/");
    } else {
        source = argv[1];
    }

    if ( ! testFn(argv[2],'/')){
        target = malloc( strlen(argv[2])+1);
        strcat(target,argv[2]);
        strcat(target,"/");
    } else {
        target = argv[2];
    }

    struct stat stats;          
    stat(source, &stats);

    if (S_ISDIR(stats.st_mode)){
        mkdir(target,stats.st_mode & 0777);
        copyDir(source,argv[2]);
    } else {
        copyFile(source,argv[2]);
    }	 
    printf("Done \n");
    return 0;
}
