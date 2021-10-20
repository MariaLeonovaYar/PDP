#include <iostream>
#include <math.h>

using namespace std;

double func(double x, double y) {
	return sin(x * x - 2 * y);
}

int main() {
	double x0, y0, h, xn, yn;
	double k1, k2, k3, k4;
	int n;

	cout << "Введите x0, y0, h, n\n";

	cin >> x0 >> y0 >> h >> n;

	xn = x0;
	yn = y0;
	cout << "\t" << "k1" << "\t\t" << "k2" << "\t\t" << "k3" << "\t\t" << "k4" << "\t\t" << "yn\n";

	for (int i = 0; i < n; i++) {
		k1 = func(xn, yn);
		k2 = func(xn + h / 2.0, yn + h / 2.0 * k1);
		k3 = func(xn + h / 2.0, yn + h / 2.0 * k2);
		k4 = func(xn + h, yn + h * k3);

		xn = xn + h;
		yn = yn + h / 6.0 * (k1 + 2 * k2 + 2 * k3 + k4);

		cout << "\t" << k1 << "\t\t" << k2 << "\t\t" << k3 << "\t\t" << k4 << "\t\t" << yn << "\n";

	}
}