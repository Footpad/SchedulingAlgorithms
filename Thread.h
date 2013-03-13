/*
 * Thread.h
 *
 *  Created on: Dec 27, 2012
 *      Author: jeff
 */

#ifndef THREAD_H_
#define THREAD_H_

#include <pthread.h>

class Thread {
public:
	Thread();
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

protected:
	// Flag which will be set when stop() is called.
	bool killThread;

	void setPriority(int prio);

private:
	/**
	 * Static function which calls the Thread's run() method.
	 */
	static void* pthread_entry(void* args);

	// pthread id for this thread.
	pthread_t thread;
};

#endif /* THREAD_H_ */
