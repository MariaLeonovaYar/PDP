#include<math.h>

float sin_integral(float a, float b, int n)
{
    float h = (b - a) / n;
    float integral = 0;

#pragma omp parallel reduction (+: integral) num_threads(4)
    {
#pragma omp for
        for (int i = 1; i <= n; i++)
        {
            integral += sin(a + i * h + h / 2);
        }
    }
    return integral * h;
}

int main()
{
    sin_integral(0, 1, 100);
    return 0;
}