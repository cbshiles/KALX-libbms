// Basis_Spline.cpp
// Copyright 2013 (c) KALX, LLC. No warranty is made. All rights reserved.
#include "../basis_spline.h"
#include "Curves.h"

using namespace xll;

double WINAPI xll_curves_basis_spline_derivative(double x, size_t i, size_t k, _FP* t)
{
#pragma XLLEXPORT
	double result(0);

	try {
		result = curves::basis_spline::derivative<double,double>(k, size(*t), t->array)(i, x);
	}
	catch (const std::exception& ex) {
		XLL_ERROR(ex.what());
	}

	return result;
}

double WINAPI xll_curves_basis_spline_integral(double x, size_t i, size_t k, _FP* t)
{
#pragma XLLEXPORT
	double result(0);

	try {
		result = curves::basis_spline::integral<double,double>(k, size(*t), t->array)(i, x);
	}
	catch (const std::exception& ex) {
		XLL_ERROR(ex.what());
	}

	return result;
}

double WINAPI xll_curves_basis_spline_value(double x, size_t i, size_t k, _FP* t)
{
#pragma XLLEXPORT
	double result(0);

	try {
		result = curves::basis_spline::value<double,double>(k, size(*t), t->array)(i, x);
	}
	catch (const std::exception& ex) {
		XLL_ERROR(ex.what());
	}

	return result;
}

