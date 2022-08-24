#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <iterator>
#include "cipher.h"

int main (int argc, char *argv[]){

    if(argc < 2){
        std::cout<<"Missing Jpeg file as argument"<<std::endl;
        return -1;
    }
    std::ifstream jpeg;
    jpeg.open(argv[1],std::ios::binary);
    std::vector<unsigned char> result;
    unsigned char curr_char;
    if(jpeg.is_open()){
        while(jpeg){
            curr_char = (unsigned char)jpeg.get();
            result.push_back(curr_char);
        }
    }
    else{
        std::cout<<"Error opening jpeg"<<std::endl;
        return -1;
    }


   int x=0;
   bool header_found = false;
   std::vector<uint8_t> encrypted_message_vec;
   while(x<result.size()-3){
        if((result.at(x) == 0xFF) && (result.at(x+1) == 0xD9)){
            header_found = true;
            x=x+2;//when we identify the end header, we know the rest are going to be 8 bytes chuncks
            while(x<result.size()-1){
                uint8_t temp = 0;
                for(int y=0; y<8; y++){
                    temp += (uint8_t)result.at(x);
                    x++;
                }
                encrypted_message_vec.push_back(temp);
            }
        }
        else{
            x++;
        }
    }

    if(!header_found){
        std::cout<<"Secret header not found in jpeg"<<std::endl;
        return -1;
    }
    std::cout<<"Encrypted message in Hex: "<<std::endl;
    for(uint8_t i: encrypted_message_vec){
        printf("%02x",i);
    }
    printf("\n");
    unsigned char* encrypted_message;
    encrypted_message = (unsigned char*)malloc(encrypted_message_vec.size()*sizeof(unsigned char));
    unsigned char* temp = encrypted_message;
    for(int z=0; z<encrypted_message_vec.size(); z++){
        *temp = encrypted_message_vec.at(z);
        temp++;
    }
    std::cout<<"Decrypted message in ascii: "<<std::endl;
    cipher(encrypted_message, encrypted_message_vec.size(), 0x12345678, true);
    free(encrypted_message);

    jpeg.close();
    return 0;

}

