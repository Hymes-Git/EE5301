CC=g++
#CFLAGS=-Wall -std=c++11
CFLAGS= -std=c++17

run: hymes019_PA0_B
	./hymes019_PA0_B

main: hymes019_PA0_B.cpp
	$(CC) $(CFLAGS) hymes019_PA0_B.cpp -o hymes019_PA0

clean:
	rm -f hymes019_PA0_B

test: hymes019_PA0_B
	./hymes019_PA0_B NLDM_lib_max2Inp b18_1.isc 65303

.PHONY: hymes019_PA0_B.cpp
