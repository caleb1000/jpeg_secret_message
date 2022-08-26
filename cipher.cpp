#include "cipher.h"
const uint32_t F = 0xDEADBEEF;
const uint32_t maskBit = 0x01;
const uint32_t maskByte = 0xFF;

unsigned char *cipher(unsigned char * data, int dataLength, uint32_t initialValue, bool ascii)
{
    unsigned char * result;
    result = (unsigned char*)malloc(dataLength*sizeof(unsigned char));
    uint32_t S = initialValue;

    // loops each character
    for(int x = 0; x < dataLength; x++)
    {

        for(int y = 0; y < 8; y++)
        {
            // loops 8 times to create key
            if(S & maskBit)
            {
                // lowest bit of S is 1
                S = (S >> 1) ^ F;
            }
            else
            {
                // lowest bit of S is 0
                S = (S >> 1);
            }
        }
        // get first key byte of S
        uint32_t lowestByte = S & maskByte;
        // xor key byte with character
        uint32_t outputChar = lowestByte ^ data[x];
        result[x] = outputChar;
        if(ascii){
            printf("%c",outputChar);
        }
        else{
            printf("%02x",outputChar);
        }
    }
    printf("\n");
    return result;

}
