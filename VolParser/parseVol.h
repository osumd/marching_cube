
#ifndef PARSE_VOL_H
#define PARSE_VOL_H 

#include "currentDirectory.h"
#include "endianess.h"
#include <memory>
#include <math.h>


// Define a struct to represent a half-precision float
struct HalfFloat {
    uint16_t value;

    // Constructor to convert 16-bit binary representation to a half-precision float
    HalfFloat(uint16_t val) : value(val) {}

    // Function to convert the half-precision float to a regular float
    float toFloat() const {
        // Implement the conversion logic according to the half-precision format
        // This is a simplified example and may not cover all edge cases
        // You might need to adjust this based on your specific requirements
        uint16_t mantissa = value & 0x03FF;
        uint16_t exponent = (value >> 10) & 0x1F;
        int sign = (value >> 15) & 0x01;

        int biased_exponent = exponent - 15 + 127;
        uint32_t result = (sign << 31) | (biased_exponent << 23) | (mantissa << 13);

        float converted;
        memcpy(&converted, &result, sizeof(float));
        return converted;
    }
};

float halftoFloat(uint16_t value) {
    // Implement the conversion logic according to the half-precision format
    // This is a simplified example and may not cover all edge cases
    // You might need to adjust this based on your specific requirements
    uint16_t mantissa = value & 0x03FF;
    uint16_t exponent = (value >> 10) & 0x1F;
    int sign = (value >> 15) & 0x01;

    int biased_exponent = exponent - 15 + 127;
    uint32_t result = (sign << 31) | (biased_exponent << 23) | (mantissa << 13);

    float converted;
    memcpy(&converted, &result, sizeof(float));
    return converted;
}


int halfFloatsWithMipMaps(int nmipmaps)
{
    int ss = 0;
    for(int i = 0; i <= nmipmaps; i++){

        ss+=pow(2,i)*pow(2,i)*pow(2,i);
    }

    ss*=nmipmaps;

    return ss;
}

int parseVol(const char* fileName, int* width, int* height, int* depth, float*& data)
{
    int errorStatus = 0;
    char* volumeFile = getCurrentDirectoryFile(fileName, 1, &errorStatus);
    if(errorStatus != 0)
    {
        return errorStatus;
    }
    FILE* openFile = nullptr; 
    fopen_s(&openFile, volumeFile, "rb");


    int spaceFileType[5];
    fread(&spaceFileType[0], sizeof(int), 1, openFile);
    fread(&spaceFileType[1], sizeof(int), 1, openFile);
    fread(&spaceFileType[2], sizeof(int), 1, openFile);
    fread(&spaceFileType[3], sizeof(int), 1, openFile);
    fread(&spaceFileType[4], sizeof(int), 1, openFile);

    *width = spaceFileType[0];
    *height = spaceFileType[1];
    *depth = spaceFileType[2];
    
    int halfFloatsNumbered = halfFloatsWithMipMaps(4);

    //float* data = new float[halfFloatsNumbered];
    data = new float[halfFloatsNumbered];
    int ttj = 0;

    uint16_t halfloat = 0;
    while(ttj < halfFloatsNumbered)
    {
        fread(&halfloat, sizeof(uint16_t), 1, openFile);
        data[ttj] = halftoFloat(halfloat);
        ttj++;
    }


    return 1;
}

#endif