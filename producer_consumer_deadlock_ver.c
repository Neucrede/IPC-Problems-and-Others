#include <stdio.h>
#include <windows.h>

#define	BUFFER_SIZE		2
int		item_count = 0;

HANDLE	hdl_producer, hdl_consumer;

DWORD WINAPI	thread_producer(LPVOID lpParameter)
{
	int		item_count_buffer;
	while (1)
	{
		printf("P: There are %d item(s) in the buffer.\n", item_count);

		item_count_buffer = item_count;

		if (item_count_buffer == BUFFER_SIZE)
		{
			SuspendThread(hdl_producer);
			printf("P: Buffer is full. Sleep.\n");
		}


		item_count++;
		printf("P: Produced an item. Total: %d.\n", item_count);

		item_count_buffer = item_count;

		if (item_count_buffer == 1)
		{
			ResumeThread(hdl_consumer);
			printf("P: Wake up the consumer.\n");
		}
	}

	return	0;
}

DWORD WINAPI	thread_consumer(LPVOID lpParameter)
{
	int	item_count_buffer;

	while (1)
	{
		printf("C: There are %d item(s) in the buffer.\n", item_count);

		item_count_buffer = item_count;
		Sleep(500);
		if (item_count_buffer == 0)
		{
			SuspendThread(hdl_consumer);
			printf("C: There is no item in the buffer. Sleep.\n");
		}


		item_count--;
		printf("C: Consumed one item. %d item(s) left.\n", item_count);

		item_count_buffer = item_count;

		if (item_count_buffer == BUFFER_SIZE - 1)
		{
			ResumeThread(hdl_producer);
			printf("C: Wake up the producer.\n");
		}
	}

	return	0;
}

int	main(int argc, char **argv[])
{
	HANDLE	handles[2];

	hdl_producer = (HANDLE)_beginthreadex(NULL,
			0,
			&thread_producer,
											NULL,
											NULL,
											NULL);
	hdl_consumer = (HANDLE)_beginthreadex(NULL,
											0,
											&thread_consumer,
											NULL,
											NULL,
											NULL);
	handles[0] = hdl_producer;
	handles[1] = hdl_consumer;

	WaitForMultipleObjects(2, handles, TRUE, INFINITE);
	
	return	0;
}


