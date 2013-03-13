/*
 * Task.cpp
 *
 *  Created on: Mar 13, 2013
 *      Author: dam7633
 */

#include "Task.h"
#include <cassert>
#include <semaphore.h>

Task::Task(std::string n, int c, int p, int d) :
killThread(false),
thread(NULL),
name(n),
computeTime(c),
period(p),
deadline(d) {
	assert(computeTime > 0);
	assert(period > 0);
	assert(deadline > 0);
}

Task::~Task() {}

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
	// TODO:

	return NULL;
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
