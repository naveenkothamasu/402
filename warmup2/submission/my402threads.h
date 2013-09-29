#include<sys/time.h>
#include "my402list.h"
#include "cs402.h"
#include "my402constants.h"

#define handle_errors(err, exception) \
	do{errno = err; perror(exception); exit(EXIT_FAILURE); }while(0)


typedef struct tagFilterData {

	My402List *pListQ1;
	My402List *pListQ2;
	int tokenCount;

	int isStopNow;
	int isMorePackets;
	
	My402Stats *stats;
	
} My402FilterData;

typedef struct tagPrintTime{
	
	int intPart;
	int decPart;
	long long  actual_num;

} printtime;

typedef struct tagMyPacket {
	
	long long int packet_num; 
	long long inter_arrival_time;
	int tokens;
	long long service_time;
	printtime q1_begin_time;
	printtime q1_end_time;
	printtime q2_begin_time;
	printtime q2_end_time;
	printtime arrivalStamp;
} My402Packet;

typedef struct tagMyStats {

	
	double avg_time_in_q1;
	double sd_time_spent_system;
	double avg_time_spent_system;
 
} My402Stats;

typedef struct tagArrivalStats {

	double avg_inter_arrival_time;
	double avg_packets_in_q1;
	long packets_dropped;
	long current_packets;

} My402ArrivalStats;

typedef struct tagTokenStats {

	long tokens_dropped;
	long current_tokens;

} My402TokenStats;

typedef struct tagServiceStats {
	double avg_service_time;
	double avg_packets_in_q2;
	double avg_packets_in_s;

	long long packets_served;
} My402ServiceStats;


void *arrivalManager(void *);
void *tokenManager(void *);
void *serviceManager(void *);
