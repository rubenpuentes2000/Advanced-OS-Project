Project4610: main.o sdisk.o block.o filesys.o shell.o table.o
	g++ -o Project4610 main.o sdisk.o block.o filesys.o shell.o table.o

main.o: main.cpp
	g++ -c main.cpp

sdisk.o: sdisk.cpp
	g++ -c sdisk.cpp

block.o: block.cpp
	g++ -c block.cpp

shell.o: shell.cpp
	g++ -c shell.cpp

filesys.o: filesys.cpp
	g++ -c filesys.cpp

table.o: table.cpp
	g++ -c table.cpp

clean:
	rm *.o Project4610
	rm disk1
	clear
