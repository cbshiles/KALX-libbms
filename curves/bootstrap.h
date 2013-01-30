// bootstrap.h - bootstrap a piecewise-flat forward curve
// Copyright (c) 2013 KALX, LLC. All rights reserved.
#pragma once
/*
Piecewise-flat forward curves are modeled by arrays t and f, and an extrapolated value _f;

	       { f[0] , 0 <= t <= t[0]
	f(t) = { f[i] , t[i-1] < t <= t[i], i = 1,...,n-1
	       { _f   , t > t[n-1]

|                 _f
|  f0          o---------
*------*  f1   
|      o-------*
|
0______t0______t1________

Note f(t[i]) = f[i], unlike for order 0 basis splines.
*/
#include <algorithm>
#include "../include/ensure.h"
#include "../numerical/newton.h"

namespace curves {
namespace pwflat {

	template<class T = double, class F = double>
	class forward {
		size_t n_;
		const T* t_;
		const F* f_;
		F _f_;
	public:
		forward(size_t n = 0, const T* t = 0, const F* f = 0, F _f = 0)
			: n_(n), t_(t), f_(f), _f_(_f)
		{ }
		size_t size(void) const
		{
			return n_;
		}
		F extrapolate(void) const
		{
			return _f;
		}
		forward& extrapolate(F _f)
		{
			_f_ = _f;

			return *this;
		}
		F value(T u) const
		{
			const T* ti = std::lower_bound(t_, t_ + n_, u); // t <= *ti

			if (ti == t_ + n_)
				return _f_;

			return f_[ti - t_];
		}
		F operator()(T u) const
		{
			return value(u);
		}
		F integral(T u) const
		{
			F I(0);
			T t0 = 0;

			size_t i;
			for (i = 0; i < n_ && t_[i] < u; ++i) {
				I += f_[i]*(t_[i] - t0);
				t0 = t_[i];
			}

			I += static_cast<F>((i == n_ ? _f_ : f_[i])*(u - t0));

			return I;
		}
		F present_value(size_t m, const T* u, const F* c)
		{
			F pv(0);

			for (size_t i = 0; i < m; ++i) {
				pv += c[i] * exp(-integral(u[i]));
			}

			return pv;
		}
		F duration(size_t m, const T* u, const T* c, T u0 = 0)
		{
			T dur(0);

			const T* ui = std::lower_bound(u, u + m, u0); // u0 <= *ui

			for (size_t i = ui - u; i < m; ++i) {
				dur += - (u[i] - u0) * c[i] * exp(-integral(u[i]));
			}

			return dur;
		}
	};
/*
	// global functions
	template<class T, class F>
	inline std::function<F(T)> discount(size_t n, const T* t, const F* f, const F& f_)
	{
		return [n,t,f,f_](T u) -> F 
		{ 
			return exp(-forward<T,F>(n, t, f, f_).integral(u)); 
		};
	}

	template<class T, class F>
	inline std::function<F(T)>  spot(size_t n, const T* t, const T* f, const T& f_)
	{
		static T tol(sqrt(std::numeric_limits<T>::epsilon());

		return [tol,n,t,f,f_](T u) -> F 
		{ 
			return fabs(t) < tol ? f(t) : forward(n, t, f, f_).integral(u)/t; 
		};
	}
*/

	// cash deposit
	template<class T, class F>
	inline T bootstrap1(T u, F c, size_t n, const T* t, const F* f)
	{
		ensure (u > 0);
		ensure (c > 0);

		T t0 = n ? t[n-1] : 0;
		F D0 = forward(n, t, f)discount(t0);

		return log(c*D0)/(u - t0);
	}

	// forward rate agreement
	template<class T, class F>
	inline T bootstrap2(T u0, F c0, T u1, F c1, size_t n, const T* t, const F* f)
	{
		T _f;
		T t0 = n ? t[n-1] : 0;
		F D0 = forward(n, t, f)discount(t0);
		T d = -c1/c0; // works if c0 != -1

		if (u0 < t0) { // overlap or cash deposit
			T Dm = forward(n, t, f)discount(u1);
			_f = static_cast<T>(log(d*D0/Dm)/(u1 - u0));
		}
		else { // underlap
			_f = static_cast<T>(log(d)/(u1 - t0));
		}

		return _f;
	}

	template<class T, class F>
	inline T bootstrap(size_t m, const T* u, const F* c, size_t n, const T* t, const F* f, T _f = 0, T p = 0)
	{
		ensure (m && (n == 0 || u[m-1] > t[n-1]));

		// cd
		if (m == 1) {
			return bootstrap1<T,F>(u[0], c[0], n, t, f);
		}

		// fra
		if (m == 2 && p == 0) {
			return bootstrap2<T,F>(u[0], c[0], u[1], c[1], n, t, f);
		}
		
		T t0 = n ? t[n-1] : 0;
		const T* um = std::upper_bound(u, u + m, t0);
		ensure (um != u + m); // already checked above
		F p0 = forward<T,F>(n, t, f).present_value(um - u, u, c);

		m -= um - u;
		c += um - u;
		u += um - u;

		auto F = [p,p0,m,u,c,n,t,f](F _f) 
		{ 
			return -p + p0 + forward<T,F>(n, t, f, f_).present_value(m, u, c); 
		};
		auto dF = [t0,m,u,c,n,t,f](F _f) 
		{ 
			return forward_curve<T,F>(n, t, f, _f).duration(m, u, c, t0);
		};

		if (_f == 0)
			_f = n ? f[n-1] : static_cast<F>(0.01);

		return numerical::root1d::newton(_f, F, dF);
	}

} // namespace pwflat
} // namespace curves