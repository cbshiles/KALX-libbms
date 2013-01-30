// polynomial.cpp
#include "polynomial.h"

namespace Curves {
	/// <summary>Polynomial summary</summary>
	public ref class Polynomial {
	public:
		/// <summary>Evaluate polynomial at x using the Horner method.</summary>
		/// <param name="x">is the number at which to evaluate the polynomial</param>
		/// <param name="p">is an array of polynomial coefficients</param>
		/// <remarks>Calculates p[0] + x*(p[1] + ... )</remarks>
		/// <include file='curves.xml' path='curves/polynomial/result[@name="horner"]' />
		static double horner(double x, array<double>^ p) {
			pin_ptr<double> pp(&p[0]);
			const double* p_(pp);

			return curves::polynomial::horner<double,double>(p->Length, p_)(x);
		}

		/// <summary>Evaluate a Taylor polynomial at x using the Horner method.</summary>
		/// <param name="x">is the number at which to evaluate the polynomial</param>
		/// <param name="p">is an array of Taylor polynomial coefficients</param>
		/// <remarks>Calculates p[0] + x*(p[1] + x/2*(p[2] + ... )</remarks>
		/// <include file='curves.xml' path='curves/polynomial/result[@name="taylor"]' />
		static double taylor(double x, array<double>^ p) {
			pin_ptr<double> pp(&p[0]);
			const double* p_(pp);

			return curves::polynomial::taylor<double,double>(p->Length, p_)(x);
		}
	};
}