#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>

#include "datacontainer.h"
#include "dcreader.h"
#include "linkedlist.h"
#include "recommendation.h"

//	Arbitary number.
#define MAX_NUMBER_OF_THREADS 10

typedef struct ThreadData
{
	char* fileName;
	int subjectID;
	int similarsNum;
}ThreadData;

void recommendation_thread(void* arg)
{
	ThreadData* argData = (ThreadData*) arg;
	DC* data = readDC(argData->fileName);
	recommend(data, argData->subjectID, argData->similarsNum);
	free(argData->fileName);
	free(argData);
	pthread_exit(0);
}

int main()
{
	pthread_t  threads[MAX_NUMBER_OF_THREADS];
	
	//	Prepare data:
	ThreadData* arg = (ThreadData*) calloc(1, sizeof(ThreadData));
	arg->fileName = (char*) calloc(50, sizeof(char));
	strncpy(arg->fileName, "RecomendationDataSet.csv", 50);
	arg->similarsNum = 5;
	arg->subjectID = 0;

	//	Proof of concept:
	pthread_create(&(threads[0]), NULL, recommendation_thread, arg);

	pthread_join(threads[0], NULL);

	system("pause");
	return 0;
}
