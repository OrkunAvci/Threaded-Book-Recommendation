#include "recommendation.h"

double avg_points_read(CPS* reader)
{
	int read_book_number = 0;
	double point_avg_reader = 0.0;

	int i;
	for (i = 0; i < reader->numOfPoints; i++)
	{
		if (reader->points[i] != 0)
		{
			point_avg_reader += (double)reader->points[i];
			read_book_number++;
		}
	}

	point_avg_reader /= read_book_number;
	return point_avg_reader;
}

double sim(CPS* subject, CPS* reader)
{
	double point_avg_subject = 0.0;
	double point_avg_reader = 0.0;

	point_avg_reader = avg_points_read(reader);
	point_avg_subject = avg_points_read(subject);

	int i;
	double sum_top = 0.0;
	double sum_bottom_reader = 0.0;
	double sum_bottom_subject = 0.0;
	for (i = 0; i < reader->numOfPoints; i++)
	{
		if (reader->points[i] != 0 && subject->points[i] != 0)
		{
			sum_top += (((double)reader->points[i] - point_avg_reader)* ((double)subject->points[i] - point_avg_subject));
			sum_bottom_reader += (((double)reader->points[i] - point_avg_reader)* ((double)reader->points[i] - point_avg_reader));
			sum_bottom_subject += (((double)subject->points[i] - point_avg_subject)* ((double)subject->points[i] - point_avg_subject));
		}
	}

	double similarity = sum_top / ((sqrt(sum_bottom_reader))* (sqrt(sum_bottom_subject)));

	return similarity;
}

NODE* get_similars(DC* data, int subjectMarker, int k)
{
	//	Get first k readers into the list.
	int i;
	double similarity;
	NODE* head = NULL;
	NODE* tempCPS;
	for (i = 0; i < k; i++)
	{
		similarity = sim(data->subjects[subjectMarker], data->readers[i]);
		tempCPS = create_node(data->readers[i], similarity);
		head = add(head, tempCPS);
	}

	//	Test the rest of the readers and replace if one with more similarity is found (in replace).
	for (i = k; i < data->readerCount; i++)
	{
		similarity = sim(data->subjects[subjectMarker], data->readers[i]);
		tempCPS = create_node(data->readers[i], similarity);
		head = replace(head, tempCPS);
	}

	//	Return the most similar ones.
	return head;
}

double pred(CPS* subject, NODE* head, int bookMarker)
{
	double top = 0.0;
	double bottom = 0.0;

	NODE* curr = head;
	while (curr != NULL)
	{
		top += (curr->sim * (curr->data->points[bookMarker] - avg_points_read(curr->data)));
		bottom += curr->sim;
		curr = curr->next;
	}

	//	Average points of subject is optional here since it will be the same value added to all predictions and will have no impact on the outcome.
	double prediction = avg_points_read(subject) + (top / bottom);
	return prediction;
}

void recommend(DC* data, int subjectMarker, int numOfSimilars)
{
	if (subjectMarker >= data->subjectCount || subjectMarker < 0 || numOfSimilars <= 0 || numOfSimilars > data->readerCount)
	{
		printf("Input out of bounds. Terminating...\n");
		return;
	}
	else
	{
		system("cls");
		printf("Similar readers to search: %d\n", numOfSimilars);
		printf("Subject is %s\n\n", data->subjects[subjectMarker]->name);
	}

	//	Get similar readers into a linked list.
	NODE* head = get_similars(data, subjectMarker, numOfSimilars);
	output_linked_list(head);

	double prediction;
	double bestPred = -5.0;
	int predIndex = 0;
	int i;
	for (i = 0; i < data->bookCount; i++)
	{
		//	If the book has not been read.
		if (data->subjects[subjectMarker]->points[i] == 0)
		{
			prediction = pred(data->subjects[subjectMarker], head, i);
			printf("%s-> [ %f ]\n", data->books[i], prediction);

			//	Collect best prediction.
			if (prediction > bestPred)
			{
				bestPred = prediction;
				predIndex = i;
			}
		}
	}

	printf("\nFinal recommendation is %s with %f.\n\n", data->books[predIndex], bestPred);

	terminate_linked_list(head);
}
