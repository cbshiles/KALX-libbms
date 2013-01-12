// basis_spline.h - basis splines
// Copyright (c) 2013 KALX, LLC. All rights reserved.
#pragma once
#include <algorithm>
#include <functional>

namespace functional {

//                       x - t(i)              t(i+k+1) - x
//     b(i,k+1)(x)  =  -----------b(i,k)(x) + ---------------b(i+1,k)(x)
//                     t(i+k)-t(i)            t(i+k+1)-t(i+1)
	
	// right continuous i-th basis spline of order k having n knot points t
	template<class T, class U>
	inline std::function<T(size_t,T)> basis_spline(size_t k, size_t n, const U* t)
	{
		if (k == 0) { // order 0
			
			return [n,t](size_t i, T x) -> T { 
				ensure (i + 1 < n);
			
				return 1*(t[i] <= x && x < t[i + 1]); 
			};
		}

		return [k,n,t](size_t i, T x) -> T {
			ensure (i + k + 1 < n);

			if (x < t[i] || x >= t[i + k + 1])
				return 0;

			auto B = basis_spline<T,U>(k - 1, n, t);
			T b  = B(i, x);
			T b_ = B(i + 1, x);
			T dt  = static_cast<T>(t[i + k] - t[i]);
			T dt_ = static_cast<T>(t[i + k + 1] - t[i + 1]);

			return (x - t[i])*b/dt + (t[i + k + 1] - x)*b_/dt_;
		};
	}

	template<class U, size_t K>
	class basis_spline_struct {
		size_t n_;
		const U* t_;
	public:
		basis_spline_struct(size_t n, const U* t)
			: n_(n), t_(t)
		{ }
		size_t size(void) const
		{
			return n_;
		}
		const U* knots(void) const
		{
			return t_;
		}
		U knot(size_t i) const
		{
			return t_[i];
		}
		template<class T>
		T operator()(size_t i, T x) const
		{
			ensure (i + K + 1 < n_);

			auto B = basis_spline_struct<U,K-1>(n_, t_);

			T b  = B(i, x);
			T b_ = B(i + 1, x);
			T dt  = static_cast<T>(t_[i + K] - t_[i]);
			T dt_ = static_cast<T>(t_[i + K + 1] - t_[i + 1]);

			return (x - t_[i])*b/dt + (t_[i + K + 1] - x)*b_/dt_;
		}
		template<class T>
		T integral(size_t i, T x) const
		{
			T I(0);

			for (size_t j = i; j + K + 2 < n_; ++j)
				I += basis_spline_struct<U, K + 1>(n_, t_)(j, x);

			return I*(t_[i + K + 1] - t_[i])/(K + 1);
		}
	};
	template<class U>
	struct basis_spline_struct<U,0> {
		size_t n_;
		const U* t_;
	public:
		basis_spline_struct(size_t n, const U* t)
			: n_(n), t_(t)
		{ }
		size_t size(void) const
		{
			return n_;
		}
		const U* knots(void) const
		{
			return t_;
		}
		U knot(size_t i) const
		{
			return t_[i];
		}
		template<class T>
		T operator()(size_t i, T x) const
		{
			ensure (i + 1 < n_);

			return 1*(t_[i] <= x && x < t_[i + 1]);
		}
		template<class T>
		T integral(size_t i, T x) const
		{
			T I(0);

			for (size_t j = i; j + 2 < n_; ++j) // use knots!!!
				I += basis_spline_struct<U, 1>(n_, t_)(j, x);

			return I*(t_[i + 1] - t_[i]);
		}
	};

	template<class T, class U, size_t K>
	inline std::function<T(size_t,T)> integral(const basis_spline_struct<U, K>& f)
	{
		return [f](size_t i, T x) -> T {
			return f<U,K>::integral(i, x);
		};
	}

	template<class T, class U>
	inline std::function<T(T)> bspline(const U* a, size_t k, size_t n, const U* t)
	{
		return [a,k,n,t](T x) {
			T b(0);

			//!! localize based on x first!!!
			U* lb = std::lower_bound<U>(t, t + n, x);
			if (lb == t + n)
				return 0;

			i0 = lb - t;
			for (size_t i = i0; i < i0 + k; ++i)
				b += a[i]*basis_spline(i, k, n, t)(x);

			return b;
		};
	}


} // namespace functional
