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


int main (int argc, char *argv[]){

    //arg[1] = input jpeg, arg[2] = message, arg[3] = optional output filename

    if(argc < 3 || argc > 4){
        std::cout<<"Too many arugments, missing Jpeg file or missing message string"<<std::endl;
        return -1;
    }

    std::string filename;
    if(argc == 4){
        filename = argv[3];
    }
    else{
        filename = "secret_message.jpeg";
    }

    std::string message = argv[2];
    int message_len = message.size();

    printf("Encrypted message in Hex: ");
    unsigned char* encrypted_message = cipher((unsigned char*)argv[2], message_len, 0x12345678, false);

    std::vector<unsigned char> result; //vector to hold raw jpeg data
    result = jpeg_to_vec(argv[1]);
    if(result.empty()){
        std::cout<<"Jpeg vector returned empty, exiting program"<<std::endl;
        return -1;
    }
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

    std::ofstream file(filename, std::ios::out|std::ios::binary);
    std::copy(result.cbegin(), result.cend(), std::ostream_iterator<unsigned char>(file));
    file.flush();
    file.close();
    return 0;

}
