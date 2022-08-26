#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <iterator>
#include "cipher.h"

std::vector<uint8_t> parse_from_chrom(std::vector<unsigned char> result){
    //we don't know full message size but cipher should still be able to work
    bool first_header_found = false;
    std::vector<uint8_t> message;
    for(int x = 0; x<result.size(); x++){
        if(result.at(x) == 0xFF && result.at(x+1) == 0xDB){
            if(first_header_found){
                uint16_t size;
                uint16_t hi = (uint8_t)result.at(x+2) << 8;
                uint16_t low = (uint8_t)result.at(x+3);
                size = hi + low;
                int bit = 0;
                uint8_t temp = 0;
                for(int y = 5; y<(int)size; y++){
                     //printf("%d",result.at(y+x) & 0x01);
                     if(bit == 7){
                         temp += (result.at(y+x) & 0x1) << bit;
                         message.push_back(temp);
                         temp = 0;
                         bit = 0;
                     }
                     else{
                         temp += (result.at(y+x) & 0x1) << bit;
                         bit++;
                     }
                }
                printf("\n");
                break;
            }
            else{
                first_header_found = true;
            }
        }
    }

    return message;
}


int main (int argc, char *argv[]){

    if(argc < 2 || argc > 3 ){
        std::cout<<"Invalid arguments"<<std::endl;
        return -1;
    }
    bool inject_at_eof = true;//means we search for message at end if true
    if(argc == 3){
        std::string arg2(argv[2]);
            if(arg2 == "-i"){
                std::cout<<"Parsing secret message within chromo of the jpeg"<<std::endl;
                inject_at_eof = false;
            }
            else{
                std::cout<<"Parsing secret message at end of the jpeg"<<std::endl;
            }
    }
    else{
        std::cout<<"Parsing secret message at end of the jpeg"<<std::endl;
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
    printf("jpeg size %ld\n",result.size());

    if(inject_at_eof){

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
	        std::cout<<"Secret message not found in jpeg"<<std::endl;
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
    else{
        std::vector<uint8_t> encrypted_message_vec = parse_from_chrom(result);
        std::cout<<"Encrypted message in Hex: ";
        unsigned char* encrypted_message = (unsigned char*)malloc(encrypted_message_vec.size()*sizeof(unsigned char));
        unsigned char* temp = encrypted_message;
        for(int x =0; x< encrypted_message_vec.size(); x++){
            printf("%02x",encrypted_message_vec.at(x));
            *temp = (unsigned char)encrypted_message_vec.at(x);
            temp++;
        }
        printf("\n");
        std::cout<<"Decrypted message in ascii: "<<std::endl;
        cipher(encrypted_message, encrypted_message_vec.size(), 0x12345678, true);
        free(encrypted_message);
        jpeg.close();
        return 0;
    }

return 0;
}

