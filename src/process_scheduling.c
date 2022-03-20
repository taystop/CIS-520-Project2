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
    int size = dyn_array_size(ready_queue);
    ProcessControlBlock_t p[size];
    int wt[size];
    int totalwait = 0;
    int totalburst = 0;
    clock_t clockstart = clock();
    for(int i = 0; i < size; i++){
	dyn_array_extract_front(ready_queue,&p[i]);
    }
    
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

    wt[0] = 0;
    
    for(int i = 1; i < size; i++){
	wt[i] = 0;
	for(int j = 0; j < i; j++){
		wt[i] += p[i].remaining_burst_time;
	}
    }

    for(int i = 0; i < size; i++){
	totalwait += wt[i];
	totalburst += (int)p[i].remaining_burst_time;
    }

    result->average_turnaround_time = (float)((totalburst + totalwait) / size);
    result->average_waiting_time = (float)(totalwait / size);
    result->total_run_time = (unsigned long)((clock() - clockstart) / CLOCKS_PER_SEC);
    return false;   
}

bool priority(dyn_array_t *ready_queue, ScheduleResult_t *result) 
{
    UNUSED(ready_queue);
    UNUSED(result);
    return false;   
}

bool round_robin(dyn_array_t *ready_queue, ScheduleResult_t *result, size_t quantum) 
{
    UNUSED(ready_queue);
    UNUSED(result);
    UNUSED(quantum);
    return false;
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
