// Интегрирование обходной траектории полета к Луне (облет Луны)
#include "stdafx.h"
#include <iostream>
#include <cmath>
#include <iomanip>
#include <fstream>
#include <vector>
#define _USE_MATH_DEFINES
#include "math.h"
#include "MoonLocation.h"
#define Mu_Earth 398600E9
#define R_eq_Earth 6378245
#define Mu_Moon 4902.8E9
#define R_Moonsphere 66E6  // Радиус сферы влияния Луны
using namespace std;
ofstream txtfile("1.txt");

#define dtdt  K[0][j]
#define dxdt  K[1][j]
#define dydt  K[2][j]
#define dzdt  K[3][j]
#define dVxdt K[4][j]
#define dVydt K[5][j]
#define dVzdt K[6][j]

#define t  par[0]
#define x  par[1]
#define y  par[2]
#define z  par[3]
#define Vx par[4]
#define Vy par[5]
#define Vz par[6]

#define t_RK  parbuf[0]
#define x_RK  parbuf[1]
#define y_RK  parbuf[2]
#define z_RK  parbuf[3]
#define Vx_RK parbuf[4]
#define Vy_RK parbuf[5]
#define Vz_RK parbuf[6]

double ddt[100], par[100], parbuf[100];

int main()
{
	setlocale(LC_ALL, "ru");
	double t_a = 2458584.0416667;
	vector <double> r_EM_ = { 55582.3189093754E3, 351830.785915383E3, 134159.042227681E3 },
		v_EM_ = { -1.021763142861E3, 0.071224541640291E3, 0.117894309048765E3 };           // Истинное ПОЛОЖЕНИЕ ЛУНЫ НА 10.04.2019 13:00:00 (эфемериды)
	double t_FL = 58.839 * 3600.0;
	double dt = 10, r, r_buf,rm,rm_buf;
	double a[100] = { 0.0,0.5,0.5,1.0 }, K[100][100] = { 0.0 }; int k = 0, step;
	double x1, y1, z1, Vx1, Vy1, Vz1;
	t = -t_FL, x = 827.418E3, y = -5974.36E3, z = -2590.08E3, Vx = 10.3637E3, Vy = 2.50018E3, Vz = -2.45625E3; // НАЧАЛЬНЫЕ УСЛОВИЯ
	r = pow(x*x + y*y + z*z, 0.5);
	txtfile << setw(20) << "t, ч" << setw(20) << "x, km" << setw(20) << "y, km" << setw(20) << "z, km" << setw(20) << "Vx, km/s" << setw(20) << "Vy, km/s" << setw(20) << "Vz, km/s" 
		<< setw(20) << "x_m, km" << setw(20) << "y_m, km" << setw(20) << "z_m, km" << setw(20) << "Vx_m, km/s" << setw(20) << "Vy_m, km/s" << setw(20) << "Vz_m, km/s" << endl;
	
	// ЭЛЛИПТИЧЕСКАЯ ОРБИТА 1
	bool Moon_peri = false; r_buf = 1E40; rm_buf = 1E40;
	for (;;) {
		// Пересчет в лунную СК J2000 
		MoonLocation(t_a + t / (3600 * 24), &r_EM_, &v_EM_);
		x1 = x - r_EM_[0];
		y1 = y - r_EM_[1];
		z1 = z - r_EM_[2];
		Vx1 = Vx - v_EM_[0];
		Vy1 = Vy - v_EM_[1];
		Vz1 = Vz - v_EM_[2];
		r = pow(x*x + y*y + z*z, 0.5);
		rm = pow(x1*x1 + y1*y1 + z1*z1, 0.5);

		if ((Moon_peri == false) && (rm_buf < rm)) {						//Запись момента достижения периселения
			MoonLocation(t_a + t / (3600 * 24), &r_EM_, &v_EM_);
			x1 = x - r_EM_[0];
			y1 = y - r_EM_[1];
			z1 = z - r_EM_[2];
			Vx1 = Vx - v_EM_[0];
			Vy1 = Vy - v_EM_[1];
			Vz1 = Vz - v_EM_[2];
			r = pow(x*x + y*y + z*z, 0.5);
			rm = pow(x1*x1 + y1*y1 + z1*z1, 0.5);
			txtfile << setw(20) << t / 3600.0
				<< setw(20) << x*1E-3 << setw(20) << y*1E-3 << setw(20) << z*1E-3
				<< setw(20) << Vx*1E-3 << setw(20) << Vy*1E-3 << setw(20) << Vz*1E-3
				<< setw(20) << x1*1E-3 << setw(20) << y1*1E-3 << setw(20) << z1*1E-3
				<< setw(20) << Vx1*1E-3 << setw(20) << Vy1*1E-3 << setw(20) << Vz1*1E-3 << endl;
			cout << setw(20) << t / 3600.0 << setw(20) << rm*1E-3 << setw(20) << "ПЕРИСЕЛЕНИЙ" << endl;
			Moon_peri = true;
		}
		rm_buf = rm;

		if ((r_buf < r) && (t > 20 * 3600)) {		//Условие выхода
			cout << "END" << endl;
			MoonLocation(t_a + t / (3600 * 24), &r_EM_, &v_EM_);
			x1 = x - r_EM_[0];
			y1 = y - r_EM_[1];
			z1 = z - r_EM_[2];
			Vx1 = Vx - v_EM_[0];
			Vy1 = Vy - v_EM_[1];
			Vz1 = Vz - v_EM_[2];
			r = pow(x*x + y*y + z*z, 0.5);
			rm = pow(x1*x1 + y1*y1 + z1*z1, 0.5);
			txtfile << setw(20) << t / 3600.0
				<< setw(20) << x*1E-3 << setw(20) << y*1E-3 << setw(20) << z*1E-3
				<< setw(20) << Vx*1E-3 << setw(20) << Vy*1E-3 << setw(20) << Vz*1E-3
				<< setw(20) << x1*1E-3 << setw(20) << y1*1E-3 << setw(20) << z1*1E-3
				<< setw(20) << Vx1*1E-3 << setw(20) << Vy1*1E-3 << setw(20) << Vz1*1E-3 << endl;
			cout << setw(20) << t / 3600.0 << setw(20) << r*1E-3 << endl;
			break;
		}
		r_buf = r;		

		// Вывод в файл
		step = 3600 / dt;
		if (k % step == 0) {
			txtfile << setw(20) << t / 3600.0 
				<< setw(20)	<< x*1E-3 << setw(20) << y*1E-3 << setw(20) << z*1E-3
				<< setw(20) << Vx*1E-3 << setw(20) << Vy*1E-3 << setw(20) << Vz*1E-3
				<< setw(20) << x1*1E-3 << setw(20) << y1*1E-3 << setw(20) << z1*1E-3
				<< setw(20) << Vx1*1E-3 << setw(20) << Vy1*1E-3 << setw(20) << Vz1*1E-3 << endl;
			cout << setw(20) << t / 3600.0 << setw(20) << rm*1E-3 << setw(20) << r*1E-3 << endl;
		}
		k++;

		//Цикл расчета коэффициентов
		for (int j = 1; j <= 4; j++) {
			for (int i = 0; i <= 6; i++)  parbuf[i] = par[i] + K[i][j - 1] * a[j - 1]; // Считаем новые аргументы

				//Производные (коэффициенты K[i][j])
				// Пересчет в лунную СК J2000 
			MoonLocation(t_a + t_RK / (3600 * 24), &r_EM_, &v_EM_);
			x1 = x_RK - r_EM_[0];
			y1 = y_RK - r_EM_[1];
			z1 = z_RK - r_EM_[2];
			Vx1 = Vx_RK - v_EM_[0];
			Vy1 = Vy_RK - v_EM_[1];
			Vz1 = Vz_RK - v_EM_[2];
			r = pow(x_RK*x_RK + y_RK*y_RK + z_RK*z_RK, 0.5),
			rm = pow(x1*x1 + y1*y1 + z1*z1, 0.5) ,
				dVxdt = (-Mu_Earth*x_RK / (r*r*r) - Mu_Moon*x1 / (rm*rm*rm))*dt,
				dVydt = (-Mu_Earth*y_RK / (r*r*r) - Mu_Moon*y1/ (rm*rm*rm))*dt,
				dVzdt = (-Mu_Earth*z_RK / (r*r*r) - Mu_Moon*z1 / (rm*rm*rm))*dt,
				dxdt = Vx_RK*dt,
				dydt = Vy_RK*dt,
				dzdt = Vz_RK*dt,
				dtdt = 1 * dt;
		}

		// Приращения
			for (int i = 0; i <= 6; i++) {
				double b[5] = { 0.0, 1.0 / 6.0, 2.0 / 6.0, 2.0 / 6.0, 1.0 / 6.0 };
				for (int j = 1; j <= 4; j++) par[i] += K[i][j] * b[j];
			}

	}

/*	// ГИПЕРБОЛИЧЕСКАЯ ОРБИТА
	bool Moon_peri = false; r_buf = 1E40;
	x -= r_EM_[0], y -= r_EM_[1], z -= r_EM_[2],
		Vx -= v_EM_[0], Vy -= v_EM_[1], Vz -= v_EM_[2];

	for (;;) {
		// Пересчет в земную СК J2000 
		MoonLocation(t_a + t / (3600 * 24), &r_EM_, &v_EM_);
		x1 = x + r_EM_[0];
		y1 = y + r_EM_[1];
		z1 = z + r_EM_[2];
		Vx1 = Vx + v_EM_[0];
		Vy1 = Vy + v_EM_[1];
		Vz1 = Vz + v_EM_[2];

		// Вывод в файл
		step = 3600 / dt;
		if (k % step == 0) {
			txtfile << setw(20) << t / 3600.0 
				<< setw(20) << x1*1E-3 << setw(20) << y1*1E-3 << setw(20) << z1*1E-3
				<< setw(20) << Vx1*1E-3 << setw(20) << Vy1*1E-3 << setw(20) << Vz1*1E-3
				<< setw(20) << x*1E-3 << setw(20) << y*1E-3 << setw(20) << z*1E-3
				<< setw(20) << Vx*1E-3 << setw(20) << Vy*1E-3 << setw(20) << Vz*1E-3 << endl;
			cout << setw(20) << t / 3600.0 << setw(20) << r*1E-3 << endl;
		}
		k++;

		if ((t > ( 1 * 3600)) && (r > R_Moonsphere)) {                  //Условие выхода
			cout << "step 3" << endl;
			txtfile << setw(20) << t / 3600.0
				<< setw(20) << x1*1E-3 << setw(20) << y1*1E-3 << setw(20) << z1*1E-3
				<< setw(20) << Vx1*1E-3 << setw(20) << Vy1*1E-3 << setw(20) << Vz1*1E-3
				<< setw(20) << x*1E-3 << setw(20) << y*1E-3 << setw(20) << z*1E-3
				<< setw(20) << Vx*1E-3 << setw(20) << Vy*1E-3 << setw(20) << Vz*1E-3 << endl;
			break;	
		}

		//Цикл расчета коэффициентов
		for (int j = 1; j <= 4; j++) {
			for (int i = 0; i <= 6; i++)  parbuf[i] = par[i] + K[i][j - 1] * a[j - 1]; // Считаем новые аргументы

				//Производные (коэффициенты K[i][j])
			r = pow(x_RK*x_RK + y_RK*y_RK + z_RK*z_RK, 0.5),
				dVxdt = -Mu_Moon*x_RK / (r*r*r)*dt,
				dVydt = -Mu_Moon*y_RK / (r*r*r)*dt,
				dVzdt = -Mu_Moon*z_RK / (r*r*r)*dt,
				dxdt = Vx_RK*dt,
				dydt = Vy_RK*dt,
				dzdt = Vz_RK*dt,
				dtdt = 1 * dt;
		}

		// Приращения
		for (int i = 0; i <= 6; i++) {
			double b[5] = { 0.0, 1.0 / 6.0, 2.0 / 6.0, 2.0 / 6.0, 1.0 / 6.0 };
			for (int j = 1; j <= 4; j++) par[i] += K[i][j] * b[j];
		}

		if ((Moon_peri == false) && (r_buf < r)) {						//Запись момента достижения периселения
			txtfile << setw(20) << t / 3600.0
				<< setw(20) << x1*1E-3 << setw(20) << y1*1E-3 << setw(20) << z1*1E-3
				<< setw(20) << Vx1*1E-3 << setw(20) << Vy1*1E-3 << setw(20) << Vz1*1E-3
				<< setw(20) << x*1E-3 << setw(20) << y*1E-3 << setw(20) << z*1E-3
				<< setw(20) << Vx*1E-3 << setw(20) << Vy*1E-3 << setw(20) << Vz*1E-3 << endl;
			cout << setw(20) << t / 3600.0 << setw(20) << r*1E-3 << setw(20) << "ПЕРИСЕЛЕНИЙ" << endl;
			Moon_peri = true;
		}
		r_buf = r;
	}

	MoonLocation(t_a + t / (3600 * 24), &r_EM_, &v_EM_);
	x += r_EM_[0], y += r_EM_[1], z += r_EM_[2],
		Vx += v_EM_[0], Vy += v_EM_[1], Vz += v_EM_[2];

	// ЭЛЛИПТИЧЕСКАЯ ОРБИТА ВОЗВРАЩЕНИЯ
	r_buf = 1E40;
	for (;;) {
		// Пересчет в лунную СК J2000 
		MoonLocation(t_a + t / (3600 * 24), &r_EM_, &v_EM_);
		x1 = x - r_EM_[0];
		y1 = y - r_EM_[1];
		z1 = z - r_EM_[2];
		Vx1 = Vx - v_EM_[0];
		Vy1 = Vy - v_EM_[1];
		Vz1 = Vz - v_EM_[2];

		// Вывод в файл
		step = 3600 / dt;
		if (k % step == 0) {
			txtfile << setw(20) << t / 3600.0 
				<< setw(20) << x*1E-3 << setw(20) << y*1E-3 << setw(20) << z*1E-3
				<< setw(20) << Vx*1E-3 << setw(20) << Vy*1E-3 << setw(20) << Vz*1E-3
				<< setw(20) << x1*1E-3 << setw(20) << y1*1E-3 << setw(20) << z1*1E-3
				<< setw(20) << Vx1*1E-3 << setw(20) << Vy1*1E-3 << setw(20) << Vz1*1E-3 << endl;
			cout << setw(20) << t / 3600.0 << setw(20) << r*1E-3  << endl;
		}
		k++;

		//Цикл расчета коэффициентов
		for (int j = 1; j <= 4; j++) {
			for (int i = 0; i <= 6; i++)  parbuf[i] = par[i] + K[i][j - 1] * a[j - 1]; // Считаем новые аргументы

																					   //Производные (коэффициенты K[i][j])
			r = pow(x_RK*x_RK + y_RK*y_RK + z_RK*z_RK, 0.5),
				dVxdt = -Mu_Earth*x_RK / (r*r*r)*dt,
				dVydt = -Mu_Earth*y_RK / (r*r*r)*dt,
				dVzdt = -Mu_Earth*z_RK / (r*r*r)*dt,
				dxdt = Vx_RK*dt,
				dydt = Vy_RK*dt,
				dzdt = Vz_RK*dt,
				dtdt = 1 * dt;
		}

		// Приращения
		for (int i = 0; i <= 6; i++) {
			double b[5] = { 0.0, 1.0 / 6.0, 2.0 / 6.0, 2.0 / 6.0, 1.0 / 6.0 };
			for (int j = 1; j <= 4; j++) par[i] += K[i][j] * b[j];
		}

		if (r_buf < r) {		//Условие выхода
			cout << "END" << endl;
			txtfile << setw(20) << t / 3600.0
				<< setw(20) << x*1E-3 << setw(20) << y*1E-3 << setw(20) << z*1E-3
				<< setw(20) << Vx*1E-3 << setw(20) << Vy*1E-3 << setw(20) << Vz*1E-3
				<< setw(20) << x1*1E-3 << setw(20) << y1*1E-3 << setw(20) << z1*1E-3
				<< setw(20) << Vx1*1E-3 << setw(20) << Vy1*1E-3 << setw(20) << Vz1*1E-3 << endl;
			cout << setw(20) << t / 3600.0 << setw(20) << r*1E-3 << endl;
			break;
		}
		r_buf = r;
	}*/
	

	txtfile.close();
	system("pause");
    return 0;
}

