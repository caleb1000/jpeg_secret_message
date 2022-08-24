#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <iterator>
#include "cipher.h"

int main (int argc, char *argv[]){

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

    unsigned char curr_char;
    std::vector<unsigned char> result;
    std::ifstream jpeg;
    jpeg.open(argv[1],std::ios::binary);
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

    result.pop_back();//remove 0xFF end char
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
