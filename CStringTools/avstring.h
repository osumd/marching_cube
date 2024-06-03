#ifndef CSV_STRING_H
#define CSV_STRING_H

#include <string.h>
#include <stdio.h>


#pragma region csvStrMemOp

typedef enum {
    CSV_ERROR,
    CSV_NO_ERROR
} csvError ;

//********************************************************
//! @brief Copy string to destination from source with bounds checking involved.
//! Destination, sourceString, maxDestinationLength, sourceLength. Assumes that destination is the start.
//! @return 
//********************************************************
csvError csvstrcpy(char* destination, const char* source, int maxLength, int sourceLength)
{
    if(sourceLength > maxLength)
    {
        return CSV_ERROR;
    }
    memcpy(destination, source, sourceLength);
    return CSV_NO_ERROR;
}

//********************************************************
//! @brief Internal use for paramter division use hcsvcat, use this to concatenate with more parameter knowledge.
//!
//! @return 
//********************************************************
csvError csvstrcat(const char* strOne, int SZstrOne, const char* strTwo, int SZstrTwo, char* DST, int dstLength)
{
    int csvERR = 0;
    csvERR = csvstrcpy(DST, strOne, dstLength, SZstrOne);
    if(csvERR == CSV_ERROR)
    {
        return CSV_ERROR;
    }
    csvERR = csvstrcpy(DST+SZstrOne, strTwo, dstLength, SZstrTwo);
    if(csvERR == CSV_ERROR)
    {
        return CSV_ERROR;   
    }
    return CSV_NO_ERROR;
}

//********************************************************
//! @brief Concatenates two strings together.
//!
//! @return 
//********************************************************
csvError hcsvcat(const char* stringOne, const char* stringTwo, char* destination, int destinationLength)
{
    csvError currentError = CSV_NO_ERROR;

    int sizeStringOne = strlen(stringOne);
    int sizeStringTwo = strlen(stringTwo);

    currentError = csvstrcat(stringOne, sizeStringOne, stringTwo, sizeStringTwo, destination, destinationLength);
    
    return currentError;
}

#pragma endregion


#endif