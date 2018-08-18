#include "ICU.h"

namespace han
{

	void ICU::gpio_setting(){
		//setup GPIO in wiringPi mode
		if (wiringPiSetupGpio() == -1){
		fprintf (stdout, "Unable to start wiringPi: %s\n", strerror (errno)) ;
		exit(1);
		}
		
		//FAN은 선풍기 IO 포트 번호 지정
		pinMode(FAN, OUTPUT);
		pinMode(BUTTON, INPUT);
		pinMode(LED_RED, OUTPUT);
		pinMode(LED_YELLOW, OUTPUT);

	}

	void ICU::setup_raspberry(){
				cout << "raspberry, aduino setting" <<endl;
				patient1.aduino_setting("/dev/ttyACM0");
				patient2.aduino_setting("/dev/ttyACM1");
				gpio_setting();
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


void ICU::upload_data(){ //mysql
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
	
		while(1){
			sleep(2);
			time(&now);
			////////////////patient1 전용//////////////////////
			patient1.m.lock();
			sprintf(query,"insert into %s (time, heart_rate, temp, alarm) value (%d,%d,%lf,%d);", "patient1", now, patient1.get_heart_rate(), patient1.get_temp(), patient1.get_danger());
			patient1.m.unlock();
			printf("%s\n",query);
			res = mysql_perform_query(conn, query);
			////////////////patient2 전용//////////////////////
			patient2.m.lock();
			sprintf(query,"insert into %s (time, heart_rate, weight, alarm) value (%d,%d,%lf,%d);", "patient2", now, patient2.get_heart_rate(), patient2.get_infusion_solution(), patient2.get_danger());
			patient2.m.unlock();
			printf("%s\n",query);
			res = mysql_perform_query(conn, query);
	
		sleep(5);

		
		}	
	}

	void ICU::print(){
		cout << "ICU print << \t room temp : " << room_temp << "\t danger_situation : " << danger_situation << endl;
		patient1.print();
		patient2.print();
		nurse.print();
		cout << endl;
	}
	
	
	void ICU::situation_control(){

		int danger = get_danger_situation();
		int room_temperature = get_room_temp();
		int set_room_temp = 26;

		switch(danger){
			case 1: danger = nurse.danger_emergency(); break;
			case 2: danger = nurse.danger_ringer(); break;
			case 3: danger = nurse.danger_situation_off(); break;
			case 0: break;
		}

		if(set_room_temp < room_temperature){
			digitalWrite(FAN, HIGH);
		}
		else digitalWrite(FAN, LOW);

	}// 민지

	void ICU::get_sensor(){
		while(true){
			patient1.get_sensor();
			patient2.get_sensor();
			judge_danger();
		}
	}
/*#define DANGEROUS_TEMP 30.0
#define DANGEROUS_BEAT 130

#define GOOD	1
#define SOSO	2
#define BAD		3
#define BUTTON	5	*/
	void ICU::judge_danger(){
		patient1.m.lock();
		if(patient1.get_temp() > DANGEROUS_TEMP)
			patient1.set_danger(2);
		else if(patient1.get_heart_rate() > DANGEROUS_BEAT)
			patient1.set_danger(1);
		else
			patient1.set_danger(0);
		patient1.m.unlock();
		patient2.m.lock();
		if(patient2.get_infusion_solution() < DANGEROUS_INFUSION_WEIGHT)
			patient2.set_danger(2);
		else if(patient2.get_heart_rate() > DANGEROUS_BEAT)
			patient2.set_danger(1);
		/*else if(patient2.get_wake_up() == 1)
			patient2.set_danger(3);*/
		else
			patient2.set_danger(0);
		patient2.m.unlock();
		cout << "fever_patient danger is --"<<patient1.get_danger() <<"-- vegeta_patient danger is --"<<patient2.get_danger() <<"--"<<endl;

	}
}
