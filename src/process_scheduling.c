#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <time.h>

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
    UNUSED(ready_queue);
    UNUSED(result);
    return false;
}

bool shortest_job_first(dyn_array_t *ready_queue, ScheduleResult_t *result) 
{
    //variable allocation
    int size = dyn_array_size(ready_queue);
    ProcessControlBlock_t p[size];
    int wt[size];
    int totalwait = 0;
    int totalburst = 0;
    //get initial clock time
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
    
    if(fp == NULL){
	return NULL;
    }

    fread(buffer,sizeof(buffer),1,fp);
 
    memcpy(&count,buffer,4);
    
    queue = dyn_array_create(count,sizeof(buffer),NULL);

    if(queue == NULL)
	return NULL;

    for(uint32_t i = 0; i < count; i = i + 3){
	uint32_t burst;
	uint32_t priority;
  	uint32_t arrival;

	ProcessControlBlock_t * temp = malloc(sizeof(ProcessControlBlock_t));
	fread(buffer,sizeof(buffer),1,fp);
	memcpy(&burst,buffer,4);
	fread(buffer,sizeof(buffer),1,fp);
	memcpy(&priority,buffer,4);
	fread(buffer,sizeof(buffer),1,fp);
	memcpy(&arrival,buffer,4);

	temp->remaining_burst_time = burst;
	temp->priority = priority;
	temp->arrival = arrival;
	temp->started = false;

	dyn_array_push_back(queue,&temp);
    }
    fclose(fp);
    return queue;
}

bool shortest_remaining_time_first(dyn_array_t *ready_queue, ScheduleResult_t *result) 
{
    UNUSED(ready_queue);
    UNUSED(result);
    return false;
}
