/*
 * Task.cpp
 *
 *  Created on: Mar 13, 2013
 *      Author: dam7633
 */

#include "Task.h"
#include <cassert>
#include <semaphore.h>

//1ms deadline and period
#define TIMER_PERIOD_NANO	(5000000)
#define TIMER_PERIOD_SEC	(0)

Task::Task(std::string n, int c, int p, int d, sem_t* scheduler) :
Thread(n),
computeTime(c),
period(p),
deadline(d),
state(TP_READY),
scheduler(scheduler) {
	assert(computeTime > 0);
	assert(period > 0);
	assert(deadline > 0);

	// produce a thread which calls tick with this as an argument when the timer ticks
	SIGEV_THREAD_INIT(&deadlineEvent, Task::tick, this, NULL);

	/* Establish the measurement period */
	deadlineTimerSpec.it_interval.tv_nsec = (TIMER_PERIOD_NANO * deadline);
	deadlineTimerSpec.it_interval.tv_sec = TIMER_PERIOD_SEC;
	deadlineTimerSpec.it_value.tv_nsec = (TIMER_PERIOD_NANO * deadline);
	deadlineTimerSpec.it_value.tv_sec = TIMER_PERIOD_SEC;

	deadlineTimerString = name + " deadline timer.";
	deadlineMissedString = name + " missed deadline!";
	deadlineTimerMsg = deadlineTimerString.c_str();
	deadlineMissedMsg = deadlineMissedString.c_str();

	// initialize the semaphore
	sem_init(&doWork, 0, 0);
}

Task::~Task() {
	sem_destroy(&doWork);
	timer_delete(deadlineTimer);
}

void Task::startDeadlineTimer() {
	// set the timer to get going on ticking
	timer_create(CLOCK_REALTIME, &deadlineEvent, &deadlineTimer);
	timer_settime(deadlineTimer, 0, &deadlineTimerSpec, NULL);
}

void* Task::run() {
	std::string runningString(name + " Running");
	std::string finishedString(name + " Finished");
	const char *runningMsg = runningString.c_str();
	const char *finishedMsg = finishedString.c_str();

	completedTime = 0; 							// initialize the completedTime
	while(!killThread) {
		sem_wait(&doWork);						// Block until the deadline passes so we don't do work
#if TRACE_EVENT_LOG_NORMAL
		TraceEvent(_NTO_TRACE_INSERTUSRSTREVENT, _NTO_TRACE_USERFIRST, runningMsg);
#endif
		while(completedTime < computeTime) {
			//TODO: fix this magic number
			nanospin_ns( 4380000 );				//spin for 5ms
			this->incrementCompletedTime();
			state = TP_RUNNING;					// Spin-lock to burn CPU
		}
		completedTime = 0; 						// reset the completedTime for SCT finished state.
#if TRACE_EVENT_LOG_NORMAL
		TraceEvent(_NTO_TRACE_INSERTUSRSTREVENT, _NTO_TRACE_USERLAST, finishedMsg);
#endif
		state = TP_FINISHED;
		sem_post(scheduler); 					// signal the scheduler it is time to schedule again (because the task finished)
	}
	return NULL;
}

void Task::postSem() {
	//post on it
	sem_post(&doWork);
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

void Task::tick(union sigval sig) {
	//get the schedulers semaphore from the timer...
	Task* self = (Task*) sig.sival_ptr;

#if TRACE_EVENT_LOG_NORMAL
	TraceEvent(_NTO_TRACE_INSERTUSRSTREVENT, _NTO_TRACE_USERFIRST, self->deadlineTimerMsg);
#endif

	//if the task hasn't finished since the last deadline...
	if(self->state != TP_FINISHED) {
#if TRACE_EVENT_LOG_CRITICAL
		TraceEvent(_NTO_TRACE_INSERTUSRSTREVENT, _NTO_TRACE_USERFIRST, self->deadlineMissedMsg);
#endif
	}

	self->state = TP_READY;

	//post on our semaphore because we can do work again
	sem_post(&self->doWork);
	//post on the schedulers semaphore to so it can schedule tasks again
	sem_post(self->scheduler);
}
