/*
 * Task.cpp
 *
 *  Created on: Mar 13, 2013
 *      Author: dam7633
 */

#include "Task.h"
#include <cassert>
#include <semaphore.h>

Task::Task(std::string n, int c, int p, int d, sem_t* scheduler) :
killThread(false),
thread(NULL),
name(n),
computeTime(c),
period(p),
deadline(d),
state(TP_FINISHED) {
	assert(computeTime > 0);
	assert(period > 0);
	assert(deadline > 0);
	// initialize the semaphore
	sem_init(&doWork, 0, 0);
}

Task::~Task() {
	sem_destroy(&doWork);
}

void Task::join() {
	pthread_join(thread, NULL);
}

void Task::start() {
	pthread_create(&thread, NULL, pthread_entry, this);
	pthread_setname_np(thread, name.c_str());
}

void Task::stop() {
	killThread = true;
}

void* Task::run() {
	while(!killThread) {
		sem_wait(&doWork);						// Block until the deadline passes so we don't do work
		state = TP_READY;
		while(completedTime != computeTime) {
			state = TP_RUNNING;					// Spin-lock to burn CPU
		}
		state = TP_FINISHED;
		completedTime = 0; 						// reset the completedTime and go back to waiting for the next period
		sem_post(scheduler); 					// signal the scheduler it is time to schedule again (because the task finished)
	}
	return NULL;
}

void Task::postSem() {
	int cur;
	int ret;
	ret = sem_getvalue(&doWork, &cur);
	//if the call succeeded and the semaphore is locked
	if( ret == 0 && cur == 0) {
		//post on it
		sem_post(&doWork);
	}
	//otherwise, we don't care because the task already knows it can run again and is in the READY state
}

int Task::getPriority() {
	int policy;
	struct sched_param params;
	pthread_getschedparam(thread, &policy, &params);
	return params.sched_priority;
}

void Task::setPriority(int prio) {
	//Get us on a higher priority...
	int policy;
	struct sched_param params;
	pthread_getschedparam(thread, &policy, &params);
	params.sched_priority = prio;
	pthread_setschedparam(thread, policy, &params);
}

int Task::getState() {
	return state;
}

void Task::setState(int newState) {
	state = newState;
}

void Task::incrementCompletedTime() {
	completedTime += 1;
}

int Task::getRemainingComputeTime() {
	return (computeTime - completedTime);
}

int Task::getComputeTime() {
	return computeTime;
}

void Task::setComputeTime(int c) {
	computeTime = c;
}

int Task::getPeriod() {
	return period;
}

void Task::setPeriod(int p) {
	period = p;
}

int Task::getDeadline() {
	return deadline;
}

void Task::setDeadline(int d) {
	deadline = d;
}

void* Task::pthread_entry(void* arg) {
	Task* threadedObject = (Task*)arg;
	return threadedObject->run();
}
