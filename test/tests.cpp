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
//global total
unsigned int total;

//
class GradeEnvironment : public testing::Environment 
{
    public:
        virtual void SetUp() 
        {
            //TestScore = 20;
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
//FCFS Null dyn_array check
TEST (first_come_first_serve, nullPCB) {
	dyn_array_t* dyn = NULL;
	ScheduleResult_t *sched = new ScheduleResult_t;
	
	bool res = first_come_first_serve (dyn,sched);
	EXPECT_EQ(false,res);
	delete sched;
	score+=1;
}
//FCFS Null schedule check
TEST (first_come_first_serve, nullSchedule) {
	dyn_array_t* dyn = dyn_array_create(0,sizeof(ProcessControlBlock_t),NULL);
	ScheduleResult_t *sched = NULL;
	
	bool res = first_come_first_serve (dyn,sched);
	EXPECT_EQ(false,res);
	dyn_array_destroy(dyn);
	score+=1;
}
//FCFS valid input check
TEST (first_come_first_serve, validInput) {
	ScheduleResult_t *sched = new ScheduleResult_t;
	dyn_array_t* dyn = dyn_array_create(0,sizeof(ProcessControlBlock_t),NULL);
	memset(sched,0,sizeof(ScheduleResult_t));
	ProcessControlBlock_t data[3] = {
			[0] = {24,0,0,false},
			[1] = {3,0,0,false},
			[2] = {3,0,0,false}
	};
	// back loading dyn_array, pull from the back
	dyn_array_push_back(dyn,&data[2]);
	dyn_array_push_back(dyn,&data[1]);
	dyn_array_push_back(dyn,&data[0]);	
	bool res = first_come_first_serve (dyn,sched);	
	ASSERT_EQ(true,res);
	EXPECT_EQ(27,sched->average_waiting_time);
	EXPECT_EQ(17,sched->average_turnaround_time);
	EXPECT_EQ((float) 30,sched->total_run_time);
	dyn_array_destroy(dyn);
	delete sched;

	score+=1;
}



//Round Robin Unit tests
//Round Robin Null array check
TEST (round_robin, nullPCB) {
	ScheduleResult_t *sched = new ScheduleResult_t;
	dyn_array_t* dyn = NULL;
	bool res = round_robin (dyn,sched, QUANTUM);
	EXPECT_EQ(false,res);
	delete sched;

}
//Round Robin null result check
TEST (round_robin, nullSchedule) {
	ScheduleResult_t *sched = NULL;
	dyn_array_t* dyn = dyn_array_create(0,sizeof(ProcessControlBlock_t),NULL);
	bool res = round_robin (dyn,sched, QUANTUM);
	EXPECT_EQ(false,res);
	dyn_array_destroy(dyn);

}
//Round Robin valid input check
TEST (round_robin, validInput) {
	ScheduleResult_t *sched = new ScheduleResult_t;
	dyn_array_t* dyn = dyn_array_create(0,sizeof(ProcessControlBlock_t),NULL);
	memset(sched,0,sizeof(ScheduleResult_t));
	ProcessControlBlock_t data[3] = {
			[0] = {24,0,0,false},
			[1] = {3,0,1,false},
			[2] = {3,0,2,false}
	};
	dyn_array_push_back(dyn,&data[2]);
	dyn_array_push_back(dyn,&data[1]);
	dyn_array_push_back(dyn,&data[0]);	
	bool res = round_robin (dyn,sched, QUANTUM);	
	ASSERT_EQ(true,res);
	EXPECT_EQ(2,sched->average_waiting_time);
	EXPECT_EQ(12,sched->average_turnaround_time);
	EXPECT_EQ((float) 30,sched->total_run_time);
	dyn_array_destroy(dyn);
	delete sched;

	score+=1;
}


//SJF Unit tests
//
TEST (shortest_job_first, nullPCB) {
	dyn_array_t* dyn = NULL;
	ScheduleResult_t *sched = new ScheduleResult_t;
	
	bool res = shortest_job_first (dyn,sched);
	EXPECT_EQ(false,res);
	delete sched;
	score+=1;
}

TEST (shortest_job_first, nullSchedule) {
	dyn_array_t* dyn = dyn_array_create(0,sizeof(ProcessControlBlock_t),NULL);
	ScheduleResult_t *sched = NULL;
	
	bool res = shortest_job_first (dyn,sched);
	EXPECT_EQ(false,res);
	dyn_array_destroy(dyn);
	score+=1;
}

TEST (shortest_job_first, validInput) {
	ScheduleResult_t *sched = new ScheduleResult_t;
	dyn_array_t* dyn = dyn_array_create(0,sizeof(ProcessControlBlock_t),NULL);
	memset(sched,0,sizeof(ScheduleResult_t));
	ProcessControlBlock_t data[4] = {
			[0] = {24,0,0,false},
			[1] = {3,0,0,false},
			[2] = {8,0,0,false},
			[3] = {26,0,0,false}
	};
	dyn_array_push_back(dyn,&data[3]);
	dyn_array_push_back(dyn,&data[2]);
	dyn_array_push_back(dyn,&data[1]);
	dyn_array_push_back(dyn,&data[0]);	
	bool res = shortest_job_first (dyn,sched);	
	ASSERT_EQ(true,res);
	EXPECT_EQ(12,sched->average_waiting_time);
	EXPECT_EQ(27,sched->average_turnaround_time);
	EXPECT_EQ((float) 61,sched->total_run_time);
	dyn_array_destroy(dyn);
	delete sched;

	score+=1;
}

//SRT Unit tests
//SRT
TEST (shortest_remaining_time_first, nullPCB) {
	dyn_array_t* dyn = NULL;
	ScheduleResult_t *sched = new ScheduleResult_t;
	
	bool res = shortest_remaining_time_first (dyn,sched);
	EXPECT_EQ(false,res);
	delete sched;
	score+=1;
}

TEST (shortest_remaining_time_first, nullSched) {
	dyn_array_t* dyn = dyn_array_create(0,sizeof(ProcessControlBlock_t),NULL);
	ScheduleResult_t *sched = NULL;
	
	bool res = shortest_remaining_time_first (dyn,sched);
	EXPECT_EQ(false,res);
	dyn_array_destroy(dyn);
	score+=1;
}

TEST (shortest_remaining_time_first, validInput) {
	ScheduleResult_t *sched = new ScheduleResult_t;
	dyn_array_t* dyn = dyn_array_create(0,sizeof(ProcessControlBlock_t),NULL);
	memset(sched,0,sizeof(ScheduleResult_t));
	ProcessControlBlock_t data[3] = {
			[0] = {7,0,0,false},
			[1] = {3,0,1,false},
			[2] = {4,0,3,false}
	};
	dyn_array_push_back(dyn,&data[2]);
	dyn_array_push_back(dyn,&data[1]);
	dyn_array_push_back(dyn,&data[0]);	
	bool res = shortest_remaining_time_first (dyn,sched);	
	ASSERT_EQ(true,res);
	EXPECT_EQ(3,sched->average_waiting_time);
	EXPECT_EQ(7,sched->average_turnaround_time);
	EXPECT_EQ((float) 24,sched->total_run_time);
	dyn_array_destroy(dyn);
	delete sched;

	score+=1;
}

//PCBlock Loading Unit tests
//Null fiel path check
TEST (load_process_control_blocks, nullPath) {
    
	dyn_array_t* dyn = load_process_control_blocks (NULL);
	ASSERT_EQ(dyn,(dyn_array_t*) NULL);

	score+=1;
}
//File name not found check
TEST (load_process_control_blocks, fileNotFound) {

	dyn_array_t* dyn = load_process_control_blocks ("FileNameIsNotReal.c");
	ASSERT_EQ(dyn,(dyn_array_t*)NULL);

	score+=1;
}
//File with no contents check
TEST (load_process_control_blocks, emptyFile) {
    //Empty file not written to at all
	const char* fname = "FileDoesNotExist.c";
	//Accepted open read and write modes
	mode_t mode = S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP;
	//Flagged file open modes
	int flags = O_CREAT | O_TRUNC | O_WRONLY;
	//File opened, but never written to ensuring a blank file
	int fd = open(fname, flags, mode);
	close(fd);
	dyn_array_t* dyn = load_process_control_blocks (fname);
	ASSERT_EQ(dyn,(dyn_array_t*)NULL);

	score+=1;
}

//Correct file found check
TEST (load_process_control_blocks, CorrectPCBFile) {
    bool result = false;
	const char* fname = "PCB.bin";
	dyn_array_t* dyn = load_process_control_blocks (fname);
	if(dyn != NULL)
	{
	    result = true;
	}
    ASSERT_EQ(result, true);
    ASSERT_EQ(fname, "PCB.bin");
	score+=1;
}
//Invalid file found check
TEST (load_process_control_blocks, incorrectPCBFoundFile) {
	const char* fname = "ThisistheWrongFile.c";
	uint32_t pcb_num = 10;
	uint32_t dyn [5] = {1,2,3,4,5};
	//File open accessible modes
	mode_t mode = S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP;
	//file open flagged modes
	int flags = O_CREAT | O_TRUNC | O_WRONLY;
	int fd = open(fname, flags, mode);
	//Incorrect specifications of PCB file that should ensure a null dyn_array
	write(fd,&pcb_num,sizeof(uint32_t));
	write(fd,&dyn,5 * sizeof(uint32_t));
	close(fd);
	dyn_array_t* dynd = load_process_control_blocks (fname);
	ASSERT_EQ(dynd,(dyn_array_t*)NULL);
	score+=1;
}






int main(int argc, char **argv) 
{
    ::testing::InitGoogleTest(&argc, argv);
    ::testing::AddGlobalTestEnvironment(new GradeEnvironment);
    return RUN_ALL_TESTS();
}
