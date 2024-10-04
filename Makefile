all:	share	encode	decode

share:		share.c
	gcc -c share.c -o share.o

encode:		encode.c
	gcc -o encode encode.c share.c

decode:		decode.c
	gcc -o decode decode.c

clean:
	rm -rf *.o encode decode share
