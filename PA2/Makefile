#CFLAGS=-O3
CFLAGS=-g

all: suraj_parser.o main.o
	g++ $(CFLAGS) -o suraj_parser suraj_parser.o main.o

suraj_parser.o: suraj_parser.cpp
	g++ $(CFLAGS) -c suraj_parser.cpp

main.o: main.cpp suraj_parser.h
	g++ $(CFLAGS) -c main.cpp

clean: 
	del *.o suraj_parser.exe


