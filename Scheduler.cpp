/*
 * Scheduler.cpp
 *
 *  Created on: Mar 13, 2013
 *      Author: dam7633
 */

#include "Scheduler.h"
#include "Task.h"

Scheduler::Scheduler() :
	tickCounter(0) {

	sem_init(&scheduleSem, 0, 0);
}

Scheduler::~Scheduler() {
	// destroy the timer
	timer_delete(tickingTimer);

	// delete all the allocated task objects
	while(taskSet.size() > 0) {
		delete taskSet.back();
		taskSet.pop_back();
	}
}

void Scheduler::createTask(std::string name, int computeTime, int period,
		int deadline) {
	taskSet.push_back(new Task(name, computeTime, period, deadline, &scheduleSem));
}

void* Scheduler::run() {
	this->setPriority(TP_RUNNING);

	// declare the variables the timer will run off of
	struct itimerspec timerSpec;
	struct sigevent event;

	//When we first are told to start, start the tasks...
	for(std::size_t i; i < taskSet.size(); i++) {
		taskSet[i]->start();
		taskSet[i]->postSem();
		taskSet[i]->setState(TP_READY);
	}

	// produce a thread which calls tick with this as an argument when the timer ticks
	SIGEV_THREAD_INIT(&event, Scheduler::tick, this, NULL);

	/* Establish the measurement period */
	timerSpec.it_interval.tv_nsec = TIMER_PERIOD_NANO;
	timerSpec.it_interval.tv_sec = TIMER_PERIOD_SEC;
	timerSpec.it_value.tv_nsec = TIMER_PERIOD_NANO;
	timerSpec.it_value.tv_sec = TIMER_PERIOD_SEC;

	// set the timer to get going on ticking
	timer_create(CLOCK_REALTIME, &event, &tickingTimer);
	timer_settime(tickingTimer, 0, &timerSpec, NULL);

	//then perform the scheduling
	while (!killThread) {
		TraceEvent(_NTO_TRACE_INSERTUSRSTREVENT, _NTO_TRACE_USERFIRST, "Scheduling");
		scheduleTasks();
		TraceEvent(_NTO_TRACE_INSERTUSRSTREVENT, _NTO_TRACE_USERLAST, "Done Scheduling");
		sem_wait(&scheduleSem);
	}

	return NULL;
}

void Scheduler::tick(union sigval sig) {
	Scheduler* self = (Scheduler*) sig.sival_ptr;
	self->tickCounter++;

	bool semPosted = false;

	// To keep this O(N), and not 2N, the loop will post on passed deadline tasks
	// and it will also search for the currently running task to increment its compute time.
	std::size_t vectorSize = self->taskSet.size();
	int runningIdx = -1;
	int highestPriority = -1;
	for (std::size_t i = 0; i < vectorSize; i++) {
		Task *task = self->taskSet[i];
		if ((self->tickCounter % task->getDeadline()) == 0) {
			if (task->getState() != TP_FINISHED) {
				//if the deadline has been met, and the task isnt finished or waiting to
				//execute, then we missed a deadline

				//TODO: report missed deadline here?
				TraceEvent(_NTO_TRACE_INSERTSUSEREVENT, _NTO_TRACE_USERFIRST, self->tickCounter, i);
			}

			semPosted = true;
			task->setState(TP_READY);	//make doubly sure the task is marked ready
			task->postSem();
		}

		//If the task is ready to run or presently running and it has the highest priority,
		//then save that index as we search through the tasks.
		if ((task->getState() == TP_READY || task->getState() == TP_RUNNING)
				&& (task->getPriority() > highestPriority)) {
			highestPriority = task->getPriority();
			runningIdx = i;
		}
	}

	//if the index isn't -1, then there is a running task that is to be incremented.
	if (runningIdx != -1) {
		self->taskSet[runningIdx]->incrementCompletedTime();
	}

	//if a semaphore was posted, its time to schedule.
	if (semPosted) {
		sem_post(&(self->scheduleSem));
	}
}
