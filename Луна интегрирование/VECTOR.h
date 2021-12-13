#include "stdafx.h"
#include <iostream>
#include <cmath>
#include <iomanip>
#include <fstream>
#include <vector>
#define _USE_MATH_DEFINES
#include "math.h"
using namespace std;
// Функция нахождения модуля вектора
double getLenght(vector <double> vec) {
	double sum = 0;
	for (int i = 0; i < vec.size(); i++) {
		sum += vec[i] * vec[i];
	}
	return pow(sum, 0.5);
}
// Функция нахождения произведения вектора на число
vector <double> vecMultiNumber(double a, vector <double> vec1) {
	vector <double> vec2 = { 0,0,0 };
	for (int i = 0; i < vec1.size(); i++) {
		vec2[i] = a*vec1[i];
	}
	return vec2;
}
// Функция нахождения вычитания векторов
vector <double> vecSum(vector <double> vec1, vector <double> vec2) {
	vector <double> vec3 = { 0,0,0 };
	for (int i = 0; i < vec1.size(); i++) {
		vec3[i] = vec1[i] + vec2[i];
	}
	return vec3;
}
// Функция нахождения вычитания векторов
vector <double> vecDifference(vector <double> vec1, vector <double> vec2) {
	vector <double> vec3 = { 0,0,0 };
	for (int i = 0; i < vec1.size(); i++) {
		vec3[i] = vec1[i] - vec2[i];
	}
	return vec3;
}
// Функция нахождения скалярного произведения векторов
double vecMultiSkal(vector <double> vec1, vector <double> vec2) {
	double sum = 0;
	for (int i = 0; i < vec1.size(); i++) {
		sum += vec1[i] * vec2[i];
	}
	return sum;
}
// Функция нахождения векторного произведения векторов
vector <double> vecMulti(vector <double> vec1, vector <double> vec2) {
	vector <double> vec3 = { 0,0,0 };
	vec3[0] = vec1[1] * vec2[2] - vec1[2] * vec2[1];
	vec3[1] = vec1[2] * vec2[0] - vec1[0] * vec2[2];
	vec3[2] = vec1[0] * vec2[1] - vec1[1] * vec2[0];
	return vec3;
}
