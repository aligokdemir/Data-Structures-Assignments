#include <stdlib.h>
#include <stdio.h>
#include <time.h>
int totcall=0,totidle=0,totwait=0,totsertime=0,maxq=0; //variables for the report.
int callnumber=1; //varible to keep track of the number of calls that are handled.
typedef struct QueueNode Node;
typedef	struct QueueRecord q;
struct QueueNode{
	int callnum;
	int arrivalt;
	int waittime;
	int starttime;
	int servtime;
	int endtime;
	Node *next;
};

struct QueueRecord{
	Node *front;
	Node *rear;
	int size;
};

q* create_queue(){
	q *simq;
	simq = (struct q*)malloc(sizeof(q));
	
	simq->size = 0;
	simq->front = malloc(sizeof(Node));
	simq->front->next = NULL;
	simq->rear = simq->front;	
	
	return simq;
}

void enq(q *simq,Node *caller){
	caller->next == NULL;
	
	if(simq->size == 0){
		simq->front = caller;
		simq->rear = caller;
	}
	else{
		simq->rear->next = caller;
		simq->rear = caller;
	}
	simq->size++;
	
}

void deq(q *simq){
	Node *deleted;
	if((simq->size > 0)){
		deleted = simq->front;
		simq->front = simq->front->next;
		free(deleted);
		simq->size--;
	}
}

void call_simulator(q *simq,int time,int servtime){
	int call;
	Node *caller;
	caller = (struct Node*)malloc(sizeof(Node));
	caller->next= NULL;
	
	call = (rand()%2);
	//printf("\n-----%d-------",call);
	
	if(call == 1 && simq->size > 1){
		caller->callnum = callnumber;
		caller->arrivalt = time;
		caller->servtime = ((rand() % servtime) + 1);
		caller->tempserv = caller->servtime;
		caller->starttime = (simq->rear->starttime)+(simq->rear->servtime)+1;
		caller->endtime = (caller->starttime)+(caller->servtime);
		caller->waittime = caller->starttime - caller->arrivalt;
		
		enq(simq,caller);
		if(simq->size > maxq)
			maxq = simq->size;
		totcall++;
		callnumber++;
		
	}
	//if queue size is one, take the starttime as clock time because there is no rear element in the queue.
	else if(call == 1 && simq->size <= 1){
		caller->callnum = callnumber;
		caller->arrivalt = time;
		caller->servtime = ((rand() % 2) + 1);
		caller->tempserv = caller->servtime;
		caller->starttime = caller->arrivalt;
		caller->waittime = 0;
		caller->endtime = (caller->starttime)+(caller->servtime)+1;
		
		enq(simq,caller);
		if(simq->size > maxq)
			maxq = simq->size;	
		totcall++;
		callnumber++;
	}
	
	if(simq->size == 1){
		simq->front->starttime=time;
	}
	if(simq->size == 0) //if q size is 0 this means no calls are being received for that time.
		totidle++;
	
}
//
void process_active_call(q *simq,int cltime){

	if(simq->front->endtime == cltime-1){
		printf("\n%d\t%d\t%d\t\t%d\t\t%d\t\t%d\t%d\n",cltime,simq->front->callnum,simq->front->arrivalt,simq->front->waittime,simq->front->starttime,
		simq->front->servtime,simq->size-1); //-1 is there because when the call is finished it should leave the queue
	}
	
}

void start_new_call(q *simq, int cltime){
	if(simq->front->endtime == cltime && simq->size > 0){
		totsertime += simq->front->servtime;
		totwait += simq->front->waittime;
		deq(simq);
		//simq->size--;
	}
	
}

void run_simulator(q *simq,int servtime,int maxst){
	int i=0;
	for(i;i<servtime*60;i++){
		
		call_simulator(simq,i,maxst);
		
		//process_active_call(simq,i);
		
		if(simq->size > 0){
			start_new_call(simq,i);
		}
		
	}
	
	while(simq->size != 0){
		totsertime += simq->front->servtime;
		totwait += simq->front->waittime;
		deq(simq);
	}//this while loop continues until there is no calls waiting in the queue
	
}

void finalise_report_simulator(){
	float avgwait = totwait/totcall;
	float avgservice = totsertime/totcall;
	printf("\nReport for the calls during service time is below.\n\n");
	printf("\n\nMaximum Queue Size: %d\n", maxq);
	printf("Total Calls Received: %d\n", totcall);
	printf("Total Service Time: %d\n", totsertime);
	printf("Total Idle Time: %d\n",totidle);
	printf("Total Wait Time: %d\n",totwait);
	printf("Average Wait Time: %.2f\n",avgwait);
	printf("Average Service Time: %.2f\n",avgservice);
}

int main(){
	int serv,maxst;
	srand(time(NULL));
	
	printf("Enter the hours service will be provided: ");
	scanf("%d",&serv);
	printf("Enter the maximum service time for one client: ");
	scanf("%d",&maxst);

	q *myq;
	
	myq = create_queue();
	
	printf("Clock\tCall N\tArrival T\tWait T\t     Start T\t     Service T\tQueue S");
	run_simulator(myq,serv,maxst);
	wait_time(myq);
	finalise_report_simulator();
	
	
	return 0;
}
