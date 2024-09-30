#include <stdio.h>
#include <fcntl.h>
#include <sys/types.h>
#include <unistd.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
 
#define BUFFER_SIZE 16
 
int main(int argc, char* argv[]){
    if(argc < 2){
        printf("File name was not given by arguments\n");
        exit(1);
    }
  
    const char* filename = argv[1];
 
    int file = open(filename, O_RDONLY);

    if(file == -1){
        printf("Something went wrong while opening the file. Error %s\n", strerror(errno));
        exit(errno);
    }
 
    char* buffer =(char*) malloc(BUFFER_SIZE + 1);

    long overallBytes = 0;
 
    long readBytes = 0;
 
    while(true){
 
        readBytes = read(file, buffer, BUFFER_SIZE);
        
        if(readBytes == -1){
            printf("Something went wrong while reading the file. Error %s\n", strerror(errno));
            exit(errno);
        }
        
        if(readBytes == 0){
            break;
        }

        overallBytes += readBytes;

        buffer[readBytes] = '\0';
 
        printf("%s", buffer);
    }
 
    close(file);
 
    free(buffer);
 
    printf("Completed reading file. Overall bytes: %ld\n", overallBytes);
 
    return 0;
}
