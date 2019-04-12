#include <stdio.h>
#include <windows.h>

int		turn;
int		interested[2] = {0};
int		buffer = 0;

HANDLE	handles[2];

typedef DWORD WINAPI (*pThreadFunc)(LPVOID);

void	enter_critical_region(int process)
{
	int	other;

	other = 1 - process;
	interested[process] = 1;
	turn = process;
	printf("%d requests entering.\n", process);

	while ((turn == process) && interested[other] == 1)
	{
	}

	printf("%d entered critical region.\n", process);
}

void	leave_critical_region(int process)
{
	interested[process] = 0;
	printf("%d left critical region.\n", process);
}

DWORD WINAPI	Thread0(LPVOID lpParameter)
{
	while (1)
	{
		enter_critical_region(0);
		Sleep(700);
		buffer = 0;
		printf("From 0: buffer = %d\n", buffer);
		leave_critical_region(0);
		Sleep(600);
	}
}

DWORD WINAPI	Thread1(LPVOID lpParameter)
{
	while (1)
	{
		enter_critical_region(1);
		Sleep(100);
		buffer = 1;
		printf("From 1: buffer = %d\n", buffer);
		leave_critical_region(1);
		Sleep(300);
	}
}

int		main(int argc, char **argv[])
{
	int				i;
	pThreadFunc		threads[2];

	threads[0] = Thread0;
	threads[1] = Thread1;

	for (i = 0; i < 2; i++)
	{
		handles[i] = (HANDLE)_beginthreadex(NULL, 0, threads[i], NULL, 0, NULL);
	}

	WaitForMultipleObjects(2, handles, TRUE, INFINITE);

	return	0;
}

