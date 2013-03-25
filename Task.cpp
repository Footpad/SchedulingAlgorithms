/*
 * Task.cpp
 *
 *  Created on: Mar 13, 2013
 *      Author: dam7633
 */

#include "Task.h"
#include <sys/trace.h>
#include <time.h>
#include <sys/siginfo.h>
#include <cassert>
#include <semaphore.h>
#include <stdlib.h>

// 5 milliseconds
// The timeslice value for which the task time values are based.
#define TIMER_PERIOD_NANO	(5000000)
#define TIMER_PERIOD_SEC	(0)

// Calibrated number of nanoseconds used for the busy-wait
// using nanospin_ns().
#define BUSY_WAIT_NANO (4440000)

Task::Task(std::string n, int c, int p, int d, sem_t* scheduler) :
Thread(n),
computeTime(c),
period(p),
deadline(d),
state(TP_READY),
scheduler(scheduler) {
	// Feasibility assertions.
	assert(computeTime > 0);
	assert(period > 0);
	assert(deadline > 0);

	// produce a thread which calls tick with this as an argument when the timer ticks
	SIGEV_THREAD_INIT(&deadlineEvent, Task::tick, this, NULL);

	// Establish the measurement period
	deadlineTimerSpec.it_interval.tv_nsec = (TIMER_PERIOD_NANO * deadline);
	deadlineTimerSpec.it_interval.tv_sec = TIMER_PERIOD_SEC;
	deadlineTimerSpec.it_value.tv_nsec = (TIMER_PERIOD_NANO * deadline);
	deadlineTimerSpec.it_value.tv_sec = TIMER_PERIOD_SEC;

	// Cache the logging strings.
	deadlineTimerString = name + " deadline timer.";
	deadlineMissedString = name + " missed deadline!";
	deadlineTimerMsg = deadlineTimerString.c_str();
	deadlineMissedMsg = deadlineMissedString.c_str();

	// initialize the semaphore
	sem_init(&doWork, 0, 0);
}

Task::~Task() {
	// Clean up the task, timer, and semaphore.
	pthread_abort(thread);
	sem_destroy(&doWork);
	timer_delete(deadlineTimer);
}

void Task::startDeadlineTimer() {
	// set the timer to get going on ticking
	timer_create(CLOCK_REALTIME, &deadlineEvent, &deadlineTimer);
	timer_settime(deadlineTimer, 0, &deadlineTimerSpec, NULL);
}

void* Task::run() {
	// Build cached strings for logging. on the stack.
	std::string runningString(name + " Running");
	std::string finishedString(name + " Finished");
	const char *runningMsg = runningString.c_str();
#if TRACE_EVENT_LOG_DEBUG
	const char *finishedMsg = finishedString.c_str();
#endif
	completedTime = 0;							// initialize the completedTime

	sem_post(scheduler);						// notify the scheduler that the task is created

	while(!killThread) {
		sem_wait(&doWork);						// Block until the deadline passes so we don't do work
#if TRACE_EVENT_LOG_NORMAL
		TraceEvent(_NTO_TRACE_INSERTUSRSTREVENT, TRACE_EVENT_TASK_START, runningMsg);
#endif
		while(completedTime < computeTime) {
			nanospin_ns(BUSY_WAIT_NANO);		// spin for a time tick
			this->incrementCompletedTime();
			state = TP_RUNNING;					// Spin-lock to burn CPU
		}
		completedTime = 0; 						// reset the completedTime for SCT finished state.
#if TRACE_EVENT_LOG_DEBUG
		TraceEvent(_NTO_TRACE_INSERTUSRSTREVENT, TRACE_EVENT_TASK_END, finishedMsg);
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

long Task::getRemainingDeadline() {
	timer_gettime(deadlineTimer, &deadlineRemainingSpec);
	return (deadlineRemainingSpec.it_value.tv_nsec + (TIMER_PERIOD_NANO/2))/TIMER_PERIOD_NANO;
}

int Task::getPeriod() {
	return period;
}

int Task::getDeadline() {
	return deadline;
}

static int missedDeadlines = 0;
void Task::tick(union sigval sig) {
	//get the schedulers semaphore from the timer...
	Task* self = (Task*) sig.sival_ptr;

#if TRACE_EVENT_LOG_DEBUG
	TraceEvent(_NTO_TRACE_INSERTUSRSTREVENT, TRACE_EVENT_DEADLINE_TIMER, self->deadlineTimerMsg);
#endif

	//if the task hasn't finished since the last deadline...
	if(self->state != TP_FINISHED) {
#if TRACE_EVENT_LOG_CRITICAL
		TraceEvent(_NTO_TRACE_INSERTUSRSTREVENT, TRACE_EVENT_DEADLINE_MISS, self->deadlineMissedMsg);
#endif
		missedDeadlines++;

		if(missedDeadlines > MAX_DEADLINE_MISSES) {
			exit(-1);
		}
	} else {
		if((missedDeadlines > 0))
			missedDeadlines--;
	}

	self->state = TP_READY;

	//post on our semaphore because we can do work again
	sem_post(&self->doWork);
	//post on the schedulers semaphore to so it can schedule tasks again
	sem_post(self->scheduler);
}
