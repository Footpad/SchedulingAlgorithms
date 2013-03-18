/*
 * Thread.cpp
 *
 *  Created on: Dec 27, 2012
 *      Author: jeff
 */

#include "Thread.h"

Thread::Thread(std::string n) :
killThread(false),
name(n),
thread(NULL) {}

Thread::~Thread() {}

void Thread::join() {
	pthread_join(thread, NULL);
}

void Thread::start() {
	pthread_create(&thread, NULL, pthread_entry, this);
	pthread_setname_np(thread, name.c_str());
}

void Thread::stop() {
	killThread = true;
}

void* Thread::pthread_entry(void* arg) {
	Thread* threadedObject = (Thread*)arg;
	return threadedObject->run();
}

int Thread::getPriority() {
	int policy;
	struct sched_param params;
	pthread_getschedparam(thread, &policy, &params);
	return params.sched_priority;
}

void Thread::setPriority(int prio) {
	//Get us on a higher priority...
	int policy;
	struct sched_param params;
	pthread_getschedparam(thread, &policy, &params);
	params.sched_priority = prio;
	pthread_setschedparam(thread, policy, &params);
}

std::string Thread::getName() {
	return name;
}
