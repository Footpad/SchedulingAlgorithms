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
	this->setPriority(TP_SCHEDULER);

	//When we first are told to start, start the tasks...
	for(std::size_t i; i < taskSet.size(); i++) {
		taskSet[i]->setState(TP_READY);
		taskSet[i]->start();
		taskSet[i]->setPriority(TP_READY);
	}

//	TraceEvent(_NTO_TRACE_INSERTUSRSTREVENT, _NTO_TRACE_USERFIRST, "Scheduling");
	scheduleTasks();
//	TraceEvent(_NTO_TRACE_INSERTUSRSTREVENT, _NTO_TRACE_USERLAST, "Done Scheduling");
	for (std::size_t i = 0; i < taskSet.size(); i++) {
		Task *task = taskSet[i];
		task->postSem();
	}

	//then perform the scheduling
	while (!killThread) {
		sem_wait(&scheduleSem);
//		TraceEvent(_NTO_TRACE_INSERTUSRSTREVENT, _NTO_TRACE_USERFIRST, "Scheduling");
		scheduleTasks();
//		TraceEvent(_NTO_TRACE_INSERTUSRSTREVENT, _NTO_TRACE_USERLAST, "Done Scheduling");
	}

	return NULL;
}
