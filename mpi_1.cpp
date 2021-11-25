#include <stdio.h>
#include <math.h>
#include "mpi.h"

using namespace std;

float sin_integral(float a, float b, int n)
{
	float h = (b - a) / n;
	float integral = 0;

	for (int i = 1; i <= n; i++) {
		integral += sin(a + i * h + h / 2);
	}

	return integral * h;
}

int main(int* argc, char** argv) {
	int rank;
	float int_0, int_1, int_2;
	float sum = 0, buffer;
	MPI_Request request;
	MPI_Status status;

	MPI_Init(argc, &argv);

	MPI_Comm_rank(MPI_COMM_WORLD, &rank);

	//Процесс 0 посылает 1, 1 добавляет свой результат и посылает 2. Итоговый рез-т у 2
	
	if (rank == 0) {
		int_0 = sin_integral(0, 1, 10);
		MPI_Isend(&int_0, 1, MPI_DOUBLE, 1, 0, MPI_COMM_WORLD, &request);
		printf("Процесс %d, вычислил %f, итоговый результат сейчас равен %f\n", rank, int_0, sum);
	}

	if (rank == 1) {
		int_1 = sin_integral(1, 2, 10);
		MPI_Recv(&buffer, 1, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD, &status);
		printf("Получил от 0 число %f\n", buffer);
		sum = int_1 + sum + buffer;
		MPI_Isend(&sum, 1, MPI_DOUBLE, 2, 0, MPI_COMM_WORLD, &request);
		printf("Процесс %d, вычислил %f, итоговый результат сейчас равен %f\n", rank, int_1, sum);
	}

	if (rank == 2) {
		int_2 = sin_integral(2, 3, 10);
		MPI_Recv(&buffer, 1, MPI_DOUBLE, 1, 0, MPI_COMM_WORLD, &status);
		printf("Получил от 1 число %f\n", buffer);
		sum = sum + buffer + int_2;
		printf("Процесс %d, вычислил %f, итоговый результат сейчас равен %f\n", rank, int_2, sum);
	}

	MPI_Finalize();
	return 0;
}