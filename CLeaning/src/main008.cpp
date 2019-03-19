#include <stdio.h>
#include <string>
#include <vector>

using namespace std;
int main()
{
    vector<string> vec;
    FILE * pFile;
    char mystring [100];
    const char * myfile = "/home/hdc/CLionProjects/cPlus/CLeaning/src/vector.cpp";
    pFile = fopen(myfile, "r");

    if (pFile == NULL) perror("Error opening file");
    else{
        while (!feof(pFile)){
            if ( fgets (mystring, 100, pFile) !=NULL)
            {
                vec.push_back(mystring);
                puts(mystring);
            }
        }
        fseek(pFile, 0, SEEK_END);
        long size = ftell(pFile);

        fclose(pFile);
        printf("size of vector.cpp is %ld\n", size);
    }
    printf("size of vec is %d\n", sizeof(vec));
    printf("size of vec is %ld\n", vec.size());
    return 0;
}