#include <iostream>
#include <cstdlib>
#include <math.h>
#include <fstream>

using namespace std;

float f(float x, float y, float z){
    return cos(3*x) - 4*y;
}

float g(float x, float y, float z){
    return z;
}

int main(){
    int n = 100000, x0 = 0, y0=0.8, z0=2, xn, yn, zn;
    float h = 0.00002;

    float *k1, *k2, *k3, *k4;
    float *l1, *l2, *l3, *l4;

    float *x, *y, *z;

    k1 = (float*) malloc(n * sizeof (float));
    k2 = (float*) malloc(n * sizeof (float));
    k3 = (float*) malloc(n * sizeof (float));
    k4 = (float*) malloc(n * sizeof (float));

    l1 = (float*) malloc(n * sizeof (float));
    l2 = (float*) malloc(n * sizeof (float));
    l3 = (float*) malloc(n * sizeof (float));
    l4 = (float*) malloc(n * sizeof (float));

    x = (float*) malloc(n * sizeof (float));
    y = (float*) malloc(n * sizeof (float));
    z = (float*) malloc(n * sizeof (float));

    ofstream fout("point.txt");

    x[0] = x0;
    y[0] = y0;
    z[0] = z0;

    cout << "\t" << "X" << "\t\t" << "Y" << "\n";

    #pragma omp parallel num_threads(5)
    {
        #pragma omp for
            for (int i = 0; i < n; i++) {

                l1[i] = g(0, 0, z[i]);
                k1[i] = f(x[i], y[i], z[i]);

                l2[i] = g(0, 0, z[i] + (k1[i] * h / 2.0));
                k2[i] = f(x[i] + h / 2.0, y[i] + (l1[i] * h / 2.0), z[i] + (k1[i] * h / 2.0));

                l3[i] = g(0, 0, z[i] + (k2[i] * h / 2.0));
                k3[i] = f(x[i] + h / 2.0, y[i] + (l2[i] * h / 2.0), z[i] + (k2[i] * h / 2.0));

                l4[i] = g(0, 0, z[i] + k3[i] * h);
                k4[i] = f(x[i] + h, y[i] + l3[i] * h, z[i] + k3[i] * h);

                x[i + 1] = x[i] + h;
                y[i + 1] = y[i] + (k1[i] + 2 * k2[i] + 2 * k3[i] + k4[i]) / 6.0;
                z[i + 1] = z[i] + (l1[i] + 2 * l2[i] + 2 * l3[i] + l4[i]) / 6.0;
            }
    }
    for (int i = 0; i < n; i++){
        fout << x[i] << " ";
    }
    fout << "\n";
    for (int i = 0; i < n; i++){
        fout << y[i] << " ";
    }

    fout << "\n";

    for (int i = 0; i < n; i++) {
        fout << z[i] << " ";
    }

    fout.close();

}