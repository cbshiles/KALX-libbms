// functional.h - routines involving functions
// Copyright (c) 2006-2013 KALX, LLC. All rights reserved. No warranty is made.
#pragma once

#include "../include/ensure.h"
#include "apply_iterator.h"
#include "basis_spline.h"
#include "polynomial.h"

namespace functional {

	template<class F>
	inline auto derivative(const F& f) -> std::function<decltype(f(0))(decltype(f(0)))>
	{
		typedef typename decltype(f(0)) T;

		return [f](T x) -> T {
			T h = sqrt(std::numeric_limits<T>::epsilon());

			return (f(x + h) - f(x - h))/(2*h);
		};
	}

	template<class F, class G, class T>
	inline auto extrapolate(const F& f, T _x, const G& g) -> std::function<T(T)>
	{
		return [f,g,_x](T x) -> T { return x <= _x ? f(x) : g(x); };
	}

	template<class F>
	inline auto integral(const F& f, size_t n = 100) -> std::function<decltype(f(0))(decltype(f(0)))>
	{
		typedef typename decltype(f(0)) T;

		// integral from 0 to x !!! replace with Romberg
		return [f,n](T x) -> T {
			T I(0);

			T f0 = f(0);
			for (size_t i = 1; i <= n; ++i) {
				T fi = f(x*i/n);
				I += (f0 + fi)/(2*n);
				f0 = fi;
			}

			return I;
		};
	}

} // namespace functional