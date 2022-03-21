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

    printf("Loading PCBs\n");

    //Load PCBS into a dynamic array
    queue = load_process_control_blocks(argv[1]);

    //Check if loading went through
    if(queue == NULL){
	printf("Error Loading PCBS\n");
	return EXIT_FAILURE;
    }

    char fcfs[] = "FCFS", sjf[] = "SJF", rr[] = "RR", srtf[] = "SRTF";
    printf("Loading Complete\n\n");
    //Compare algorithm request and execute algorithm if found
    if(strcmp(argv[2],fcfs) == 0){
	printf("Starting First Come First Serve Algorithm\n");	
	first_come_first_serve(queue,&results);
    }
    else if(strcmp(argv[2],sjf) == 0){
	printf("Starting Shortest Job First Algorithm\n");
	shortest_job_first(queue,&results);
    }
    else if(strcmp(argv[2],rr) == 0){
	printf("Starting Round Robin Algorithm\n");
	double quantum;
	sscanf(argv[3],"%lf",&quantum);
	round_robin(queue,&results,quantum);
    }
    else if(strcmp(argv[2],srtf) == 0){
	printf("Starting Shortest Remaining Time First Algorithm\n");
	shortest_remaining_time_first(queue,&results);
    }
    else{
	printf("Error selecting algorithm\n");
	return EXIT_FAILURE;
    }

    //Print results to stdout
    printf("Average Waiting Time: %f \n", results.average_waiting_time);
    printf("Average Turnaround Time: %f \n", results.average_turnaround_time);
    printf("Total Clock Time: %ld \n", results.total_run_time);

    free(queue);

    return EXIT_SUCCESS;
}
