#include "dcreader.h"

int custom_atoi(char* str)
{
	if (str == NULL) { return 0; }
	int i = 0;
	int num = 0;
	while ('0' <= str[i] && str[i] <= '9')
	{
		num *= 10;
		num += str[i] - '0';
		i++;
	}
	return num;
}

char* parse_string(char* buffer, int* marker)
{
	int i = 0;
	char* created = (char*)calloc(1, sizeof(char));
	while (buffer[*marker + i] != ';' && buffer[*marker + i] != ',' && buffer[*marker + i] != '\n' && buffer[*marker + i] != '\0')
	{
		created = (char*)realloc(created, (i + 1) * sizeof(char));
		created[i] = buffer[*marker + i];
		i++;
	}
	created = (char*)realloc(created, (i + 1) * sizeof(char));
	created[i] = '\0';

	*marker = *marker + i + 1;
	return created;
}

CPS* read_CPS(char* buffer)
{
	CPS* created = (CPS*)calloc(1, sizeof(CPS));
	int marker = 0;
	created->name = parse_string(buffer, &marker);

	//	Unreadable name/line. Also seperates readers from subjects.
	if (created->name[0] == '\0')
	{
		free(created->name);
		free(created);
		return NULL;
	}

	created->numOfPoints = 0;
	created->points = (int*)calloc(1, sizeof(int));
	char* catcher;
	while (buffer[marker] != '\0')
	{
		created->points = (int*)realloc(created->points, (created->numOfPoints + 1) * sizeof(int*));
		catcher = parse_string(buffer, &marker);
		created->points[created->numOfPoints] = custom_atoi(catcher);
		created->numOfPoints++;
		free(catcher);
	}

	return created;
}

DC* readDC(char* fileName)
{
	DC* data = (DC*)calloc(1, sizeof(DC));
	FILE* f = fopen(fileName, "r");
	int bufferSize = 200;
	char* buffer = (char*)calloc(bufferSize, sizeof(char));
	char* token;
	int marker = 0;

	fgets(buffer, bufferSize, f);
	buffer++;		//	This line skips the first empty cell above user names and before book names.

	//	Get Books:
	data->books = (char**)calloc(1, sizeof(char*));
	data->bookCount = 0;
	token = parse_string(buffer, &marker);
	while (buffer[marker] != '\0')
	{
		data->books = (char**)realloc(data->books, (data->bookCount + 1) * sizeof(char*));
		data->books[data->bookCount] = token;
		data->bookCount++;
		token = parse_string(buffer, &marker);
	}
	data->books = (char**)realloc(data->books, (data->bookCount + 1) * sizeof(char*));
	data->books[data->bookCount] = token;
	data->bookCount++;
	marker = 0;

	//	Get Readers:
	data->readers = (CPS**)calloc(1, sizeof(CPS*));
	data->readerCount = 0;
	fgets(buffer, bufferSize, f);
	CPS* tempCPS = read_CPS(buffer);
	while (tempCPS != NULL)
	{
		data->readers = (CPS**)realloc(data->readers, (data->readerCount + 1) * sizeof(CPS*));
		data->readers[data->readerCount] = tempCPS;
		data->readerCount++;
		fgets(buffer, bufferSize, f);
		tempCPS = read_CPS(buffer);
	}

	//	Get Subjects:
	data->subjects = (CPS**)calloc(1, sizeof(CPS*));
	data->subjectCount = 0;
	while (fgets(buffer, bufferSize, f))
	{
		tempCPS = read_CPS(buffer);
		data->subjects = (CPS**)realloc(data->subjects, (data->subjectCount + 1) * sizeof(CPS*));
		data->subjects[data->subjectCount] = tempCPS;
		data->subjectCount++;
	}


	free(fileName);
	free(buffer);
	fclose(f);
	return data;
}

void terminateDC(DC* data)
{
	int i;

	// If readers and subject don't referance the same reader array.
	if (data->readers != data->subjects)
	{
		//	Dealloc subjects:
		for (i = 0; i < data->subjectCount; i++)
		{
			free(data->subjects[i]->name);
			free(data->subjects[i]->points);
			free(data->subjects[i]);
		}
	}

	//	Dealloc readers:
	for (i = 0; i < data->readerCount; i++)
	{
		free(data->readers[i]->name);
		free(data->readers[i]->points);
		free(data->readers[i]);
	}

	//	Dealloc books:
	for (i = 0; i < data->bookCount; i++)
	{
		free(data->books[i]);
	}

	//	Dealloc main data block:
	free(data);
}

void output_dc(DC* data)
{
	int i;
	int j;
	printf("    ");
	for (i = 0; i < data->bookCount; i++)
	{
		printf("%16s", data->books[i]);
	}
	printf("\n");
	for (i = 0; i < data->readerCount; i++)
	{
		printf("%4s", data->readers[i]->name);
		for (j = 0; j < data->readers[i]->numOfPoints; j++)
		{
			printf("%16d", data->readers[i]->points[j]);
		}
		printf("\n");
	}
	printf("\n");
	for (i = 0; i < data->subjectCount; i++)
	{
		printf("%4s", data->subjects[i]->name);
		for (j = 0; j < data->subjects[i]->numOfPoints; j++)
		{
			printf("%16d", data->subjects[i]->points[j]);
		}
		printf("\n");
	}
}
