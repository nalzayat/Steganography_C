/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
 * Stego.c: A program for manipulating images                           *
 *++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

#include <string.h>
#include "image.h"
#include <stdio.h>
// Prototype: setlsbs()
void setlsbs(unsigned char *p, unsigned char b0);

int main(int argc, char *argv[])
{  
  int i, j, k, cover_bits, bits;
  struct Buffer b = {NULL, 0, 0};
  struct Image img = {0, NULL, NULL, NULL, NULL, 0, 0};
  byte b0;
   
  if (argc != 4) 
    {
      printf("\n%s <cover_file> <stego_file> <file_to_hide> \n", argv[0]);
      exit(1);
    }
  ReadImage(argv[1],&img);       // read image file into the image buffer img
                                 // the image is an array of unsigned chars (bytes) of NofR rows
                                 // NofC columns, it should be accessed using provided macros
  ReadBinaryFile(argv[3],&b);    // Read binary data
 

  // hidden information 
  // first four bytes is the size of the hidden file
  // next 4 bytes is the G number (4 bits per digit)
  if (!GetColor)
    cover_bits = img.NofC*img.NofR;
  else 
    cover_bits = 3*img.NofC*img.NofR;    
  bits = (8 + b.size)*8;
  if (bits > cover_bits)
    {
      printf("Cover file is not large enough %d (bits) > %d (cover_bits)\n",bits,cover_bits);
      exit(1);
    }
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
 // EMBEDING B.SIZE 
 // Embed four size bytes for the Buffer's size field
 // Variables bytes, bit, bit_place all set to zero 
 int img_size = b.size; // First assign b.size to variable img_size, this will aid in shifting later on 
 // Three variables 
 int bit_counter = 0;
 int bit_counter2 = 0;
 int group_bytes = 0;
 unsigned char byte_array[8]; // An array of unsigned characters, size is 8 
 // For loop four times to get eight bytes at a time totaling 32 bytes
 for (group_bytes; group_bytes  < 4; group_bytes++){ // Less than 4 because 0,1,2,3
	int array_looper = 0; // Resets the looper so that it is does not access out of bounds index of array
	// For loops to place the 8 bytes into the array and updates the counter 
	for (array_looper; array_looper < 8 ; array_looper++){       
                byte_array[array_looper] = GetGray(bit_counter); // Assigns the byte to the  corresponding array index 
		bit_counter++; // Increments to keep track of where it is 
        }
	// Calls setlsbs giving it the array of 8 bytes and the new size of the image (if shifted)
	setlsbs(byte_array,img_size);
	array_looper = 0; // Resests the looper so that it does not access out of bounds index of array 
	for(array_looper; array_looper < 8; array_looper++){
		SetGray(bit_counter2, byte_array[array_looper]);
		bit_counter2++;
	}
	// Shifts the image by 8  bits to the right
	img_size = img_size >> 8;	
} 

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
 // EMBEDING G NUMBER
 // Embed the eight digits of your G# using 4 bits per digit
 // Creates an array of unsigned char
 // Contains my G#
 unsigned char g_number[8];
 g_number[0] = 0; // Index 0 is my first number
 g_number[1] = 0; // Index 1 is my second number
 g_number[2] = 8; // Index 2 is my third number
 g_number[3] = 9; // So on...
 g_number[4] = 1;
 g_number[5] = 7;
 g_number[6] = 3;
 g_number[7] = 2;
 
 // Keeps track of index of my g_number array
 int index_counter = 0;
 // Reinitializes the start 
 group_bytes = 0;
 // For loop four times to get eight bytes at a time totaling 32 bytes
 for(group_bytes; group_bytes < 4; group_bytes++){
	int array_looper = 0; // Resets the looper so it does not exceed the array length 
	for(array_looper; array_looper < 8; array_looper++){ // For loops to place the 8 bytes into the array and updates the bit counter 
		byte_array[array_looper] = GetGray(bit_counter);
		bit_counter++;
	}
	// Grabs two indexes at a time and shift the first to the left four and then or it to make it one byte
	unsigned char first_num_shift = g_number[index_counter] << 4;
	//unsigned char second_num_or = ((first_num_shift) | (g_number[(index_counter+1)])) + 1;
	unsigned char second_num_or = ((first_num_shift) | (g_number[(index_counter+1)]));
	// Debugging //
	//printf("first is %d\n", first_num_shift);
	//printf("second is %d\n", second_num_or);
	// Update the index counter to get the next two
	index_counter += 2;
	// Calls setlsbs and send in the array of the g number as well as the two numbers that are now "ored" together
	setlsbs(g_number,second_num_or);
	array_looper = 0;
	for(array_looper; array_looper < 8; array_looper++){
		SetGray(bit_counter2, g_number[array_looper]);
		bit_counter2++;
	}
 } 
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// EMBED PAYLOAD
// here you embed information into the image one byte at the time
// note that you should change only the least significant bits of the image
 for (i=0; i<b.size; i++){ // Given to us 
	unsigned char byte_array2[8];// Creates an array of size 8
	int array_looper = 0; // Resets the looper 
	for (array_looper; array_looper < 8; array_looper++){ // For loops eight times to place the proper byte from the img into corresponding array index
		byte_array2[array_looper] = GetGray(bit_counter);
		bit_counter++;
	}
	setlsbs(byte_array2,GetByte(i)); // Calls function and sends in the array of unsigned characters as well as the byte  
	array_looper = 0;
	for(array_looper; array_looper < 8; array_looper++){
        	SetGray(bit_counter2, byte_array2[array_looper]);
                bit_counter2++;
        }
}

  // Given to us
  WriteImage(argv[2],img);  // output stego file (cover_file + file_to_hide)

 //FREE STUFF

 //Debugging code:  Print to check proper size 
 /* printf("\n"); 
  printf("B.size is: %d", b.size);
  printf("\n");
*/
 return 0;
}

// Function setlsbs: Taken from pervious Lab 11
void setlsbs(unsigned char *p, unsigned char b0){
          int i = 0;
          while(i < 8){
                  unsigned char temporary_unsigned = 1<<i;
                  temporary_unsigned = temporary_unsigned & b0;
                  temporary_unsigned = temporary_unsigned >> i;
  
                  unsigned char array_unsigned = p[i]&1;
                  if(temporary_unsigned != array_unsigned){
                          p[i] ^= 1;
                  }
                  i++;
          }
  }
