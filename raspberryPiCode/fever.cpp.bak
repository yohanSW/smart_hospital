#include "fever.h"

namespace han{

	Fever();
	void Fever::aduino_setting(String device_port){
		//get filedescriptor
		if ((fd = serialOpen (device_port, 115200)) < 0){
			fprintf (stderr, "Unable to open serial device: %s\n", strerror (errno)) ;
			exit(1); //error
		}
 
		//setup GPIO in wiringPi mode
		if (wiringPiSetup () == -1){
			fprintf (stdout, "Unable to start wiringPi: %s\n", strerror (errno)) ;
			exit(1); //error
		}

		//setup the others
		name = "yohan";
		danger = 0;
	}


	void Fever::get_sensor(){
		char buf[200], flushBuf[200]={0, };
		char *ptr;
		char bufStr[20];
		int i = 0;

		while(1){
			
	
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
			if(i>40){
				ptr = strchr(buf, '!');
				strncpy(bufStr,ptr+1,3);
				if(bufStr[2]==',')
					bufStr[2]='\n';
				else bufStr[3]='\n';
				m.lock();
				heart_rate = atoi(bufStr);
				m.unlock();
	
				ptr = strchr(buf, '%');
				strncpy(bufStr,ptr+1,5);
				bufStr[5]='\n';
				m.lock();
				temp = atof(bufStr);
				printf("heart : %d , temp : %lf\n",heart_rate, temp);
				m.unlock();

				strcpy(buf, flushBuf);
				i = 0;
			}
		    //fgets(bufStr, sizof(bufStr),stdout);
  		  fflush(stdout);
		  }
		}	
	}


	void Fever::give_order(){
		cout << "give_order" << endl;
	}
}