#include <iostream>
#include <cstdlib>
#include <math.h>
#include <fstream>

using namespace std;

double f(double x, double y, double z){
    return cos(3*x) - 4*y;
}

double g(double x, double y, double z){
    return z;
}

int main(){
    int n = 100000, x0 = 0, y0=1, z0=1, xn, yn, zn;
    double h = 0.00002;

    double *k1, *k2, *k3, *k4;
    double *l1, *l2, *l3, *l4;
    double *x, *y, *z;

    k1 = (double*) malloc(n * sizeof (double));
    k2 = (double*) malloc(n * sizeof (double));
    k3 = (double*) malloc(n * sizeof (double));
    k4 = (double*) malloc(n * sizeof (double));

    l1 = (double*) malloc(n * sizeof (double));
    l2 = (double*) malloc(n * sizeof (double));
    l3 = (double*) malloc(n * sizeof (double));
    l4 = (double*) malloc(n * sizeof (double));

    x = (double*) malloc(n * sizeof (double));
    y = (double*) malloc(n * sizeof (double));
    z = (double*) malloc(n * sizeof (double));

    ofstream fout("point.txt");

    x[0] = x0;
    y[0] = y0;
    z[0] = z0;

    cout << "\t" << "X" << "\t\t" << "Y" << "\t\t" << "Z\n";

    #pragma omp parallel num_threads(5)
    {
        #pragma omp for
            for (int i = 0; i < n; i++) {

                k1[i] = h * f(x[i], y[i], z[i]);
                l1[i] = h * g(x[i], y[i], z[i]);

                k2[i] = h * f(x[i] + h / 2.0, y[i] + k1[i] / 2.0, z[i] + l1[i] / 2.0);
                l2[i] = h * g(x[i] + h / 2.0, y[i] + k1[i] / 2.0, z[i] + l1[i] / 2.0);

                k3[i] = h * f(x[i] + h / 2.0, y[i] + k2[i] / 2.0, z[i] + l2[i] / 2.0);
                l3[i] = h * g(x[i] + h / 2.0, y[i] + k2[i] / 2.0, z[i] + l2[i] / 2.0);

                k4[i] = h * f(x[i] + h, y[i] + k3[i], z[i] + l3[i]);
                l4[i] = h * g(x[i] + h, y[i] + k3[i], z[i] + l3[i]);

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
    for (int i = 0; i < n; i++){
        fout << z[i] << " ";
    }
    fout << "\n";

    fout.close();

}