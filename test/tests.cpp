#include <fcntl.h>
#include <stdio.h>
#include "gtest/gtest.h"
#include <pthread.h>
#include "../include/processing_scheduling.h"

// Using a C library requires extern "C" to prevent function managling
extern "C" 
{
#include <dyn_array.h>
}


#define NUM_PCB 30
#define QUANTUM 5 // Used for Robin Round for process as the run time limit

unsigned int score;
unsigned int total;

class GradeEnvironment : public testing::Environment 
{
    public:
        virtual void SetUp() 
        {
            score = 0;
            total = 210;
        }

        virtual void TearDown()
        {
            ::testing::Test::RecordProperty("points_given", score);
            ::testing::Test::RecordProperty("points_total", total);
            std::cout << "SCORE: " << score << '/' << total << std::endl;
        }
};

//FCFS Unit Tests

TEST (first_come_first_serve, nullInputProcessControlBlockDynArray) {
	ScheduleResult_t *sr = new ScheduleResult_t;
	dyn_array_t* pcbs = NULL;
	bool res = first_come_first_serve (pcbs,sr);
	EXPECT_EQ(false,res);
	delete sr;
}

TEST (first_come_first_serve, nullScheduleResult) {
	ScheduleResult_t *sr = NULL;
	dyn_array_t* pcbs = dyn_array_create(0,sizeof(ProcessControlBlock_t),NULL);
	bool res = first_come_first_serve (pcbs,sr);
	EXPECT_EQ(false,res);
	dyn_array_destroy(pcbs);

}

TEST (first_come_first_serve, goodInputA) {
	//init_lock();
	ScheduleResult_t *sr = new ScheduleResult_t;
	dyn_array_t* pcbs = dyn_array_create(0,sizeof(ProcessControlBlock_t),NULL);
	memset(sr,0,sizeof(ScheduleResult_t));
	// add PCBs now
	ProcessControlBlock_t data[3] = {
			[0] = {24,0,2,0},
			[1] = {3,0,1,1},
			[2] = {3,0,3,1}
	};
	// back loading dyn_array, pull from the back
	dyn_array_push_back(pcbs,&data[2]);
	dyn_array_push_back(pcbs,&data[1]);
	dyn_array_push_back(pcbs,&data[0]);	
	bool res = first_come_first_serve (pcbs,sr);	
	ASSERT_EQ(true,res);
	float answers[3] = {27,17,30};
	EXPECT_EQ(answers[0],sr->average_waiting_time);
	EXPECT_EQ(answers[1],sr->average_turnaround_time);
	EXPECT_EQ(answers[2],sr->total_run_time);
	dyn_array_destroy(pcbs);
	delete sr;

	score+=5;
}

//Priority Unit tests

//Round Robin Unit tests

TEST (round_robin, nullInputProcessControlBlockDynArray) {
	ScheduleResult_t *sr = new ScheduleResult_t;
	dyn_array_t* pcbs = NULL;
	bool res = round_robin (pcbs,sr, QUANTUM);
	EXPECT_EQ(false,res);
	delete sr;

}

TEST (round_robin, nullScheduleResult) {
	ScheduleResult_t *sr = NULL;
	dyn_array_t* pcbs = dyn_array_create(0,sizeof(ProcessControlBlock_t),NULL);
	bool res = round_robin (pcbs,sr, QUANTUM);
	EXPECT_EQ(false,res);
	dyn_array_destroy(pcbs);

}


//SJF Unit tests

//SRT Unit tests

//PCBlock Loading Unit tests

TEST (load_process_control_blocks, nullFilePath) {
	dyn_array_t* da = load_process_control_blocks (NULL);
	ASSERT_EQ(da,(dyn_array_t*) NULL);

	score+=5;
}

TEST (load_process_control_blocks, notFoundFile) {

	dyn_array_t* da = load_process_control_blocks ("NotARealFile.Awesome");
	ASSERT_EQ(da,(dyn_array_t*)NULL);

	score+=5;
}

TEST (load_process_control_blocks, emptyFoundFile) {
	const char* fname = "EMPTYFILE.DARN";
	mode_t mode = S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP;
	int flags = O_CREAT | O_TRUNC | O_WRONLY;
	int fd = open(fname, flags, mode);
	close(fd);
	dyn_array_t* da = load_process_control_blocks (fname);
	ASSERT_EQ(da,(dyn_array_t*)NULL);

	score+=5;
}

TEST (load_process_control_blocks, incorrectPCBFoundFile) {
	const char* fname = "CANYOUHANDLETHE.TRUTH";
	uint32_t pcb_num = 10;
	uint32_t pcbs[5] = {1,2,3,4,5};
	mode_t mode = S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP;
	int flags = O_CREAT | O_TRUNC | O_WRONLY;
	int fd = open(fname, flags, mode);
	write(fd,&pcb_num,sizeof(uint32_t));
	write(fd,&pcbs,5 * sizeof(uint32_t));
	close(fd);
	dyn_array_t* da = load_process_control_blocks (fname);
	ASSERT_EQ(da,(dyn_array_t*)NULL);

	score+=5;
}




int main(int argc, char **argv) 
{
    ::testing::InitGoogleTest(&argc, argv);
    ::testing::AddGlobalTestEnvironment(new GradeEnvironment);
    return RUN_ALL_TESTS();
}
