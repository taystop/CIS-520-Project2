#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <limits.h>

#include "dyn_array.h"
#include "processing_scheduling.h"


// You might find this handy.  I put it around unused parameters, but you should
// remove it before you submit. Just allows things to compile initially.
#define UNUSED(x) (void)(x)

// private function
void virtual_cpu(ProcessControlBlock_t *process_control_block) 
{
    // decrement the burst time of the pcb
    --process_control_block->remaining_burst_time;
}

bool first_come_first_serve(dyn_array_t *ready_queue, ScheduleResult_t *result) 
{
    //Initial null checks
    if(ready_queue == NULL)
    {
    	return false;
    }
    if(result == NULL){
    	return false;
    }
    //parameters for calculations specified prior to any analytical opperations
    int size = dyn_array_size(ready_queue);
    ProcessControlBlock_t p[size];
    int wt[size];
    int totalwait = 0;
    int totalburst = 0;
    //Clock timer started for total run time update
    clock_t clockstart = clock();
    //Each PCB is pulled from the queue to an array of each pcb object
    for(int i = 0; i < size; i++)
    {
    dyn_array_extract_front(ready_queue,&p[i]);
    }

    //sort pcbs by arrival time
    for(int i = 0; i < size; i++){
        int pos = i;
        for(int j = i + 1; j < size; j++){
                if(p[j].remaining_burst_time < p[pos].remaining_burst_time)
                        pos = j;
        }

        ProcessControlBlock_t temp = p[i];
        p[i] = p[pos];
        p[pos] = temp;
    }
	
    //The initial wait time will always be zero since the first object is processed first no matter what
    wt[0] = 0;
    //Nested for loop used to attain each process prior to the current one we are soluting the wait time for
    for (int i=1; i < size; i++)
    	{
    	for(int j = 0; j < i; j++)
    		{
    		wt[i] += p[j].remaining_burst_time;
    	}
    }
    //The total wait time and total burst time are computed
    for(int v = 0; v < size; v++)
    {
    	totalwait += wt[v];
      	totalburst += (int)p[v].remaining_burst_time;
    }
    //global variables are updated by local computations
    result->average_turnaround_time = (float)((totalburst + totalwait) / size);
    result->average_waiting_time = (float)(totalwait / size);
    result->total_run_time = (unsigned long)((clock() - clockstart) / CLOCKS_PER_SEC);
    //UNUSED(ready_queue);
    //UNUSED(result);
    return true;
}

bool shortest_job_first(dyn_array_t *ready_queue, ScheduleResult_t *result) 
{
    //variable allocation
    int size = dyn_array_size(ready_queue);
    ProcessControlBlock_t p[size];
    int wt[size];
    int totalwait = 0;
    int totalburst = 0;
    
    clock_t clockstart = clock();
    //read pcb's into an array
    for(int i = 0; i < size; i++){
	dyn_array_extract_front(ready_queue,&p[i]);
    }
    
    //sort pcbs according to length of burst time remaining
    for(int i = 0; i < size; i++){
	int pos = i;
 	for(int j = i + 1; j < size; j++){
		if(p[j].remaining_burst_time < p[pos].remaining_burst_time)
			pos = j;
	}
	
	ProcessControlBlock_t temp = p[i];
	p[i] = p[pos];
	p[pos] = temp;
    }

    //Initial wait time is 0
    wt[0] = 0;
    
    //Assign waiting time for each pcb based on the burst times before it
    for(int i = 1; i < size; i++){
	wt[i] = 0;
	for(int j = 0; j < i; j++){
		wt[i] += p[j].remaining_burst_time;
	}
    }

    //Calculate total wait time and burst time
    for(int i = 0; i < size; i++){
	totalwait += wt[i];
	totalburst += (int)p[i].remaining_burst_time;
    }

    //Record results into result pointer
    result->average_turnaround_time = (float)((totalburst + totalwait) / size);
    result->average_waiting_time = (float)(totalwait / size);
    result->total_run_time = (unsigned long)((clock() - clockstart) / CLOCKS_PER_SEC);
    return true;   
}

bool priority(dyn_array_t *ready_queue, ScheduleResult_t *result) 
{
    UNUSED(ready_queue);
    UNUSED(result);
    return false;   
}

bool round_robin(dyn_array_t *ready_queue, ScheduleResult_t *result, size_t quantum) 
{
    //Resource Allocation
    int i = 0, counter = 0, sum = 0, wt = 0, tat = 0;
    int size = dyn_array_size(ready_queue);
    int num = size;
    ProcessControlBlock_t p[size];
    uint32_t bt[size];

    //Record starting time
    clock_t clockstart = clock();    

    //Extract PCBs
    for(int j = 0; j < size; j++){
    	dyn_array_extract_front(ready_queue,&p[j]);
	bt[j] = p[j].remaining_burst_time;
    }

    //main loop
    while(num != 0){
	//Checks if current process can be finished in given quantum
	if(p[i].remaining_burst_time <= quantum && p[i].remaining_burst_time > 0){
		sum = sum + p[i].remaining_burst_time;
		p[i].remaining_burst_time = 0; 
   		counter = 1;
	}
	//Otherwise decreases burst left and increases time used
	else if(p[i].remaining_burst_time > 0){
		p[i].remaining_burst_time = p[i].remaining_burst_time - quantum;
		sum = sum + quantum;
	}
	//Checks if a process has finished running
	if(p[i].remaining_burst_time == 0 && counter == 1){
		num--;
		wt = wt + sum - p[i].arrival - bt[i];
		tat = tat + sum - p[i].arrival;
		counter = 0;
	}
	//Checks if we are at the end of the list
	if(i == size - 1){
		i = 0;
	}
	//Otherwise increments to next process if it arrived before time sum
	else if((int)p[i].arrival <= sum){
		i++;
	}
	//default behavior
	else{
		i = 0;
	}
    }

    //copies results to result pointer and returns success
    result->average_waiting_time = (float)(wt *1.0/size);
    result->average_turnaround_time = (float)(tat * 1.0 / size);
    result->total_run_time = (unsigned long)((clock() - clockstart) / CLOCKS_PER_SEC);
    return true;
}

dyn_array_t *load_process_control_blocks(const char *input_file) 
{
    unsigned char buffer[sizeof(uint32_t)];
    dyn_array_t * queue;
    FILE * fp = fopen(input_file,"rb");
    uint32_t count;
    
	//printf("Opening File Pointer\n");
    if(fp == NULL){
	return NULL;
    }
	//printf("Opened File\n");

    fread(buffer,sizeof(buffer),1,fp);
 
    memcpy(&count,buffer,4);

    printf("Number of PCBs = %u\n", count);
    queue = dyn_array_create(count,sizeof(buffer),NULL);

    if(queue == NULL)
	return NULL;
    //printf("Queue created\n");

    for(uint32_t i = 0; i < count; i++){
	uint32_t burst;
	uint32_t priority;
  	uint32_t arrival;

	ProcessControlBlock_t * temp = malloc(sizeof(uint32_t)*3+sizeof(bool));
	fread(buffer,sizeof(buffer),1,fp);
	memcpy(&burst,buffer,4);
	//printf("Burst for %i: %u\n",i,burst);
	fread(buffer,sizeof(buffer),1,fp);
	memcpy(&priority,buffer,4);
	//printf("Priority for %i: %u\n",i,priority);
	fread(buffer,sizeof(buffer),1,fp);
	memcpy(&arrival,buffer,4);
	//printf("Arrival for %i: %u\n",i,arrival);

	temp->remaining_burst_time = burst;
	temp->priority = priority;
	temp->arrival = arrival;
	temp->started = false;

	//printf("Pushing onto queue\n");
	dyn_array_push_back(queue,&temp);
    }
    //printf("Copy Finished\n");
    fclose(fp);
    printf("Closed File\n");
    return queue;
}

bool shortest_remaining_time_first(dyn_array_t *ready_queue, ScheduleResult_t *result) 
{
    //Allocate resources
    int wt = 0, tat = 0, counter = 0, shortest;
    int size = dyn_array_size(ready_queue);
    ProcessControlBlock_t p[size];
    int nopr = size;
    int mint = 10000000;
    uint32_t bt[size];

    //Record start time
    clock_t clockstart = clock();

    //Extract PCBs
    for(int i = 0; i < size; i++){
	dyn_array_extract_front(ready_queue,&p[i]);
	bt[i] = p[i].remaining_burst_time;
    }
 
    //main loop
    while(nopr != 0){
	bool check = false;
	
	//Run through all processes to find shortest available one
	for(int i = 0; i < nopr; i++){
		if((int)p[i].arrival <= counter && (int)bt[i] < mint && bt[i] > 0){
			mint = bt[i];
			shortest = i;
			check = true;
		}
	}
	
	//If no processes found increment time passed and restart
	if(!check){
		counter++;
		continue;
	}

	//Decrement process time left
	bt[shortest]--;
	
	//create new shortest minimum time remaining and check if it's done
	mint = bt[shortest];
	if(mint == 0)
		mint = 10000000;

	//check if process is finished
	if(bt[shortest] == 0){
		nopr--;
		check = false;

		wt = wt + counter - p[shortest].arrival - p[shortest].remaining_burst_time;
		tat = tat + counter - p[shortest].arrival;
	}

	//Increment time
	counter++;
    }

    //copies results to result pointer and returns success
    result->average_waiting_time = (float)(wt *1.0/size);
    result->average_turnaround_time = (float)(tat * 1.0 / size);
    result->total_run_time = (unsigned long)((clock() - clockstart) / CLOCKS_PER_SEC);
    
    return true;
}
