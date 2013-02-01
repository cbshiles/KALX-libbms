// yield_curve.cpp - piecewise flat forward yield curve
// Copyright (c) 2013 KALX, LLC. All rights reserved.
#include "yield_curve.h"

namespace Curves {
	namespace PWFlat {
		/// <summary>Piecewise flat forward yield curve.</summary>
		ref class Yield_Curve {
			curves::pwflat::yield_curve<>* p_;
		public:
			/// <summary>Construct a piecewise flat yield curve.</summary>
			Yield_Curve()
				: p_(new curves::pwflat::yield_curve<>())
			{ }
			~Yield_Curve()
			{
				delete p_;
			}
			/// <summary>Add instrument to curve.</summary>
			/// <param name="t">are the time in years of the cash flows</param>
			/// <param name="c">are the cash flows corresponding to times</param>
			/// <remarks>Maturities must be increasing.</remarks>
			Yield_Curve^ Add(array<double>^ t, array<double>^ c)
			{
				pin_ptr<double> t_(&t[0]);
				pin_ptr<double> c_(&c[0]);

				const double* pt = t_;
				const double* pc = c_;

				p_->add(t->Length, pt, pc);

				return this;
			}
		};
	}
}