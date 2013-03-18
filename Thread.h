/*
 * Thread.h
 *
 *  Created on: Dec 27, 2012
 *      Author: jeff
 */

#ifndef THREAD_H_
#define THREAD_H_

#include <pthread.h>
#include <string>

class Thread {
public:
	Thread(std::string name);
	virtual ~Thread();

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
	 * Subclassers implement the run loop of the thread in this method.
	 *
	 * @return the return value when the Thread exits.
	 */
	virtual void* run() = 0;

	/**
	 * Returns the priority of the thread.
	 *
	 * @return an int representing the priority of the thread
	 */
	int getPriority();

	/**
	 * Sets the priority of the thread.
	 *
	 * @param prio - the new priority to be set
	 */
	void setPriority(int prio);

	/**
	 * @return the name used by the underlying pthread
	 */
	std::string getName();

protected:
	// Flag which will be set when stop() is called.
	bool killThread;

	/**
	 * The name of the task; reflected as the name of the underlying thread.
	 */
	std::string name;

private:
	/**
	 * Static function which calls the Thread's run() method.
	 */
	static void* pthread_entry(void* args);

	/**
	 * pthread id for this thread.
	 */
	pthread_t thread;
};

#endif /* THREAD_H_ */
