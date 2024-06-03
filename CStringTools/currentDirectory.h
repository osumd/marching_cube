#ifndef CURRENT_DIRECTORY_H
#define CURRENT_DIRECTORY_H 

#include "avstring.h"
#include "currentDirectory.h"

char* getCurrentDirectory(int desiredFileDepth, int* errorStatus)
{   
    int len = strlen(__FILE__);
    char* str = new char[len+1];
    strcpy(str, __FILE__);
    int fileDepth = 0;
    for(int i = len-1; i > 0; i--)
    {
        if(str[i] == '/')
        {

            if(fileDepth >= desiredFileDepth)
            {
                str[i] = '/';
                if(i < len-2)
                {
                    str[i+1] = '\0';
                }else
                {
                    *errorStatus = 1;
                    return str;
                }
                *errorStatus = 0;
                return str;
            }else
            {
                fileDepth++;
            }
        }
    }
    printf("string: %s", str);
    *errorStatus = 1;
    return str;
}
char* getCurrentDirectoryFile(const char* fileSource, int desiredFileDepth, int* errorStatus)
{
    int len = strlen(__FILE__);
    int lenSrc = strlen(fileSource);

    int fileDepth = 0;

    char* str = new char[len+lenSrc+1];
    strcpy(str, __FILE__);

    int endFirstStr = 0;
    
    for(int i = len-1; i > 0 && endFirstStr == 0; i--)
    {
        if(str[i] == '/')
        {
            if(fileDepth >= desiredFileDepth)
            {
                if(i < len-2)
                {
                    str[i+1] = '\0';
                    endFirstStr = i+1;
                    strcpy(str+(endFirstStr), fileSource);

                }else
                {
                    *errorStatus = 1;
                    return str;
                }

                *errorStatus = 0;
                return str;
            }
            else
            {
                fileDepth++;
            }
        }
    }

    
    *errorStatus = 1;
    return str;
}

FILE* getFileWithDirectory(const char* fileSource, int desiredFileDepth, const char* filemode, int* errorStatus)
{
    int len = strlen(__FILE__);
    int lenSrc = strlen(fileSource);
    
    int fileDepth = 0;

    char* str = new char[len+lenSrc+1];
    strcpy(str, __FILE__);

    int endFirstStr = 0;
    
    for(int i = len-1; i > 0 && endFirstStr == 0; i--)
    {
        if(str[i] == '/')
        {
            if(fileDepth >= desiredFileDepth)
            {
                if(i < len-2)
                {
                    str[i+1] = '\0';
                    endFirstStr = i+1;
                    strcpy(str+(endFirstStr), fileSource);

                }else
                {
                    *errorStatus = 1;
                    return nullptr;
                }
            }
            else
            {
                fileDepth++;
            }
        }
    }

    
    
    
    FILE* ourFile = nullptr;
    errno_t ourFileError = fopen_s(&ourFile, str, filemode);
    if(ourFile == nullptr || ourFileError != 0)
    {
        *errorStatus = 1;
        return nullptr;
    }
    *errorStatus = 0;

    return ourFile;

}

char* stripLast(const char* fileSource, char selected, int desiredFileDepth)
{
    int len = strlen(fileSource);
    char* str = new char[len+1];
    strcpy(str, fileSource);
    int fileDepth = 0;
    for(int i = len-1; i > 0; i--)
    {
        if(str[i] == selected)
        {

            if(fileDepth >= desiredFileDepth)
            {
                str[i] = '/';
                if(i < len-2)
                {
                    str[i+1] = '\0';
                }else
                {
                    return str;
                }
                return str;
            }else
            {
                fileDepth++;
            }
        }
    }
    printf("string: %s", str);
    return str;
}

#endif