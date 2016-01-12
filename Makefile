FLAGS=-Wall -g -O0
Project3_Nalzayat_208:
	gcc image.c -c
	gcc Stego.c -c
	gcc Stego.o image.o -o Stego
	gcc StegoExtract.c -c
	gcc StegoExtract.o image.o -o StegoExtract
clean:
	rm -f Stego
	rm -f Stego.o
	rm -f StegoExtract.o
	rm -f image.o 
	rm -f StegoExtract
	rm file.txt
	rm out.txt
