# Jpeg secret message injector and decoder #
This project deals with adding secret messages to jpegs. Currently the method is very simple; the given input message is encrypted 
using a LFSR algorithm. At the end of a target jpeg file the encrypted message is added. To make decoding this message harder it is split up into
its individual bits, each bit now turned into a byte starting from least to most significant. Simply put, for every encoded byte, there will be
8 bytes added to the end of the jpeg. To get the original value of the encrypted message, after the end of the original jpeg file add every 8 bytes together until the end of the file. Finally, after you get the encrypted message, to decrypt it simply use the LFSR algorithm with an inital state of 0x12345678 and a feedback value of 0xDEADBEEF. These values can be changed, though please note to get the same output as input you must use the same values.

To use this tool all you need to do is first run make. Two executable files will be made that allow for injecting
secret messages into jpegs and decrypting encoded messages in jpegs.

To added a secret message run the following command
` ./jpeg_secret_message <filename>.jpeg <Message> <optional> `
i.e ./jpeg_secret_message resume.jpeg AppleSauce apple.jpeg (creates a jpeg named apple.jpeg with the encrypted message AppleSauce at the end)
Note: The last argument is the output filename and is optional, when not specified the default is secret_message.jpeg

To decode a secret message from a jpeg use the following 
`./decode_secret_message <filename>.jpeg`
i.e ./decode_secret_message secret_message.jpeg (outputs the secret message hidden in the jpeg)

New Feature: Appending the -i flag to the end of your jpeg_secret_message command allows you to encode hidden messages inside the chromoinance (warning limits message size and doesn't work well with color images)
  
  ## Future Goals ##
  -Add hidden messages within the start of frame color components (i.e luminance (Y), chrominance-blue (Cb), and chrominance-red (Cr) )
  
  -Expand to other image formats such as png or gif
  
  -Explore different cryptographic methods and algorithms 
  
  ![image](https://user-images.githubusercontent.com/30327564/186339641-9ea0d54c-0d2d-4aa6-a819-5833ad68ebac.png)


