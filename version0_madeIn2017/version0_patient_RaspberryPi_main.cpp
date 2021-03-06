/*
 Pi_Serial_test.cpp - SerialProtocol library - demo
 Copyright (c) 2014 NicoHood.  All right reserved.
 Program to test serial communication
 
 Compile with:
 sudo gcc -o Pi_Serial_Test.o Pi_Serial_Test.cpp -lwiringPi -DRaspberryPi -pedantic -Wall
 sudo ./Pi_Serial_Test.o
 */
 
// just that the Arduino IDE doesnt compile these files.
 
//include system librarys
#include <stdio.h> //for printf
#include <stdint.h> //uint8_t definitions
#include <stdlib.h> //for exit(int);
#include <string.h> //for errno
#include <errno.h> //error output
#include <pthread.h>
#include <unistd.h>
#include <mysql.h>
#include <fcntl.h>
//wiring Pi
#include <wiringPi.h>
#include <wiringSerial.h>
//for push alarm
#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/wait.h>
#include <signal.h>
#include <netinet/in.h>
#include <assert.h>
#include <arpa/inet.h>

#define DB_HOST "220.70.2.32"
#define DB_USER "doctor"
#define DB_PASS "doctor"
#define DB_NAME "Hospital"
#define DANGEROUS_TEMP 30.0
#define DANGEROUS_BEAT 130

#define GOOD	1
#define SOSO	2
#define BAD		3
#define BUTTON	5	

int patient_state=GOOD;
char *patient_name = "patient1";

// Find Serial device on Raspberry with ~ls /dev/tty*
// ARDUINO_UNO "/dev/ttyACM0"
// FTDI_PROGRAMMER "/dev/ttyUSB0"
// HARDWARE_UART "/dev/ttyAMA0"
char device[]= "/dev/ttyACM1";
// filedescriptor
int fd;
unsigned long baud = 115200;
unsigned long Atime=0;

//sensing data
int heart1, heart2, heart3, sound, checkSound;
double weight, temp;
bool w_alarm;
int nurse;
 
//prototypes
void setup(void);
 
void setup(){
  printf("%s \n", "Raspberry Startup!");
  fflush(stdout);
 
  //get filedescriptor
  if ((fd = serialOpen (device, baud)) < 0){
    fprintf (stderr, "Unable to open serial device: %s\n", strerror (errno)) ;
    exit(1); //error
  }
 
  //setup GPIO in wiringPi mode
  if (wiringPiSetup () == -1){
    fprintf (stdout, "Unable to start wiringPi: %s\n", strerror (errno)) ;
    exit(1); //error
  }
}
 
   

void *sensing(void *arg){
	char buf[200], flushBuf[200]={0, };
	char *ptr;
	char bufStr[20];
	int i = 0;

	setup();
	pinMode(BUTTON, INPUT);
	while(1){
		//button
		if(digitalRead(BUTTON) == LOW){
			patient_state = SOSO;
			printf("Thank you nurse!!\n");
			delay(100);
		}

	// Pong every 3 seconds
	  if(millis()-Atime>=3000){
	    serialPuts (fd, "Pong!\n");
	    // you can also write data from 0-255
	    // 65 is in ASCII 'A'
	    serialPutchar (fd, 65);
	    Atime=millis();
	  }
 	 // read signal
 	 if(serialDataAvail (fd)){
 	   //char newChar = serialGetchar (fd);
 	   //printf("%c", newChar);
	
		buf[i++] = serialGetchar(fd);
		if(i>60){
			ptr = strchr(buf, '!');
			strncpy(bufStr,ptr+1,3);
			if(bufStr[2]==',')
				bufStr[2]='\n';
			else bufStr[3]='\n';
			heart1 = atoi(bufStr);

			ptr = strchr(buf, '@');
			strncpy(bufStr,ptr+2,3);
			bufStr[3]='\n';
			weight = atof(bufStr);

			ptr = strchr(buf, '#');
			strncpy(bufStr,ptr+1,2);
			bufStr[2]='\n';
			sound = atoi(bufStr);
	
			ptr = strchr(buf, '%');
			strncpy(bufStr,ptr+1,5);
			bufStr[5]='\n';
			temp = atof(bufStr);
			
			printf("heart : %d , weight : %lf, sound : %d, temp : %lf\n",heart1, weight, sound, temp);
				
			strcpy(buf, flushBuf);
			i = 0;
		}
	    //fgets(bufStr, sizof(bufStr),stdout);
  	  fflush(stdout);
	  }
	}	
}
 
MYSQL* mysql_connection_setup() {
 
    MYSQL *connection = mysql_init(NULL);
 
    if(!mysql_real_connect(connection, DB_HOST, DB_USER, DB_PASS, DB_NAME, 0, NULL, 0)) {
        printf("Connection error : %s\n", mysql_error(connection));
        exit(1);
    }
    return connection;
}

MYSQL_RES* mysql_perform_query(MYSQL *connection, char *sql_query) {
 
    if(mysql_query(connection, sql_query)) {
 
        printf("MYSQL query error : %s\n", mysql_error(connection));
        exit(1);
 
    }
    return mysql_use_result(connection);
}



// main function for normal c++ programs on Raspberry
int main(void){
	MYSQL *conn;
    MYSQL_RES *res;
	MYSQL_RES *res2;
	MYSQL_RES *res3;
    MYSQL_ROW row;
	char query[256];
	time_t now;
	int cnt=0;
	bool alarm = true;
	int flag;

	conn = mysql_connection_setup();

	res = mysql_perform_query(conn, "show tables");
	while((row = mysql_fetch_row(res)) != NULL)
        printf("%s\n", row[0]);


	pthread_t sensor_thread;
	if (pthread_create(&sensor_thread, NULL, &sensing, NULL))
		printf("thread create failed!!\n");

	while(1){
		sleep(2);
		time(&now);
		if(weight>25.0) w_alarm=0;
		else w_alarm = 1;
		sprintf(query,"insert into %s (time, heartbeat,temperature,w_alarm, weight) value (%d,%d,%lf,%d,%lf);",patient_name, now,heart1,temp,w_alarm,weight);
		printf("%s\n",query);
		res = mysql_perform_query(conn, query);
		
		sprintf(query,"update Alarm set w_alarm=%d where patient_name='%s'",(int)weight, patient_name);
		//printf("%s\n",query);
		res = mysql_perform_query(conn, query);

  //		while((row = mysql_fetch_row(res)) != NULL)
		//// 
		sprintf(query, "select state from Alarm where patient_name='%s'", patient_name);
		res3 = mysql_perform_query(conn, query);
		row=mysql_fetch_row(res3);
		//printf("%s\n", query);
		nurse=atoi(row[0]);

//		if(nurse == SOSO){
//				printf("Thank you nurse!!\n");
//				patient_state=SOSO;
//		}
		mysql_free_result(res3);
		cnt=0;
		sprintf(query, "select * from %s order by time DESC limit 3", patient_name);
		res2 = mysql_perform_query(conn, query);
		
//		printf("%s\n", query);

		while((row = mysql_fetch_row(res2)) != NULL){
				if(atof(row[2])>DANGEROUS_TEMP)///////////temp, heartbeat
					cnt++;
				else if(atoi(row[1])>DANGEROUS_BEAT)
					cnt++;
		}
		if(cnt==3){
			if(patient_state==GOOD){
					printf("Alarm!!\n");
					patient_state=BAD;	
			}
		}
		else{
				printf("I'm good\n");
				patient_state=GOOD;
		}
		sprintf(query, "update Alarm set state='%d' where patient_name='%s'", patient_state, patient_name);
		mysql_perform_query(conn, query);
		printf("%s\n", query);
	}

	sleep(1000000);
  return 0;
}
 
