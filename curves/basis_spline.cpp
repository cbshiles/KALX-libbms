// basis_spline.cpp - C++/CLI wrappers and markup.
// Copyright (c) 2013 KALX, LLC. All rights reserved.
#include "basis_spline.h"

/// <summary>Top level summary</summary>
namespace Curves {
	/// <summary>Basis splines form a basis for piecewise polynomial functions.</summary>
	ref class Basis_Spline {
	public:
		/// <summary>Value i-th basis spline of order k given knots t</summary>
		/// <param name="x">is the number at which to evaluate the basis spline</param>
		/// <param name="i">is the index of the basis spline</param>
		/// <param name="k">is the order of the basis spline</param>
		/// <param name="t">is an array containing the knots of the basis spline</param>
		/// <include file='curves.xml' path='curves/basis_spline/remarks[@name="value"]' />
		/// <include file='curves.xml' path='curves/basis_spline/result[@name="value"]' />
		static double value(double x, size_t i, size_t k, array<double>^ t)
		{
			pin_ptr<double> pt(&t[0]);
			const double* t_(pt);

			return curves::basis_spline::value<double,double>(k, t->Length, t_)(i, x);
		}

		/// <summary>Integral of i-th basis spline of order k given knots t</summary>
		/// <param name="x">is the number at which to evaluate the basis spline</param>
		/// <param name="i">is the index of the basis spline</param>
		/// <param name="k">is the order of the basis spline</param>
		/// <param name="t">is an array containing the knots of the basis spline</param>
		/// <remarks>Integration starts at 0.</remarks>
		/// <include file='curves.xml' path='curves/basis_spline/result[@name="integral"]' />
		static double integral(double x, size_t i, size_t k, array<double>^ t)
		{
			pin_ptr<double> pt(&t[0]);
			const double* t_(pt);

			return curves::basis_spline::integral<double,double>(k, t->Length, t_)(i, x);
		}

		/// <summary>Derivative of i-th basis spline of order k given knots t</summary>
		/// <param name="x">is the number at which to evaluate the basis spline</param>
		/// <param name="i">is the index of the basis spline</param>
		/// <param name="k">is the order of the basis spline</param>
		/// <param name="t">is an array containing the knots of the basis spline</param>
		/// <remarks></remarks>
		/// <include file='curves.xml' path='curves/basis_spline/result[@name="derivative"]' />
		static double derivative(double x, size_t i, size_t k, array<double>^ t)
		{
			pin_ptr<double> pt(&t[0]);
			const double* t_(pt);

			return curves::basis_spline::derivative<double,double>(k, t->Length, t_)(i, x);
		}
	};

} // namespace Curves