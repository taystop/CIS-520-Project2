#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

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
    UNUSED(ready_queue);
    UNUSED(result);
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
    return queue;
}

bool shortest_remaining_time_first(dyn_array_t *ready_queue, ScheduleResult_t *result) 
{
    UNUSED(ready_queue);
    UNUSED(result);
    return false;
}
