mysql = -L/usr/include/mysql -lmysqlclient -I/usr/include/mysql

OBJECTS = main.o ICU.o patient.o fever.o vegetative_state.o nurse.o

test : $(OBJECTS)
	g++ -o smarthospital -Wall -g -std=c++0x -pthread -lwiringPi $(OBJECTS) 

main.o : common.h main.c
	g++ -c main.c -std=c++11 $(mysql)
ICU.o : common.h ICU.c
	g++ -c ICU.c -std=c++11 $(mysql)
patient.o: common.h patient.c
	g++ -c patient.c -std=c++11 $(mysql)
fever.o : common.h fever.c
	g++ -c fever.c -std=c++11 $(mysql)
vegetative_state.o : common.h vegetative_state.c
	g++ -c vegetative_state.c -std=c++11 $(mysql)
nurse.o: common.h nurse.c
	g++ -c nurse.c -std=c++11 $(mysql)