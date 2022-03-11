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

    float Average_Waiting_Time
    float Average_Turnaround_Time
    unsigned long Total_Clock_Time
    dyn_array_t * queue
    ScheduleResult_t results

    queue = load_process_control_blocks(argv[0]);

    if(queue == NULL){
	printf("Error Loading PCBS\n");
	return EXIT_FAILURE;
    switch(argv[1]){
	case "FCFS":
		first_come_first_serve(queue,results);
		break;
	case "SJF":
		shortest_job_first(queue,results);
		break;
	case "RR":
		round_robin(queue,results,argv[2]);
		break;
	case "SRTF":
		shortest_remaining_time_first(queue,results);
		break;
	default:
		return EXIT_FAILURE;
		break;
    }

    printf("Average Waiting Time: %d \n", results.average_waiting_time);
    printf("Average Turnaround Time: %d \n", results.average_turnaround_time);
    printf("Total Clock Time: %d \n", results.total_run_time);

    return EXIT_SUCCESS;
}
