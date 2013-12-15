//
//  MOAIHttpTaskCurlThread.cpp
//  libmoai
//
//  Created by Brendan Ragan on 14/12/13.
//
//

#include "MOAIHttpTaskCurlThread.h"

// private const/dest
MOAIHttpTaskCurlThread::MOAIHttpTaskCurlThread():
	mTask(NULL){
}

MOAIHttpTaskCurlThread::~MOAIHttpTaskCurlThread() {
	this->stop();
	this->wait();
}

void MOAIHttpTaskCurlThread::run() {
	if(mTask == NULL)
		return;
	
	RScopedLock l(&mLock);
	mTask->AsyncThreadRunner();
	delete(this);
}

void MOAIHttpTaskCurlThread::setTask(MOAIHttpTaskCurl *task) {
	RScopedLock l(&mLock);
	mTask = task;
}