/*
 * Task.h
 *
 *  Created on: Mar 13, 2013
 *      Author: dam7633
 */

#ifndef TASK_H_
#define TASK_H_

#include <pthread.h>
#include <string>
#include <semaphore.h>

enum TaskPriority {
	TP_FINISHED = 0,
	TP_READY = 30,
	TP_RUNNING = 40,
	TP_SCHEDULER = 50
};

class Task {
public:
	Task(std::string name, int computeTime, int period, int deadline);
	virtual ~Task();

	/**
	 * Join on the underlying thread.
	 */
	void join();

	/**
	 * Creates a pthread which targets Thread's run() method.
	 */
	virtual void start();

	/**
	 * Sets a flag which means the thread should stop.
	 *
	 * Subclassers must implement the stopping behavior in run().
	 */
	virtual void stop();

	/**
	 * @return the return value when the Thread exits.
	 */
	void* run();

	int getPriority();
	void setPriority(int prio);

	int getComputeTime();
	void setComputeTime(int computeTime);

	int getPeriod();
	void setPeriod(int period);

	int getDeadline();
	void setDeadline(int deadline);

protected:
	// Flag which will be set when stop() is called.
	bool killThread;

private:
	/**
	 * Static function which calls the Thread's run() method.
	 */
	static void* pthread_entry(void* args);

	// pthread id for this thread.
	pthread_t thread;

	std::string name;

	int computeTime;

	int period;

	int deadline;
};

#endif /* TASK_H_ */
