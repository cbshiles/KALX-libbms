// piecewise_polynomial.cpp - C++/CLI wrappers and markup.
// Copyright (c) 2013 KALX, LLC. All rights reserved.
#include "piecewise_polynomial.h"

namespace Curves {

	/// <summary>Piecewise polynomials represented as linear combinations of basis splines.</summary>
	ref class Piecewise_Polynomial {
	public:
		/// <summary>Evaluate a piecewise polynomial function at a value.</summary>
		/// <param name="x">is the value at which the function is evaluated</param>
		/// <param name="a">is the array of coefficients</param>
		/// <param name="k">is the order of the basis splines</param>
		/// <param name="t">is the array of knots</param>
		/// <remarks></remarks>
		/// <include file='curves.xml' path='curves/piecewise_polynomial/result[@name="value"]' />
		static double value(double x, array<double>^a, size_t k, array<double>^ t)
		{
			pin_ptr<double> a_ = &a[0];
			pin_ptr<double> t_ = &t[0];
			const double* pa = a_;
			const double* pt = t_;

			return curves::piecewise_polynomial<double,double>(pa, k, t->Length, pt)(x);
		}
	};

} // namespace Curves