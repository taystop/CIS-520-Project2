#include <stdio.h>
#include <stdlib.h>

#include "dyn_array.h"
#include "processing_scheduling.h"

#define FCFS "FCFS"
#define P "P"
#define RR "RR"
#define SJF "SJF"

// Add and comment your analysis code in this function.
// THIS IS NOT FINISHED.
int main(int argc, char **argv) 
{
    if (argc < 3) 
    {
        printf("%s <pcb file> <schedule algorithm> [quantum]\n", argv[0]);
        return EXIT_FAILURE;
    }

    //abort();  // replace me with implementation.

    //Create references for functions to allocate to.
    dyn_array_t * queue;
    ScheduleResult_t results;

    //Load PCBS into a dynamic array
    queue = load_process_control_blocks(argv[0]);

    //Check if loading went through
    if(queue == NULL){
	printf("Error Loading PCBS\n");
	return EXIT_FAILURE;
    }
    //Compare algorithm request and execute algorithm if found
    if(strcmp(argv[1],"FCFS") == 0){
		first_come_first_serve(queue,&results);
    }
    else if(strcmp(argv[1],"SJF") == 0){
		shortest_job_first(queue,&results);
    }
    else if(strcmp(argv[1],"RR") == 0){
		double quantum;
		sscanf(argv[2],"%lf",&quantum);
		round_robin(queue,&results,quantum);
    }
    else if(strcmp(argv[1],"SRTF") == 0){
		shortest_remaining_time_first(queue,&results);
    }
    else{
		return EXIT_FAILURE;
    }

    //Print results to stdout
    printf("Average Waiting Time: %f \n", results.average_waiting_time);
    printf("Average Turnaround Time: %f \n", results.average_turnaround_time);
    printf("Total Clock Time: %ld \n", results.total_run_time);

    return EXIT_SUCCESS;
}
