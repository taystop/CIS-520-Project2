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

//Total score for all tests
unsigned int TestScore;
//Acchieved score from passing tests
unsigned int score;


class GradeEnvironment : public testing::Environment 
{
    public:
        virtual void SetUp() 
        {
            TestScore = 20;
            score = 0;
        }

        virtual void TearDown()
        {
            ::testing::Test::RecordProperty("points_given", score);
            ::testing::Test::RecordProperty("points_total", TestScore);
            std::cout << "SCORE: " << score << '/' << TestScore << std::endl;
        }
};

//FCFS Unit Tests

TEST (first_come_first_serve, nullInputProcessControlBlockDynArray) {
	dyn_array_t* pcbs = NULL;
	ScheduleResult_t *sr = new ScheduleResult_t;
	
	bool res = first_come_first_serve (pcbs,sr);
	EXPECT_EQ(false,res);
	delete sr;
	score+=1;
}

TEST (first_come_first_serve, nullScheduleResult) {
	dyn_array_t* pcbs = dyn_array_create(0,sizeof(ProcessControlBlock_t),NULL);
	ScheduleResult_t *sr = NULL;
	
	bool res = first_come_first_serve (pcbs,sr);
	EXPECT_EQ(false,res);
	dyn_array_destroy(pcbs);
	score+=1;
}

TEST (first_come_first_serve, validInput) {
	ScheduleResult_t *sr = new ScheduleResult_t;
	dyn_array_t* pcbs = dyn_array_create(0,sizeof(ProcessControlBlock_t),NULL);
	memset(sr,0,sizeof(ScheduleResult_t));
	ProcessControlBlock_t data[3] = {
			[0] = {24,0,0,0},
			[1] = {3,0,1,0},
			[2] = {3,0,2,0}
	};
	// back loading dyn_array, pull from the back
	dyn_array_push_back(pcbs,&data[2]);
	dyn_array_push_back(pcbs,&data[1]);
	dyn_array_push_back(pcbs,&data[0]);	
	bool res = first_come_first_serve (pcbs,sr);	
	ASSERT_EQ(false,res);
	EXPECT_EQ(24,sr->average_waiting_time);
	EXPECT_EQ(26,sr->average_turnaround_time);
	EXPECT_EQ((float) 30,sr->total_run_time);
	dyn_array_destroy(pcbs);
	delete sr;

	score+=1;
}

//Priority Unit tests

TEST (priority, nullInputProcessControlBlockDynArray) {
	dyn_array_t* pcbs = NULL;
	ScheduleResult_t *sr = new ScheduleResult_t;
	
	bool res = priority(pcbs,sr);
	EXPECT_EQ(false,res);
	delete sr;
	score+=1;
}

TEST (priority, nullScheduleResult) {
	dyn_array_t* pcbs = dyn_array_create(0,sizeof(ProcessControlBlock_t),NULL);
	ScheduleResult_t *sr = NULL;
	
	bool res = priority (pcbs,sr);
	EXPECT_EQ(false,res);
	dyn_array_destroy(pcbs);
	score+=1;
}

TEST (priority, validInput) {
	ScheduleResult_t *sr = new ScheduleResult_t;
	dyn_array_t* pcbs = dyn_array_create(0,sizeof(ProcessControlBlock_t),NULL);
	memset(sr,0,sizeof(ScheduleResult_t));
	ProcessControlBlock_t data[5] = {
			[0] = {4,1,0,0},
			[1] = {3,2,0,0},
			[2] = {7,1,6,0},
			[3] = {4,3,11,0},
			[4] = {2,2,12,0}
	};
	// back loading dyn_array, pull from the back
	dyn_array_push_back(pcbs,&data[4]);
	dyn_array_push_back(pcbs,&data[3]);
	dyn_array_push_back(pcbs,&data[2]);
	dyn_array_push_back(pcbs,&data[1]);
	dyn_array_push_back(pcbs,&data[0]);	
	bool res = priority (pcbs,sr);	
	ASSERT_EQ(true,res);
	EXPECT_EQ(3.6,sr->average_waiting_time);
	EXPECT_EQ(7.6,sr->average_turnaround_time);
	EXPECT_EQ((float) 20,sr->total_run_time);
	dyn_array_destroy(pcbs);
	delete sr;

	score+=1;
}

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

TEST (round_robin, goodInputA) {
	ScheduleResult_t *sr = new ScheduleResult_t;
	dyn_array_t* pcbs = dyn_array_create(0,sizeof(ProcessControlBlock_t),NULL);
	memset(sr,0,sizeof(ScheduleResult_t));
	ProcessControlBlock_t data[3] = {
			[0] = {24,0,0,0},
			[1] = {3,0,1,0},
			[2] = {3,0,2,0}
	};
	// back loading dyn_array, pull from the back
	dyn_array_push_back(pcbs,&data[2]);
	dyn_array_push_back(pcbs,&data[1]);
	dyn_array_push_back(pcbs,&data[0]);	
	bool res = round_robin (pcbs,sr, QUANTUM);	
	ASSERT_EQ(true,res);
	EXPECT_EQ(24,sr->average_waiting_time);
	EXPECT_EQ(26,sr->average_turnaround_time);
	EXPECT_EQ((float) 30,sr->total_run_time);
	dyn_array_destroy(pcbs);
	delete sr;

	score+=1;
}


//SJF Unit tests

TEST (shortest_job_first, nullInputProcessControlBlockDynArray) {
	dyn_array_t* pcbs = NULL;
	ScheduleResult_t *sr = new ScheduleResult_t;
	
	bool res = shortest_job_first (pcbs,sr);
	EXPECT_EQ(false,res);
	delete sr;
	score+=1;
}

TEST (shortest_job_first, nullScheduleResult) {
	dyn_array_t* pcbs = dyn_array_create(0,sizeof(ProcessControlBlock_t),NULL);
	ScheduleResult_t *sr = NULL;
	
	bool res = shortest_job_first (pcbs,sr);
	EXPECT_EQ(false,res);
	dyn_array_destroy(pcbs);
	score+=1;
}

TEST (shortest_job_first, goodInputA) {
	ScheduleResult_t *sr = new ScheduleResult_t;
	dyn_array_t* pcbs = dyn_array_create(0,sizeof(ProcessControlBlock_t),NULL);
	memset(sr,0,sizeof(ScheduleResult_t));
	ProcessControlBlock_t data[4] = {
			[0] = {24,0,0,0},
			[1] = {3,0,0,0},
			[2] = {8,0,0,0},
			[3] = {26,0,0,0}
	};
	dyn_array_push_back(pcbs,&data[3]);
	dyn_array_push_back(pcbs,&data[2]);
	dyn_array_push_back(pcbs,&data[1]);
	dyn_array_push_back(pcbs,&data[0]);	
	bool res = shortest_job_first (pcbs,sr);	
	ASSERT_EQ(true,res);
	EXPECT_EQ(12.25,sr->average_waiting_time);
	EXPECT_EQ(27.5,sr->average_turnaround_time);
	EXPECT_EQ((float) 61,sr->total_run_time);
	dyn_array_destroy(pcbs);
	delete sr;

	score+=1;
}

//SRT Unit tests

TEST (shortest_remaining_time_first, nullInputProcessControlBlockDynArray) {
	dyn_array_t* pcbs = NULL;
	ScheduleResult_t *sr = new ScheduleResult_t;
	
	bool res = shortest_remaining_time_first (pcbs,sr);
	EXPECT_EQ(false,res);
	delete sr;
	score+=1;
}

TEST (shortest_remaining_time_first, nullScheduleResult) {
	dyn_array_t* pcbs = dyn_array_create(0,sizeof(ProcessControlBlock_t),NULL);
	ScheduleResult_t *sr = NULL;
	
	bool res = shortest_remaining_time_first (pcbs,sr);
	EXPECT_EQ(false,res);
	dyn_array_destroy(pcbs);
	score+=1;
}

TEST (shortest_remaining_time_first, goodInputA) {
	ScheduleResult_t *sr = new ScheduleResult_t;
	dyn_array_t* pcbs = dyn_array_create(0,sizeof(ProcessControlBlock_t),NULL);
	memset(sr,0,sizeof(ScheduleResult_t));
	ProcessControlBlock_t data[3] = {
			[0] = {7,0,0,0},
			[1] = {3,0,1,0},
			[2] = {4,0,3,0}
	};
	// back loading dyn_array, pull from the back
	dyn_array_push_back(pcbs,&data[2]);
	dyn_array_push_back(pcbs,&data[1]);
	dyn_array_push_back(pcbs,&data[0]);	
	bool res = shortest_remaining_time_first (pcbs,sr);	
	ASSERT_EQ(true,res);
	EXPECT_EQ(2.66,sr->average_waiting_time);
	EXPECT_EQ(7.33,sr->average_turnaround_time);
	EXPECT_EQ((float) 24,sr->total_run_time);
	dyn_array_destroy(pcbs);
	delete sr;

	score+=1;
}

//PCBlock Loading Unit tests

TEST (load_process_control_blocks, nullFilePath) {
    
	dyn_array_t* da = load_process_control_blocks (NULL);
	ASSERT_EQ(da,(dyn_array_t*) NULL);

	score+=1;
}

TEST (load_process_control_blocks, notFoundFile) {

	dyn_array_t* da = load_process_control_blocks ("FileNameIsNotReal.c");
	ASSERT_EQ(da,(dyn_array_t*)NULL);

	score+=1;
}

TEST (load_process_control_blocks, emptyFoundFile) {
    //Empty file not written to at all
	const char* fname = "FileDoesNotExist.c";
	//Accepted open read and write modes
	mode_t mode = S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP;
	//Flagged file open modes
	int flags = O_CREAT | O_TRUNC | O_WRONLY;
	//File opened, but never written to ensuring a blank file
	int fd = open(fname, flags, mode);
	close(fd);
	dyn_array_t* da = load_process_control_blocks (fname);
	ASSERT_EQ(da,(dyn_array_t*)NULL);

	score+=1;
}

TEST (load_process_control_blocks, incorrectPCBFoundFile) {
	const char* fname = "ThisistheWrongFile.c";
	uint32_t pcb_num = 10;
	uint32_t pcbs[5] = {1,2,3,4,5};
	//File open accessible modes
	mode_t mode = S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP;
	//file open flagged modes
	int flags = O_CREAT | O_TRUNC | O_WRONLY;
	int fd = open(fname, flags, mode);
	//Incorrect specifications of PCB file that should ensure a null dyn_array
	write(fd,&pcb_num,sizeof(uint32_t));
	write(fd,&pcbs,5 * sizeof(uint32_t));
	close(fd);
	dyn_array_t* da = load_process_control_blocks (fname);
	ASSERT_EQ(da,(dyn_array_t*)NULL);
	score+=1;
}

TEST (load_process_control_blocks, CorrectPCBFile) {
    bool result = false;
	const char* fname = "PCB.bin";
	dyn_array_t* da = load_process_control_blocks (fname);
	if(da != NULL)
	{
	    result = true;
	}
    ASSERT_EQ(result, true);
    ASSERT_EQ(fname, "PCB.bin");
	score+=1;
}





int main(int argc, char **argv) 
{
    ::testing::InitGoogleTest(&argc, argv);
    ::testing::AddGlobalTestEnvironment(new GradeEnvironment);
    return RUN_ALL_TESTS();
}
