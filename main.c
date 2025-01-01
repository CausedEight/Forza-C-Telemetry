#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <arpa/inet.h>

#define BUFFLEN 2016
#define PORT 6969

typedef struct{
	int32_t 	isRaceOn;
	uint32_t	timeStamp;
	float		engineMaxRPM;
	float		idleRPM;
	float		currentEngineRPM;
	float		accX, accY, accZ;
	float		velX, velY, velZ;
	float		angleVelX, angleVelY, angleVelZ; //updated?
	float		yaw, pitch, roll;
	float		normalizedSuspensionTravelFL, normalizedSuspensionTravelFR, normalizedSuspensionTravelRL, normalizedSuspensionTravelRR;
	float		tireSlipFL, tireSlipFR, tireSlipRL, tireSlipRR;
	float		wheelRotationFL, wheelRotationFR, wheelRotationRL, wheelRotationRR;
	int32_t		rumbleFL, rumbleFR, rumbleRL, rumbleRR;
	float		puddleFL, puddleFR, puddleRL, puddleRR;
	float		void1, void2, void3, void4;
	float		slipAngleFL, slipAngleFR, slipAngleRL, slipAngleRR;
	float		combSlipFL, combFlipFR, combSlipRL, combSlipRR;
	float		suspensionTravelMFL, suspensionTravelMFR, suspensionTravelMRL, suspensionTravelMRR;
	int32_t		carID;
	int32_t		carClass;
	int32_t		carIndex;
	int32_t		drivetrain;
	//Everything here is additional and idk if it will read correctly
	float 		void5, void6, void7, void8;
	float		X, Y, Z;// For some reason don't work
	float		speed;
	float 		power;
	float		torque;
	float		tireTempFL, tireTempFR, tireTempRL, tireTempRR;
	float		boost;
	float 		fuel;
	float 		distTravelled;
	float 		bestLap;
	float 		lastLap;
	float		currentLap;
	float		raceTimer;
	uint16_t	lapNumber;
	uint8_t		pos;
	uint8_t		accel, brake, clutch, handbrake, gear;
	int8_t		steer, driveline, brakeDiff;
	float		tireWearFL, tireWearFR, tireWearRL, tireWearRR;
	int32_t		trackID;
} TelemetryData;


void die(char *s){
	perror(s);
	exit(1);
}

void print_shifter(float currentRPM, float maxRPM, int gear){
	printf("Current RPM: %d\nMax RPM: %d\n", (int)currentRPM, (int)maxRPM);
	float ratio = 1/(maxRPM / currentRPM);
	printf("Ratio: %f\n", ratio);
	printf("Gear: ");
	if (gear == 0){
		printf("R\n");
	}else{
		printf("%d\n", gear);
	}
	printf("\033[42m  \033[0m");
	if(ratio > 0.1){
		printf("\033[42m  \033[0m");
	}
	if (ratio > 0.2){
		printf("\033[42m  \033[0m");
	}
	if(ratio > 0.3){
		printf("\033[42m  \033[0m");
	}
	if(ratio > 0.4){
		printf("\033[42m  \033[0m");
	}
	if(ratio > 0.5){
		printf("\033[43m  \033[0m");
	}
	if(ratio > 0.6){
		printf("\033[43m  \033[0m");
	}
	if(ratio > 0.7){
		printf("\033[43m  \033[0m");
	}
	if (ratio > 0.8){
		printf("\033[41m  \033[0m");
	}
	if (ratio > 0.9){
		printf("\033[41m  \033[0m");
	}
	printf("\n");
}

void print_pressures(float a, float b){
	int aR = (int)((a / 255) * 10);
	int bR = (int)((b/255)*10);
	printf("Accelerator pressure: ");
	for (int i = 0 ; i < aR; i++){
		printf("\033[104m  \033[0m");
	}
	printf("\nBrake pressure:       ");
	for (int i = 0 ; i < bR; i++){
		printf("\033[41m  \033[0m");
	}
}

void print_hex_buffer(const unsigned char * buffer, size_t length){
	TelemetryData d;
	for (size_t i = 0 ; i < length; i ++){
		memcpy(&d, buffer, sizeof(TelemetryData));
		//printf("%0.2X ", buffer[i]);
		//if ((i+1) % 20 == 0) printf("\n");
	}
	print_shifter(d.currentEngineRPM, d.engineMaxRPM, d.gear);
	print_pressures(d.accel, d.brake);
	/*
	printf("\nIs race on: %d\n", d.isRaceOn); //Works
	printf("Time stamp: %d\n", d.timeStamp); //Works
	printf("Engine max RPM: %f\n", d.engineMaxRPM); //Works
	printf("Idle RPM: %f\n", d.idleRPM); //Works
	printf("Current engine RPM: %f\n", d.currentEngineRPM); //Works
	printf("Body acceleration (X: %f\tY: %f\tZ: %f)\n", d.accX, d.accY, d.accZ); //Works
	printf("Body velocity (X: %f\tY: %f\tZ: %f)\n", d.velX, d.velY, d.velZ); //Works
	printf("Body angular velocity: (X: %f\tY: %f\tZ: %f)\n", d.angleVelX, d.angleVelY, d.angleVelZ); //Works
	printf("Yaw: %f\tPitch: %f\tRoll: %f\n", d.yaw, d.pitch, d.roll); //Works
	printf("Normalized suspension (FL: %f\tFR: %f\tRL: %f\tRR: %f)\n", d.normalizedSuspensionTravelFL, d.normalizedSuspensionTravelFR, d.normalizedSuspensionTravelFL, d.normalizedSuspensionTravelRR); //Works
	printf("Tire slip: (FL: %f\tFR: %f\tRL: %f\tRR: %f)\n", d.tireSlipFL, d.tireSlipFR, d.tireSlipRL, d.tireSlipRR); //Works
	printf("Wheel rotation: (FL: %f\tFR: %f\tRL: %f\tRR: %f)\n", d.wheelRotationFL, d.wheelRotationFR, d.wheelRotationRL, d.wheelRotationRR); //Works
	printf("Rumble: (FL: %d\tFR: %d\tRL: %d\tRR: %d)\n", d.rumbleFL, d.rumbleFR, d.rumbleRL, d.rumbleRR); //Works
	printf("PuddleL: (FL: %f\tFR: %f\tRL: %f\tRR: %f)\n", d.puddleFL, d.puddleFR, d.puddleRL, d.puddleRR);//Undetermined
	printf("Controller rumble values: (FL: %f\tFR: %f\tRL: %f\tRR: %f)\n", d.void1, d.void2, d.void3, d.void4); //Works
	printf("Slip angle: (FL: %f\tFR: %f\tRL: %f\tRR: %f)\n", d.slipAngleFL, d.slipAngleFR, d.slipAngleRL, d.slipAngleRR); //Works
	printf("Combined slip: (FL: %f\tFR: %f\tRL: %f\tRR: %f)\n", d.combSlipFL, d.combFlipFR, d.combSlipRL, d.combSlipRR); //Works
	*/
	printf("\n");
}

int main(void){
	int sockfd;
	char buf[BUFFLEN];
	//Creates 2 structs of type sockaddr_in (as given by netinet) and we are calling them si_me and si_other
	//si_me is my socket
	//si_other is the incoming traffic socket
	struct sockaddr_in si_me, si_other;

	socklen_t slen = sizeof(si_other);

	//Create a socket
	sockfd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	if (sockfd < 0){
		perror("Socket creation failed");
		exit(EXIT_FAILURE);
	}

	memset(&si_me, 0, sizeof(si_me));

	//Fill in structure data
	si_me.sin_family = AF_INET;
	si_me.sin_port = htons(PORT);
	si_me.sin_addr.s_addr = htonl(INADDR_ANY);

	//Bind the socket to the port
	if (bind(sockfd, (struct sockaddr *)&si_me, sizeof(si_me)) < 0){
		perror("Binding error");
		exit(EXIT_FAILURE);
	}

	//Now listen to incoming traffic
	while (1){
		memset(buf, 0, BUFFLEN);
		ssize_t t = recvfrom(sockfd, buf, BUFFLEN, 0, (struct sockaddr *)&si_other, &slen);
		printf("Size of recieved packet: %zd\n", t);
		if (t == 1){
			perror("Receiving error");
			exit(EXIT_FAILURE);
		}
		printf("Recieved packets from %s:%d\n", inet_ntoa(si_other.sin_addr), ntohs(si_other.sin_port));
		printf("Data: \n");
		print_hex_buffer((unsigned char *)buf, t);

	}


	return 0;
}

