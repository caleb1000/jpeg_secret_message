#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <iterator>
#include "cipher.h"


std::vector<unsigned char> jpeg_to_vec(char* jpeg_name){
    std::ifstream jpeg;
    unsigned char curr_char;
    std::vector<unsigned char> result;
    std::string jpeg_name_string = jpeg_name;
    jpeg.open(jpeg_name_string,std::ios::binary);
    if(jpeg.is_open()){
        while(jpeg){
            curr_char = (unsigned char)jpeg.get();
            result.push_back(curr_char);
        }
    }
    else{
        std::cout<<"Error opening jpeg"<<std::endl;
    }
    result.pop_back();//remove 0xFF aka end of file char
    return result;
}

void inject_in_chrom(std::vector<unsigned char>& result, unsigned char* encrypted_message, int message_len){

    bool first_header_found = false;
    for(int x = 0; x<result.size(); x++){
        if(result.at(x) == 0xFF && result.at(x+1) == 0xDB){
            if(first_header_found){
                uint16_t size;
                uint16_t hi = (uint8_t)result.at(x+2) << 8;
                uint16_t low = (uint8_t)result.at(x+3);
                size = hi + low;
                if(message_len*8 > (int)size-5){
                    std::cout<<"Message too large to be added in jpeg, max size of message "<< ((int)size-5)/8 <<std::endl;
                    return;
                }
                int bit = 0;
                int string_index = 0;
                for(int y = x+5; string_index<message_len; y++){
                     result.at(y) = (result.at(y) & 0xFE) | (0x1 & (encrypted_message[string_index] >> bit ));
                     //printf("%d",result.at(y) & 0x1);
                     if(bit == 7){
                         string_index++;
                         bit = 0;
                     }
                     else{
                         bit++;
                     }
                }

                break;
            }
            else{
                first_header_found = true;
            }
        }
        else{
        }
        //find second ffdb header (next 2 bytes size, skip third byte)
    }


return;

}

int main (int argc, char *argv[]){

    //arg[1] = input jpeg, arg[2] = message, arg[3] = optional output filename, arg[4] = injection type (i.e end of file or inside file)

    if(argc < 3 || argc > 5){
        std::cout<<"Too many arugments, missing Jpeg file or missing message string"<<std::endl;
        return -1;
    }

    std::string filename;
    bool inject_at_eof = true; //by default we inject the message at the end of the file simply, but now we are testing allow file to inject inside of jpeg
    if(argc >= 4){
        if(argc == 4){
            std::string arg3(argv[3]);
            if(arg3 == "-i"){
                inject_at_eof = false;
                filename = "secret_message.jpeg";
            }
            else{
                filename = argv[3];
            }
        }
        if(argc == 5){
            filename = argv[3];
            std::string arg4(argv[4]);
            if(arg4 == "-i"){
                inject_at_eof = false;
            }
        }
    }
    else{
        filename = "secret_message.jpeg";
    }

    if(inject_at_eof){
        std::cout<<"Injected message at end of file"<<std::endl;
    }
    else{
        std::cout<<"Injected message in chromo of jpeg: WARNING THIS LIMITS THE MESSAGE SIZE"<<std::endl;
    }

    std::string message = argv[2];
    int message_len = message.size();

    printf("Encrypted message in Hex: ");
    unsigned char* encrypted_message = cipher((unsigned char*)argv[2], message_len, 0x12345678, false);

    std::vector<unsigned char> result; //vector to hold raw jpeg data
    result = jpeg_to_vec(argv[1]);//pass file name of jpeg you want to get data from
    if(result.empty()){
        std::cout<<"Jpeg vector returned empty, exiting program"<<std::endl;
        return -1;
    }

    if(!inject_at_eof){
        inject_in_chrom(result, encrypted_message, message_len);
    }

    else{
        int bit = 0; //current bit being encoded 0-7
        int string_index = 0;
        while(string_index < message_len){
            result.push_back((1<<bit) & encrypted_message[string_index]);
            if(bit == 7){
                string_index++;
                bit = 0;
            }
            else{
                bit++;
            }
        }
    }

    std::ofstream file(filename, std::ios::out|std::ios::binary);
    std::copy(result.cbegin(), result.cend(), std::ostream_iterator<unsigned char>(file));
    file.flush();
    file.close();
    return 0;

}
