#include <stdio.h>
#include <conio.h>
#include <Windows.h>
#include <math.h>
#include <time.h>

void setZero(int matrix[8][8]) {
	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 8; j++)
			matrix[i][j] = 0;
	}
}

void show(int matrix[8][8]) {
	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 8; j++)
			printf("%d ", matrix[i][j]);
		printf("\n");
	}
}

int main()
{
	int column = 8, row = 8, times = 1000000;
	int matrix[8][8];
	int secondMatrix[8][8];
	int temp;
	int count = 0;

	double time;
	LARGE_INTEGER timerFrequency, timerStart, timerStop;

	for (int i = 0; i < row; i++)
		for (int j = 0; j < column; j++)
			matrix[i][j] = rand() % 4000;

	printf("Second matrix with zero\n");

	setZero(secondMatrix);

	show(secondMatrix);

//----------------------------------------------------------------------

	QueryPerformanceFrequency(&timerFrequency);
	QueryPerformanceCounter(&timerStart);

	for (int q = 0; q < times; q++) {
		count++;
		for (int i = 0; i < row; i++)
			for (int j = 0; j < column; j++)
				secondMatrix[i][j] = matrix[i][j];
	}

	QueryPerformanceCounter(&timerStop);
	time = (double)(timerStop.QuadPart - timerStart.QuadPart) / (double)timerFrequency.QuadPart;

	printf("\nC : \n");
	printf("time %f, count %d\n", time, count);

	printf("First matrix: \n");
	show(matrix);

	printf("\nSecond matrix: \n");
	show(secondMatrix);

	count = 0;

	printf("\nSecond matrix with zero\n");

	setZero(secondMatrix);

	show(secondMatrix);

//---------------------------------------------------------------------------------

	QueryPerformanceFrequency(&timerFrequency);
	QueryPerformanceCounter(&timerStart);

	_asm {
		pusha

        mov ecx, times
		loop_1 :
		    add count, 1
		    mov temp, ecx
		    xor ebx, ebx
		    mov ecx, row
		    loop_2 :
		        xor eax, eax
			    mov edx, ecx
			    mov ecx, column
			       loop_3 :
		               mov eax, [matrix + ebx]
			           mov[secondMatrix + ebx], eax
			           add ebx, 4
			       loop loop_3
			    mov ecx, edx
		    loop loop_2
	        mov ecx, temp
        loop loop_1
		popa
	}


	QueryPerformanceCounter(&timerStop);
	time = (double)(timerStop.QuadPart - timerStart.QuadPart) / (double)timerFrequency.QuadPart;

	printf("\nAssembler without MMX : \n");
	printf("time %f, count %d\n", time, count);

	printf("First matrix: \n");
	show(matrix);

	printf("\nSecond matrix: \n");
	show(secondMatrix);

	count = 0;

	printf("\nSecond matrix with zero\n");

	setZero(secondMatrix);

	show(secondMatrix);

//---------------------------------------------------------------

	QueryPerformanceFrequency(&timerFrequency);
	QueryPerformanceCounter(&timerStart);


	_asm {
		pusha
		mov ecx, times
		loop_1mmx :
		    add count, 1
		    mov temp, ecx
		    xor esi, esi
		    mov ecx, row
		    loop_2mmx :
		       mov edx, ecx
			   mov ecx, 4 // т.к. обрабатывает сразу по 2 двойн слова 
			   pxor MM0, MM0
			          loop_3mmx :
		                     movq MM0, [matrix + esi]
			                 movq[secondMatrix + esi], MM0
			                 add esi, 8
			          loop loop_3mmx
			   mov ecx, edx
		    loop loop_2mmx
			mov ecx, temp
		loop loop_1mmx
		emms
		popa
	}

	QueryPerformanceCounter(&timerStop);
	time = (double)(timerStop.QuadPart - timerStart.QuadPart) / (double)timerFrequency.QuadPart;

	printf("\nAssembler with MMX : \n");
	printf("time %f, count %d\n", time, count);

	printf("First matrix: \n");
	show(matrix);

	printf("\nSecond matrix: \n");
	show(secondMatrix);
}