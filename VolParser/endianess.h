#ifndef ENDIAN_MODE_H
#define ENDIAN_MODE_H 

#include <stdint.h>
#include <stdio.h>

uint64_t LittleEndianSF(uint64_t BigEndian)
{

    uint64_t Little = 
    (BigEndian & 0xFF00000000000000) >> 56 | 
    (BigEndian & 0x00ff000000000000) >> 40 | 
    (BigEndian & 0x0000ff0000000000) >> 24;

    return Little;

}

int64_t LittleEndianISF(int64_t BigEndian)
{

    int64_t Little = 
    (BigEndian & 0xFF00000000000000) >> 56 | 
    (BigEndian & 0x00ff000000000000) >> 40 | 
    (BigEndian & 0x0000ff0000000000) >> 24;

    return Little;

}

uint32_t LittleEndianTT(uint32_t BigEndian)
{
    uint32_t Little = BigEndian;
    uint32_t Buffer = BigEndian;
    Little = (Buffer & 0xff000000) >> 24 | (Buffer & 0x00ff0000) >> 8 | (Buffer & 0x0000ff00) << 8 | (Buffer & 0x000000ff) << 24;

    return Little;
}


int16_t ReadBigIST(FILE* file)
{
    int16_t Deposit = 0;
    fread(&Deposit, sizeof(int16_t), 1, file);
    return Deposit;
}

int16_t LittleEndianIST(int16_t BigEndian)
{
    int16_t Little = BigEndian;
    int16_t Buffer = BigEndian;
    Little = (Buffer & 0xff00) >> 8 | (Buffer & 0x00ff) << 8;
    return Little;
}

int32_t LittleEndianITT(uint32_t BigEndian)
{
    int32_t Little = BigEndian;
    int32_t Buffer = BigEndian;
    Little = (Buffer & 0xff000000) >> 24 | (Buffer & 0x00ff0000) >> 8 | (Buffer & 0x0000ff00) << 8 | (Buffer & 0x000000ff) << 24;

    return Little;
}


uint16_t LittleEndianST(uint16_t BigEndian)
{
    uint16_t Little = BigEndian;
    uint16_t Buffer = BigEndian;
    Little = (Buffer & 0xff00) >> 8 | (Buffer & 0x00ff) << 8;
    return Little;
}

float ReadF(FILE* file)
{
    float buffer = 0;
    fread(&buffer, 4, 1, file);
    return buffer;
}

uint32_t ReadSF(FILE* file)
{
    int errcode = 0;
    uint64_t buffer = 0;
    fread(&buffer, 8, 1, file);
    return LittleEndianSF(buffer);
}

uint32_t ReadISF(FILE* file)
{
    int errcode = 0;
    int64_t buffer = 0;
    fread(&buffer, 8, 1, file);
    return LittleEndianISF(buffer);
}

uint32_t ReadTT(FILE* file)
{
    int errcode = 0;
    uint32_t buffer = 0;
    fread(&buffer, 4, 1, file);
    return LittleEndianTT(buffer);
}

int32_t ReadITT(FILE* file)
{
    int errcode = 0;
    int32_t buffer = 0;
    fread(&buffer, 4, 1, file);
    return LittleEndianITT(buffer);
}

uint32_t checksum(uint32_t * tablehead, uint32_t tablelength)
{

    uint32_t* ptrend = tablehead+(tablelength/sizeof(uint32_t));
    uint32_t sum = 0L;
    while(tablehead < ptrend)
    {
        sum += *tablehead++;
    }

    return sum;
}

uint16_t ReadST(FILE* file)
{
    uint16_t buffer = 0;
    fread(&buffer, sizeof(unsigned char), 2, file);
    return LittleEndianST(buffer);
}

int16_t ReadIST(FILE* file)
{
    int16_t buffer = 0;
    fread(&buffer, sizeof(unsigned char), 2, file);
    int16_t nnbuffer = ((buffer & 0x00FF) << 8) | ((buffer & 0xFF00) >> 8);

    return LittleEndianIST(buffer);
}
fpos_t GetPos(FILE* file)
{
    fpos_t NewPos = 0;
    fgetpos(file, &NewPos);
    return NewPos;
}
int32_t ReadITF(FILE* file)
{
    int32_t buffer = 0;
    fread(&buffer, sizeof(unsigned char), 3, file);

    buffer = (buffer & 0x00FF0000) >> 16 | (buffer & 0x0000FF00) | (buffer & 0x000000FF) << 16;
    buffer = buffer;
    return buffer;
}

uint32_t ReadTF(FILE* file)
{
    uint32_t buffer = 0;
    fread(&buffer, sizeof(unsigned char), 3, file);

    buffer = (buffer & 0x00FF0000) >> 16 | (buffer & 0x0000FF00) | (buffer & 0x000000FF) << 16;
    buffer = buffer;
    return buffer;
}

uint8_t ReadE(FILE* file)
{
    uint8_t buffer = 0;
    fread(&buffer, 1,1, file);
    return buffer;

}

int8_t ReadIE(FILE* file)
{
    int8_t buffer = 0;
    fread(&buffer, 1,1, file);
    return buffer;

}

#endif