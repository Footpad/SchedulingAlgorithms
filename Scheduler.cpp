/*
 * Scheduler.cpp
 *
 *  Created on: Mar 13, 2013
 *      Author: dam7633
 */

#include "Scheduler.h"
#include "Task.h"

Scheduler::Scheduler() {
}

Scheduler::~Scheduler() {
}

void Scheduler::createTask(std::string name, int computeTime, int period, int deadline) {

}

void* Scheduler::run() {
	while (!killThread) {
		scheduleTasks();
		sem_wait(&scheduleSem);
	}
}
