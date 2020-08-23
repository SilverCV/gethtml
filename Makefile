CC = g++
main : main.o Spider.o 
	$(CC) main.o Spider.o -o main
Spider.o : Spider.h Spider.cpp
	$(CC) -c Spider.cpp Spider.h

main.o : main.cpp
	$(CC) -c main.cpp


.PHONY: clean

clean:
	rm -rf *.o main