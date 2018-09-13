/*
 * logQueue.h
 *
 *  Created on: June 5, 2018
 *      Author: j.zh
 */

#ifndef LOG_QUEUE_H
#define LOG_QUEUE_H

#ifdef __cplusplus
extern "C" {
#endif

#include <pthread.h>

typedef struct _Node_ QNode;
typedef struct _Queue_ LogQueue;

struct _Node_ {
    QNode *next;
    QNode *prev;
};

struct _Queue_ {
	QNode node;
	unsigned int count;
    pthread_mutex_t mutex;
} ; 

void LogQueueInit(LogQueue* pQueue);
void LogQueueDeInit(LogQueue* pQueue);

unsigned int LogQueueCount(const LogQueue *pQueue);

void LogQueuePushNode(LogQueue *pQueue, QNode *pNode);
void LogQueuePushNodeHead(LogQueue *pQueue, QNode *pNode);
QNode* LogQueuePopNode(LogQueue *pQueue);


#ifdef __cplusplus
}
#endif

#endif
