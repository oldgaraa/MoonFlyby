#include <iostream> 
#include <cmath> 
#define _USE_MATH_DEFINES 
#include "math.h"
#include "VECTOR.h"
using namespace std;


// Функция переводит в десятичные градусы 
double gr(double rounds, double grad, double min, double sec) {
	if ((min>60) | (sec>3600)) cout << "************ОШИБКА В ФУНКЦИИ РАСЧЕТА ПОЛОЖЕНИЯ ЛУНЫ************" << endl;
	return (rounds * 360.0 + grad + min / 60.0 + sec / 3600.0);
}

// Функция по текущему юлианскому дню возвращает эклиптические координаты Луны ("Таблицы движения Луны" Брауна) 
void MoonLocation(double t_ul, vector <double> *r_EM_, vector <double> *v_TM_, bool flag = 0) { //flag = 0 всегда
	double fi, lamda, r;
	double t = (t_ul - 2415020.0) / 36525.0,
		lm = gr(0.0, 270.0, 26.0, 11.71) + gr(1336.0, 307.0, 53.0, 26.06)*t + gr(0.0, 0.0, 0.0, 7.14)*t*t + gr(0.0, 0.0, 0.0, 0.0068)*t*t*t,
		omegaM = gr(0.0, 334.0, 19.0, 46.4) + gr(11.0, 109.0, 2.0, 2.52)*t - gr(0.0, 0.0, 0.0, 37.17)*t*t - gr(0.0, 0.0, 0.0, 0.045)*t*t*t,
		RAANM = gr(0.0, 259.0, 10.0, 59.79) - gr(5.0, 134.0, 8.0, 31.23)*t + gr(0.0, 0.0, 0.0, 7.48)*t*t + gr(0.0, 0.0, 0.0, 0.008)*t*t*t,
		A = gr(0.0, 350.0, 44.0, 23.67) + gr(1236.0, 307.0, 7.0, 17.93)*t + gr(0.0, 0.0, 0.0, 6.05)*t*t + gr(0.0, 0.0, 0.0, 0.0068)*t*t*t,
		B = gr(0.0, 296.0, 6.0, 25.31) + gr(1325.0, 198.0, 51.0, 23.54)*t + gr(0.0, 0.0, 0.0, 44.31)*t*t + gr(0.0, 0.0, 0.0, 0.0518)*t*t*t,
		C = gr(0.0, 358.0, 28.0, 33.0) + gr(99.0, 359.0, 2.0, 59.1)*t - gr(0.0, 0.0, 0.0, 0.54)*t*t - gr(0.0, 0.0, 0.0, 0.012)*t*t*t,
		D = gr(0.0, 11.0, 15.0, 11.92) + gr(1342.0, 82.0, 1.0, 57.29)*t - gr(0.0, 0.0, 0.0, 0.34)*t*t - gr(0.0, 0.0, 0.0, 0.0012)*t*t*t; // Во втором слагаемом возможно опечатка

	lm = 3600 * (lm - 360 * round((lm / 360)));  // вычет лишних 360N градусов и перевод в секунды

	A *= M_PI / 180.0, B *= M_PI / 180.0, C *= M_PI / 180.0, D *= M_PI / 180.0; // Перевод в радианы 
	lamda = lm + 22639.500*sin(B) - 4586.426*sin(B - 2 * A) +
		2369.902*sin(2 * A) + 769.016*sin(2 * B) -
		668.111*sin(C) - 411.608*sin(2 * D) -
		211.656*sin(2 * B - 2 * A) -
		205.962*sin(B + C - 2 * A) - 125.15*sin(A) +
		191.953*sin(B + 2 * A) - 165.145*sin(C - 2 * A) +
		147.693*sin(B - C) - 109.667*sin(B + C);
	fi = (18461.480*sin(D) + 1010.180*sin(B + D) -
		999.695*sin(D - B) - 623.658*sin(D - 2 * A) +
		117.262*sin(D + 2 * A) + 199.485*sin(D + 2 * A - B) -
		166.577*sin(B + D - 2 * A) +
		61.913*sin(2 * B + D));
	// Параллакс 
	double PAR = (3422.700 + 186.5398*cos(B) + 34.3117*cos(B - 2 * A) +
		28.2333*cos(2 * A) + 10.1657*cos(2 * B) +
		3.0861*cos(B + 2 * A) + 1.9202*cos(C - 2 * A) +
		+1.4455*cos(B + C - 2 * A) +
		1.1542*cos(B - C));
	// Переводим из угловых секунд в радианы 
	lamda *= M_PI / 180.0 / 3600.0;
	fi *= M_PI / 180.0 / 3600.0;
	PAR *= M_PI / 180.0 / 3600.0;
	//cout << 0 << endl;
	//cout << lamda * 180 / M_PI << endl << fi * 180 / M_PI << endl << PAR * 180 / M_PI << endl;
	r = 6378.137E3 / sin(PAR);

	double eclip = (gr(0, 23, 26, 21.448) - gr(0, 0, 0, 46.8150)*t - gr(0, 0, 0, 0.00059)*t*t + gr(0, 0, 0, 0.001813)*t*t*t)*M_PI / 180.0;  // Наклонение плоскости эклиптики (примерно)

																																			// Направляющие косинусы J200 (поворот из эклиптической СК)
	vector <double> ix_ = { 1, 0, 0 },
		iy_ = { 0, cos(eclip), sin(eclip) },
		iz_ = { 0, -sin(eclip), cos(eclip) },

		// Направляющие косинусы вектора r_EM
		i_M_ = vecSum(vecSum(vecMultiNumber(cos(fi)*cos(lamda), ix_), vecMultiNumber(cos(fi)*sin(lamda), iy_)), vecMultiNumber(sin(fi), iz_)),
		r_M_ = vecMultiNumber(r, i_M_);
	*r_EM_ = r_M_;

	// НАХОДИМ СКОРОСТЬ ЧЕРЕЗ КОНЕЧНЫЕ РАЗНОСТИ
	vector <double> r_EM_new_;
	if (flag == 0) {
		MoonLocation(t_ul + 1, &r_EM_new_, v_TM_, 1);
		*v_TM_ = { (r_EM_new_[0] - r_M_[0]) / (1 * 24 * 3600), (r_EM_new_[1] - r_M_[1]) / (1 * 24 * 3600) , (r_EM_new_[2] - r_M_[2]) / (1 * 24 * 3600) };
	}
}
