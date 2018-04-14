#include "opencv2/objdetect.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/core.hpp"
#include "opencv2/face.hpp"

#include <iostream>
#include <fstream>
#include <sstream>
#include <stdio.h>
#include <mysql.h>
#include <dirent.h>
#include <wiringPi.h>
#include <unistd.h>
#include <fcntl.h>
#include <pthread.h>
#include <stdint.h>
#include <stdlib.h>
#include <errno.h>
#include <wiringSerial.h>
#include <string.h>
#include <softPwm.h>

using namespace std;
using namespace cv;
using namespace cv::face;

#define SERVO	1	
#define MIN_RECOG 10
#define MAXLIST 100

/** Function Headers */
void update(int index,char *name);
void initArr(int list[]);
void SaveFile();

/** Global variables */
String face_cascade_name = "/usr/share/opencv/haarcascades/haarcascade_frontalface_alt.xml";
CascadeClassifier face_cascade;
int listarr[100]={0};
int cnt = 0;
char device[]="/dev/ttyACM0";
int fd;
unsigned long baud=9600;
unsigned long time_ardu=0;
int sensor_thread_start=0;

void setup(){
	printf("%s\n", "Raspberry Pi Startup!");
	fflush(stdout);

	if((fd = serialOpen(device, baud)) < 0){
		fprintf(stderr, "Unable to open serial device: %s\n", strerror(errno));
		exit(1);
	}

	if(wiringPiSetup() == -1){
		fprintf(stdout, "Unable to start wiringPi: %s\n", strerror(errno));
		exit(1);
	}
}

static void read_csv(const string& filename, vector<Mat>& images, vector<int>& labels, char separator = ';'){
	///
	MYSQL mysql;
	MYSQL_RES* res;
	MYSQL_ROW row;

	DIR *dp;
	struct dirent *dent;
	char f_name[200];
	char user_name[200];
	char pwd[200];
	char sql_query[200];
	int i, j, k;
	int rows;

	mysql_init(&mysql);

	if(!mysql_real_connect(&mysql,"220.70.2.32", "doctor", "doctor","Hospital",3306, (char*)NULL, 0)){
		printf("%s\n", mysql_error(&mysql));
		return;
	}
	printf("db connect \n");
	/*
	if(!mysql_real_connect(&mysql, NULL, "root", "root", "security", 3306, (char*)NULL, 0)){
		printf("%s\n", mysql_error(&mysql));
		return;
	}
	*/

	printf("Database connect success\n");

	string line, path, classlabel;

	if((dp = opendir("/srv/http/facedata"))==NULL){
		perror("opendir: ../facedata");
		exit(1);
	}
	while((dent = readdir(dp))){
		strcpy(f_name, dent->d_name);
		printf("f_name = %s\n", f_name);
		
		if(strcmp(f_name, ".") && strcmp(f_name, "..")){
			i=0;
			while(f_name[i]!='_'){
				user_name[i]=f_name[i];
				i++;
			}
			user_name[i]=0;

			strcpy(pwd, "/srv/http/facedata/");
			strcat(pwd, f_name);

			path=pwd;

			sprintf(sql_query, "SELECT num FROM info where name='%s'", user_name);

			if(mysql_query(&mysql, sql_query)){
				printf("%s \n", mysql_error(&mysql));
				return;
			}

			res = mysql_store_result(&mysql);
			rows=mysql_num_rows(res);
			if(rows){
				row = mysql_fetch_row(res);
				classlabel = row[0];
				printf("name = %s, index = %s\n", user_name, row[0]);
				if(!path.empty() && !classlabel.empty()){
					Mat m = imread(path, 1);
					cvtColor(m, m, CV_BGR2GRAY);
					images.push_back(m);
					labels.push_back(atoi(classlabel.c_str()));
				}
			} else{
				printf("user name \"%s\" does not exists in Database\n", user_name);
			}
			mysql_free_result(res);
		}
	}
	closedir(dp);
	mysql_close(&mysql);
}

Ptr<FaceRecognizer> setRecognizer(int &im_width, int &im_height){
	vector<Mat> images;
	vector<int> labels;
	try{
		read_csv("/srv/http/facedata", images, labels);
	} catch(cv::Exception& e){
		cerr << "Error setRecognizer Reason: " << e.msg << endl;
		exit(1);
	}

	cout << "Database Read Complete" << endl;
	
	if(images.size() <= 1){
		string error_message = "This demo needs at least 2 images to work. Please add more images to your data set!";
		CV_Error(Error::StsError, error_message);
	}

	im_width = images[0].cols;
	im_height = images[0].rows;
	Ptr<FaceRecognizer> model = FisherFaceRecognizer::create();
	cout << "face training started" << endl;
	model -> train(images, labels);
	cout << "face training finished" << endl;
	return model;
}

void regFace(char name[], char job[]){
	MYSQL mysql;
	MYSQL_RES* res;
	MYSQL_ROW row;
	char query[256]={0};
	mysql_init(&mysql);
	if(!mysql_real_connect(&mysql,"220.70.2.32", "doctor", "doctor","Hospital",3306, (char*)NULL, 0)){
		printf("%s\n", mysql_error(&mysql));
		return;
	}
	//insert data into DB
	//**********fill******

	int cnt=0;
	char dir[200]="/srv/http/facedata/";
	char str[200];
	int num = 1;
	
	VideoCapture capture;
	std::vector<Rect> faces;
	Mat frame;
	Mat frame_gray;
	Mat face_resized;
	IplImage* bgr_frame;
	IplImage copy;
	ifstream name_checker;

    if( !face_cascade.load( face_cascade_name ) ){ printf("--(!)Error loading face cascade\n"); exit(-1); };
	capture.open(-1);
	if( !capture.isOpened() ){
		printf(" Error opening video capture!\n");
		exit(-1);
	}

	while(cnt < 15){
		capture.read(frame);
		if(frame.empty()){
			printf("No captured frame -- Break!\n");
			exit(-1);
		}
		cvtColor(frame, frame_gray, COLOR_BGR2GRAY);
		equalizeHist(frame_gray, frame_gray);

		face_cascade.detectMultiScale( frame_gray, faces, 1.1, 2, 0|CASCADE_SCALE_IMAGE, Size(90, 90) );
		Rect face_i;
		if(faces.size() != 0){
			cout << "face detected" << endl;
			face_i = faces[0];
			Mat face = frame_gray(face_i);
			Mat face_resized;
			rectangle(frame, face_i, CV_RGB(0, 255, 0), 1);

			cv::resize(face, face_resized, Size(90, 90), 1.0, 1.0, INTER_CUBIC);

			while(true){
				sprintf(str, "%s%s_%d.jpg", dir, name, num);
				name_checker.open(str);
				if(!name_checker.is_open())
					break;
				num++;
				name_checker.close();
			}
			copy = face_resized;
			bgr_frame=&copy;
			cvSaveImage(str, bgr_frame);
			printf("%s saved.\n", str);
			cnt++;
		}
	}
	printf("Finish registering face.\n");
}

void saveImage(Mat frame){
	IplImage* bgr_frame;
	IplImage copy;
	
	copy = frame;
	bgr_frame=&copy;
	cvSaveImage("/srv/http/origin2.jpg", bgr_frame);
}

int DetectFace(Mat frame, Ptr<FaceRecognizer> model){
	std::vector<Rect> faces;
	Mat frame_gray;
	int prediction;

	//to grayscale
	cvtColor(frame, frame_gray, COLOR_BGR2GRAY);
	equalizeHist(frame_gray, frame_gray);

	//detect face
	face_cascade.detectMultiScale(frame_gray, faces, 1.1, 2, 0|CASCADE_SCALE_IMAGE, Size(90, 90) );
	Rect face_i;
	if(faces.size() != 0){
		printf("face detected\n");
		face_i = faces[0];
		Mat face = frame_gray(face_i);
		Mat face_resized;
		rectangle(frame, face_i, CV_RGB(0, 255, 0), 1);

		cv::resize(face, face_resized, Size(90, 90), 1.0, 1.0, INTER_CUBIC);
		prediction = model -> predict(face_resized);

		listarr[prediction]++;
	}
	return prediction;
}

void *Sensing(void *arg){
	while(!sensor_thread_start){}
	printf("Sensor thread start!\n");

	char buf[200];
	char *ptr;
	char tempstr[3], humistr[3];
	int temp, humi;
	int i=0;
	
	setup();
	
	MYSQL mysql;
	MYSQL_RES* res;
	MYSQL_ROW row;
	int dbtemp;
	int sleeptime;
	char query[256]={0};
	int airconditionerOn=0;
	int airstate=0;
	char ircmd[100];
	
	mysql_init(&mysql);
	if(!mysql_real_connect(&mysql,"220.70.2.32", "doctor", "doctor","Hospital",3306, (char*)NULL, 0)){
		printf("%s\n", mysql_error(&mysql));
		exit(-1);
	}
	
	sprintf(query, "SELECT * FROM room WHERE number='%d'", 0);
	if(mysql_query(&mysql, query)){
		printf("%s\n", mysql_error(&mysql));
		exit(-1);
	}
	res = mysql_store_result(&mysql);
	if(row = mysql_fetch_row(res)){
		airconditionerOn=atoi(row[1]);
		dbtemp=atoi(row[2]);
		sleeptime=atoi(row[3]);

		printf("airconditionerOn = %d\n", airconditionerOn);
		printf("dbtemp = %d\n", dbtemp);
		printf("sleep_time = %d\n", sleeptime);
	}

	while(1){
		if(millis()-time_ardu>= 3000){
			serialPuts(fd, "Pong!\n");
			serialPutchar(fd, 65);
			time_ardu=millis();
		}

		if(serialDataAvail(fd)){
			buf[i++] = serialGetchar(fd);
			if(i>12){
				ptr=strchr(buf, '!');
				if(ptr==NULL)
					continue;
				strncpy(tempstr, ptr+1, 2);
				ptr=strchr(buf, '@');
				if(ptr==NULL)
					continue;
				strncpy(humistr, ptr+1, 2);
				temp=atoi(tempstr);
				humi=atoi(humistr);
				
				sprintf(query, "SELECT * FROM room WHERE number='%d'", 0);
				if(mysql_query(&mysql, query)){
					printf("%s\n", mysql_error(&mysql));
					exit(-1);
				}
				res = mysql_store_result(&mysql);
				if(row = mysql_fetch_row(res)){
					airconditionerOn=atoi(row[1]);
					dbtemp=atoi(row[2]);
					sleeptime=atoi(row[3]);
				}
				
				printf("airconditionerOn = %d\n", airconditionerOn);
				if(temp > 10 && humi > 10){
					printf("Temperature = %d, Humidity = %d\n", temp, humi);
					if(dbtemp>temp){
						//airconditioner
						if(airconditionerOn==0){
							system("irsend SEND_ONCE lgac UN-JEON/JEONG-JI_22");
							printf("airconditioner ON\n");
							airconditionerOn=1;
							sprintf(query, "UPDATE room set airon='%d' where number='%d'", airconditionerOn, 0);
							printf("%s\n", query);
							mysql_query(&mysql, query);
							res=mysql_store_result(&mysql);
						}
						if(airstate != 1){//1 : HIGH
							sprintf(ircmd, "irsend SEND_ONCE lgac HIGH_%d", dbtemp);
							system(ircmd);
							//system("irsend SEND_ONCE lgac HIGH_26");
							printf("airconditioner HIGH 26\n");
							airstate=1;
						}
						printf("cold!!\n");
					}
					else if(dbtemp<temp){
						if(airconditionerOn==0){
							system("irsend SEND_ONCE lgac UN-JEON/JEONG-JI_22");
							printf("airconditioner ON\n");
							airconditionerOn=1;
							sprintf(query, "UPDATE room set airon='%d' where number='%d'", airconditionerOn, 0);
							printf("%s\n", query);
							mysql_query(&mysql, query);
							res=mysql_store_result(&mysql);
						}
						if(airstate != 2){ // 2 : LOW
							sprintf(ircmd, "irsend SEND_ONCE lgac LOW_%d", dbtemp);
//							system("irsend SEND_ONCE lgac LOW_26");
							system(ircmd);
							printf("airconditioner LOW 26\n");
							airstate=2;
						}
						printf("hot!!\n");
					}
					else{
						system("irsend SEND_ONCE lgac UN-JEON/JEONG-JI_22");
						printf("airconditioner OFF\n");
						airconditionerOn=0;
						sprintf(query, "UPDATE room SET airon='%d' WHERE number='%d'", airconditionerOn, 0); 
						printf("%s\n", query);
						if(mysql_query(&mysql, query)){
							printf("%s\n", mysql_error(&mysql));
							exit(-1);
						}
					}
					sprintf(query, "UPDATE room SET temperature='%d' WHERE number='%d'", temp, 1);
					if(mysql_query(&mysql, query)){
						printf("%s \n", mysql_error(&mysql));
						exit(-1);
					}
				}
				i=0;
			}
			fflush(stdout);
		}
	}
	mysql_free_result(res);
	mysql_close(&mysql);
	exit(-1);
}

/** @function main */
int main( void )
{
	////speaker
	int speakerPin = 12;
    if( wiringPiSetup() == -1)
    	return 1;

    VideoCapture capture;
    Mat frame;
	int im_width;
	int im_height;
	char key;
	int listarr_index=0;
    
	//Sensor Thread
	pthread_t sensor_thread;
	if(pthread_create(&sensor_thread, NULL, &Sensing, NULL))
		printf("thread create failed!!\n");

	//-- 1. Load the cascades
    if( !face_cascade.load( face_cascade_name ) ){ printf("--(!)Error loading face cascade\n"); return -1; };
	SaveFile();
	Ptr<FaceRecognizer> model = setRecognizer(im_width, im_height);

	//sensor_thread start
	sensor_thread_start=1;

	capture.open(-1);
    //-- 2. Read the video stream
    if ( ! capture.isOpened() ) { 
		printf("--(!)Error opening video capture\n");
		return -1;
	}

	////////////////////// edition
	
	fcntl(0, F_SETFL, O_NONBLOCK);
	while(true){//nonblock
		scanf("%c", &key);//another method
		if(key=='0'){
			char name[10], job[10];
			printf("name : ");
			fcntl(0, F_SETFL, O_SYNC);
			scanf("%s", name);
			printf("job : ");
			scanf("%s", job);
			if(strcmp(job, "nurse")!=0 || strcmp(job, "family")!=0){
				printf("Wrong job\n");
				return -1;
			}
			regFace(name, job);
		}
		capture.read(frame);
		if(frame.empty()){
			printf("No captured frame -- Break!\n");
			break;
		}
		saveImage(frame);
		listarr_index=DetectFace(frame, model);
		if(listarr[listarr_index] >= MIN_RECOG){
			char dbname[256]={0};
			update(listarr_index, dbname);
			if(strcmp(dbname, "")){
				//TO DO
				printf("'%s' is entering\n", dbname);
				softPwmCreate(SERVO, 0, 200);
				softPwmWrite(SERVO, 13);
				delay(3000);
				softPwmWrite(SERVO, 22);
				delay(1000);
				softPwmStop(SERVO);
				initArr(listarr);
			}
		}	
	}
    return 0;
}

void update(int index,char *name) {
	MYSQL mysql;
	MYSQL_RES* res;
	MYSQL_ROW row;
	char query[256] = {0};
	mysql_init(&mysql);
	if(!mysql_real_connect(&mysql,"220.70.2.76", "doctor", "doctor","hospital",3306, (char*)NULL, 0)){
		printf("%s\n", mysql_error(&mysql));
		return;
	}
/*	
	if(!mysql_real_connect(&mysql, NULL, "root","root", "security" ,3306, (char *)NULL, 0))
		printf("%sï¼¼n",mysql_error(&mysql));
	*/
	sprintf(query,"SELECT name FROM info WHERE num='%d'",index);
	if(mysql_query(&mysql,query)) {
		printf("%sï¼¼n",mysql_error(&mysql));
		return ;
	}
	res = mysql_store_result(&mysql);
	if(row = mysql_fetch_row(res)) {
		printf("name = %s\n",row[0]);
		strcpy(name,row[0]);
	} else {
		strcpy(name,"");
	}
	mysql_free_result(res);
	mysql_close(&mysql);
	return;
}

void SaveFile(){
	DIR *dp;
	struct dirent *dent;
	char f_name[200];
	char user_name[200];
	char path[200]="/srv/http/picture/";
	char dir[200]="/srv/http/facedata/";
	char str[200];
	char c;
	int i, num;
	std::vector<Rect>  faces;
	Mat frame_gray;
	IplImage* bgr_frame;
	IplImage copy;
	ifstream name_checker;

	if((dp = opendir("/srv/http/picture")) == NULL){
		perror("opendir: //'picture");
		exit(1);
	}

	while((dent = readdir(dp))){
		if(!strcmp(dent->d_name, ".") || !strcmp(dent->d_name, ".."))
			continue;
		strcpy(path, "/srv/http/picture/");
		strcpy(f_name, dent->d_name);
		strcat(path, f_name);

		Mat frame = imread(path, 1);
		printf("%s opened\n", path);
	
   		cvtColor( frame, frame_gray, COLOR_BGR2GRAY );
  	 	equalizeHist( frame_gray, frame_gray );
	
	    //-- Detect faces
	  	face_cascade.detectMultiScale( frame_gray, faces, 1.1, 2, 0|CASCADE_SCALE_IMAGE, Size(90, 90) );
		Rect face_i;
		if(faces.size() != 0){
			cout << "face detected" << endl;
			face_i = faces[0];
			Mat face = frame_gray(face_i);
			Mat face_resized;
			rectangle(frame, face_i, CV_RGB(0, 255, 0), 1);
				
			cv::resize(face, face_resized, Size(90, 90), 1.0, 1.0, INTER_CUBIC);
			//
			copy = face_resized;
			bgr_frame=&copy;

			f_name[3]=0;
			num=1;
			while(true){
				sprintf(str, "%s%s_%d.jpg", dir, f_name, num);
				name_checker.open(str);
				if(!name_checker.is_open())
					break;
				num++;
				name_checker.close();
			}
			printf("%s\n", str);
			cvSaveImage(str, bgr_frame);
		}
	}
}

void initArr(int list[]){
	for(int i=0;i<MAXLIST;i++)
		list[i]=0;
}
