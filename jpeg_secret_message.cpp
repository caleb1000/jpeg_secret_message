#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <iterator>

const uint32_t F = 0xDEADBEEF;
const uint32_t maskBit = 0x01;
const uint32_t maskByte = 0xFF;

unsigned char* cipher(unsigned char* data, int dataLength, uint32_t initialValue)
{

    unsigned char* result;
    result = (unsigned char*)malloc(dataLength*sizeof(unsigned char));
    uint32_t S = initialValue;

    // loops for each character that will be encrypted
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
        //print out new char
        //printf("%c",outputChar);
        printf("%02x",outputChar);

    }
    return result;
}


int main (int argc, char *argv[]){

    if(argc < 3){
        std::cout<<"Missing Jpeg file as argument or Input string"<<std::endl;
        return -1;
    }
    std::string message = argv[2];
    std::ifstream jpeg;
    jpeg.open(argv[1],std::ios::binary);
    std::vector<unsigned char> result;
    int message_len = message.size();
    printf("Encrypted message in Hex: ");
    unsigned char* encrypted_message = cipher((unsigned char*)argv[2], message_len, 0x12345678);
    printf("\n");
    unsigned char curr_char;
    if(jpeg.is_open()){
        while(jpeg){
            curr_char = (unsigned char)jpeg.get();
            result.push_back(curr_char);
        }
    }
    else{
        std::cout<<"Error opening jpeg"<<std::endl;
    }
    result.pop_back();//remove 0xFF end char
    int string_index = 0;
    int bit = 0; //current bit being encoded 0-7
    uint8_t test =0;
    while(string_index < message_len){
        result.push_back((1<<bit) & encrypted_message[string_index]);
        test += ((1<<bit) & encrypted_message[string_index]);
        if(bit == 7){
            string_index++;
            test = 0;
            bit = 0;
        }
        else{
            bit++;
        }
    }

    //cipher(encrypted_message, message_len, 0x12345678);

    std::ofstream file("secret_message.jpeg", std::ios::out|std::ios::binary);
    std::copy(result.cbegin(), result.cend(), std::ostream_iterator<unsigned char>(file));
    file.flush();
    file.close();
    return 0;

}
