#include <stdio.h>
#include <stdlib.h>
#include "a.h"

typedef struct _HostDeviceDataPrivateData
{
    uc_uint32
};

int main(){
    FILE* pFile;
    long lSize;
    char* buffer;
    size_t result;

    pFile = fopen( "/home/hdc/CLionProjects/cPlus/CLeaning/src/DeviceData.bin", "rb" );
    if (pFile==NULL){
        fputs("File error", stderr);
        exit(1);
    }

    fseek(pFile, 0, SEEK_END);
    lSize = ftell(pFile);
    rewind(pFile);

    buffer = (char*)malloc(sizeof(char)*lSize);
    if (buffer==NULL){fputs("Memory error", stderr);
    exit(2);}
    result = fread(buffer,1,lSize,pFile);
    if (result !=lSize){
        fputs("Reading error", stderr);
        exit(3);
    }

    for (int i = 0; i < sizeof(buffer); ++i) {
        printf("%02x", &buffer[i]);
        if ((i+1)%16 == 0)
            printf("\n");
    }
    fclose(pFile);
    free(buffer);
    return 0;
}