#include <string.h>
#include "image.h"
#include <stdio.h>
// Prototypes: getlsbs()
unsigned char getlsbs(unsigned char *p);

int main(int argc, char *argv[]){
	// Taken from Stego:
   	int i = 0;
	struct Buffer b = {NULL, 0, 0};
  	struct Image img = {0, NULL, NULL, NULL, NULL, 0, 0};
	// Checks if the proper number of arguments were given
  	if (argc != 3){ 
      	 	printf("Not enough arguments. Quiting now!");
       		exit(1);
 	   }

	FILE *outfile = fopen(argv[2], "wb");
	fclose(outfile);

	ReadImage(argv[1],&img); // Read image file into the image buffer img
	ReadBinaryFile(argv[2], &b);
	// Variable img_size will contain the image counter 
	unsigned char img_size = 0;
	// Initializes to zero in order to iterate four times grabbing 8 bytes at a time 
	
/////////////////////////////////////////////////////////////////////////////////////
// Extracting b.size:
	int group_bytes = 0;
	int bit_counter = 0;
	// Creates an array of size 8
        unsigned char byte_array[8];
	for(group_bytes = 0; group_bytes < 4; group_bytes++){
		// Reinitializes to zero in order to always go from index zero to eight 
		int array_looper = 0;
		for(array_looper; array_looper < 8; array_looper++){
			byte_array[array_looper] = GetGray(bit_counter);
			bit_counter++;
		}
		img_size = getlsbs(byte_array)+img_size;
	}
 	fprintf(stdout,"\n");
	fprintf(stdout,"The size of b.size is: %d\n",img_size);
	fprintf(stdout, "\n");
	b.size = img_size;
/////////////////////////////////////////////////////////////////////////////////////
// Extracting G number:
	printf("The G number extracted is: G");
	// Creates variable to store the g number that is being extracted
 	unsigned char num1 = 0;
	unsigned char num2 = 0;
	unsigned char g_number = 0;
	group_bytes = 0;
	// Creates an array of size eight to store the g number
	unsigned char g_number_array[8]; 
	for(group_bytes = 0; group_bytes < 4; group_bytes++){
		int array_looper = 0;
		for(array_looper; array_looper < 8; array_looper++){
			g_number_array[array_looper] = GetGray(bit_counter);
			bit_counter++;
		}
		// Get the number and shift it right four to get left then right to get left half
		num1 =  getlsbs(g_number_array)>>4;
		// Then get the same number but this time shift right to get the left half
		//num2 = (getlsbs(g_number_array)^(num1<<4))+1;
		if (group_bytes == 0){	 
			num2 = (getlsbs(g_number_array)^num1<<4);	
			}
		else{
			num2 = (getlsbs(g_number_array)^num1<<4)+1;
			}
		fprintf(stdout,"%d%d",num1,num2);
	
	}
	// New line
	fprintf(stdout,"\n");
	fprintf(stdout,"\n");
/////////////////////////////////////////////////////////////////////////////////////
// Extracting Payload:	
       
	int counter1 = 0; // Two counters to keep track
	int counter2 = 0;
	char extraction[img_size]; // Array of character
	FILE *in = NULL; // Opens the file for binary writing
	in = fopen(argv[2], "wb");
	if(in == NULL){
		fprintf(stdout, "Error opening the file. Will quit now!");
		exit(1); // If the file could not be opened it exits properly
	}
	for(counter1; counter1 < img_size; counter1++){ // Loops from zero up until the image size that was extracted previously
		unsigned char array_byte[8];// An array of size eight
		counter2 = 0; // Resets the counter for next iterations
		for(counter2; counter2 < 8; counter2++){
			array_byte[counter2] = GetGray(bit_counter);
			bit_counter++;
		} 
		extraction[counter1] = getlsbs(array_byte);
	}
	fwrite(extraction, sizeof(img_size),(sizeof(extraction))/(sizeof(extraction[0])), in );
	fclose(in);	// Had to write own because the one given to use did not work 
			// Make sure to always close the file
	fprintf(stdout,"Writing file '%s'\n", argv[2]);
	//WriteBinaryFile(argv[2],b); // Does not work!!!		
	// Free stuff to prevent memory leak
/////////////////////////////////////////////////////////////////////////////////////
// Debugging: Code	
	//printf("The size of b.size is: %d\n",img_size);
	//printf("The G Number is: G%u\n",g_number);
}
// Method taken from previous Lab11 
unsigned char getlsbs(unsigned char *p){
	unsigned char temporary_unsigned = 0;
        unsigned char lsb = 0;
        int i = 7;
        while(i >= 0){
        	lsb = p[i]&1;
                temporary_unsigned = temporary_unsigned << 1;
                temporary_unsigned = temporary_unsigned | lsb;
                i--;
	}
 	return temporary_unsigned;
}	
