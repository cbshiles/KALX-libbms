// piecewise_polynomial.h - Piecewise polynomial functions.
// Copyright (c) 2013 KALX, LLC. All rights reserved.
#pragma once
#include "basis_spline.h"

namespace curves {

	template<class T, class U>
	class piecewise_polynomial {
		const T* a_; // size n - k - 1
		size_t k_, n_;
		const U* t_;
	public:
		piecewise_polynomial(const T* a, size_t k, size_t n, const U* t)
			: a_(a), k_(k), n_(n), t_(t)
		{ }
		piecewise_polynomial(const piecewise_polynomial& P)
			: a_(P.a_), B_(P.B_)
		{ }
		piecewise_polynomial& operator=(const piecewise_polynomial& P)
		{
			if (this != &P) {
				a_ = P.a_;
				B_ = P.B_;
			}

			return *this;
		}
		~piecewise_polynomial()
		{ }

		T operator()(T x) const
		{
			T p(0);

			// localize based on x
			const U* lb = std::lower_bound(t_, t_ + n_, x);
			if (lb == t_ + n_)
				return 0;

			auto B = curves::basis_spline::value<T,U>(k_, n_, t_);
			size_t i0 = lb - t_;
			for (size_t i = i0; i < i0 + k_ + 1 && i < n_ - k_ - 1; ++i)
				p += a_[i]*B(i, x);

			return p;
		}
		T derivative(T x) const
		{
			T p(0);

			// localize based on x
			const U* lb = std::lower_bound(t_, t_ + n_, x);
			if (lb == t_ + n_)
				return 0;

			auto B = curves::basis_spline::derivative<T,U>(k_, n_, t_);
			size_t i0 = lb - t_;
			for (size_t i = i0; i < i0 + k_ + 1 && i < n_ - k_ - 1; ++i)
				p += a_[i]*B(i, x);

			return p;
		}
		T integral(T x) const
		{
			T p(0);

			// localize based on x
			const U* lb = std::lower_bound(t_, t_ + n_, x);
			if (lb == t_ + n_)
				return 0;

			auto B = curves::basis_spline::integral<T,U>(k_, n_, t_);
			size_t i0 = lb - t_;
			for (size_t i = i0; i < i0 + k_ + 1 && i < n_ - k_ - 1; ++i)
				p += a_[i]*B(i, x);

			return p;
		}
	};
/*
	template<class T, class U, size_t K>
	inline std::function<T(T)> piecewise_polynomial(const T* a, size_t n, const U* t)
	{
		return [a,n,t](T x) -> T {
			return piecewise_polynomial_struct<T,U,K>(a, basis_spline_struct<U,K>(n, t))(x);
		};
	}
*/

} // namespace curves

