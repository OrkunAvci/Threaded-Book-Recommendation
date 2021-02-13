#ifndef __DATACONTAINER_H__
#define __DATACONTAINER_H__

typedef struct CustomPersonStruct
{
	int numOfPoints;
	char* name;
	int* points;
}CPS;

//	Data Container only exists so we can pass it to a thread. This is for automation and scaling purposes.
typedef struct DataContainer
{
	int bookCount;
	int readerCount;
	int subjectCount;
	char** books;		//	Names of the books.
	CPS** readers;		//	Readers to calculate predictions from.
	CPS** subjects;		//	Other readers, or subjects, to make predictions to.
}DC;

#endif /*__DATACONTAINER_H__*/
