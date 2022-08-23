all: decode_secret_message jpeg_secret_message

decode_secret_message: decode_secret_message.cpp
	g++ decode_secret_message.cpp -o decode_secret_message
jpeg_secret_message: jpeg_secret_message.cpp
	g++ jpeg_secret_message.cpp -o jpeg_secret_message
