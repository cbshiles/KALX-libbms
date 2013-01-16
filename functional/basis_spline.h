// basis_spline.h - basis splines
// Copyright (c) 2013 KALX, LLC. All rights reserved.
#pragma once
#include "../include/ensure.h"
#include <algorithm>
#include <functional>

namespace functional {
	enum Foo { ZERO, ONE, TWO, THREE };

//                       x - t(i)              t(i+k+1) - x
//     b(i,k+1)(x)  =  -----------b(i,k)(x) + ---------------b(i+1,k)(x)
//                     t(i+k)-t(i)            t(i+k+1)-t(i+1)
	
	// right continuous i-th basis spline of order k having n knot points t
	template<class T, class U>
	inline std::function<T(size_t,T)> basis_spline_func(size_t k, size_t n, const U* t)
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

			auto B = basis_spline_func<T,U>(k - 1, n, t);
			T b  = B(i, x);
			T b_ = B(i + 1, x);
			T dt  = static_cast<T>(t[i + k] - t[i]);
			T dt_ = static_cast<T>(t[i + k + 1] - t[i + 1]);
			T b_dt = dt ? b/dt : 0;
			T b_dt_ = dt_ ? b_/dt_ : 0;

			return (x - t[i])*b_dt + (t[i + k + 1] - x)*b_dt_;
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
		basis_spline_struct(const basis_spline_struct& B)
			: n_(B.n_), t_(B.t_)
		{ }
		basis_spline_struct& operator=(const basis_spline_struct& B)
		{
			if (this != &B) {
				n_ = B.n_;
				t_ = B.t_;
			}

			return *this;
		}
		~basis_spline_struct()
		{ }

		size_t size(void) const
		{
			return n_;
		}
		void knots(const U* t)
		{
			t_ = t;
		}
		const U* knots(void) const
		{
			return t_;
		}
		U knot(size_t i) const
		{
			return t_[i];
		}
		/// <summary>Value of i-th basis spline at x</summary>
		/// <param name="i">is the ordinal value of the basis spline</param>
		/// <param name="x">is the value at which to evaluate the basis spline</param>
		/// <remarks>
		/// </remarks>
		template<class T>
		T operator()(size_t i, T x) const
		{
			ensure (i + K + 1 < n_);

			auto B = basis_spline_struct<U,K-1>(n_, t_);

			T b  = B(i, x);
			T b_ = B(i + 1, x);
			T dt  = static_cast<T>(t_[i + K] - t_[i]);
			T dt_ = static_cast<T>(t_[i + K + 1] - t_[i + 1]);
			T b_dt = dt ? b/dt : 0;
			T b_dt_ = dt_ ? b_/dt_ : 0;

			return (x - t_[i])*b_dt + (t_[i + K + 1] - x)*b_dt_;
		}
		template<class T>
		T derivative(size_t i, T x) const
		{
			auto B = basis_spline_struct<U,K-1>(n_, t_);

			return K*B(i, x)/(t_[i + K] - t_[i]) - K*B(i + 1, x)/(t_[i + K + 1] - t_[i + 1]);
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
		basis_spline_struct(const basis_spline_struct& B)
			: n_(B.n_), t_(B.t_)
		{ }
		basis_spline_struct& operator=(const basis_spline_struct& B)
		{
			if (this != &B) {
				n_ = B.n_;
				t_ = B.t_;
			}

			return *this;
		}
		~basis_spline_struct()
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
		T derivative(size_t i, T x) const
		{
			return x == t_[i] ? std::numeric_limits<T>::infinity() : x == t_[i + 1] ? -std::numeric_limits<T>::infinity() : 0;
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
	class piecewise_polynomial_struct {
		const T* a_; // size n_ - K - 1
		const basis_spline_struct<U,K>& B_;
	public:
		piecewise_polynomial_struct(const T* a, const basis_spline_struct<U,K>& B)
			: a_(a), B_(B)
		{ }
		piecewise_polynomial_struct(const piecewise_polynomial_struct& P)
			: a_(P.a_), B_(P.B_)
		{ }
		piecewise_polynomial_struct& operator=(const piecewise_polynomial_struct& P)
		{
			if (this != &P) {
				a_ = P.a_;
				B_ = P.B_;
			}

			return *this;
		}
		~piecewise_polynomial_struct()
		{ }

		T operator()(T x) const
		{
			T p(0);
			size_t n = B_.size();
			const U* t = B_.knots();

			// localize based on x
			const U* lb = std::lower_bound(t, t + n, x);
			if (lb == t + n)
				return 0;

			size_t i0 = lb - t;
			for (size_t i = i0; i < i0 + K + 1 && i < B_.size() - K - 1; ++i)
				p += a_[i]*B_(i, x);

			return p;
		}
		T derivative(T x) const
		{
			T p(0);
			size_t n = B_.size();
			const U* t = B_.knots();

			// localize based on x
			const U* lb = std::lower_bound<U>(t, t + n, x);
			if (lb == t + n)
				return 0;

			size_t i0 = lb - t;
			for (size_t i = i0; i < i0 + k + 1 && i < B_.size() - K - 1; ++i)
				p += a_[i]*B_.derivative(i, x);

			return p;
		}
		T integral(T x) const
		{
			T p(0);
			size_t n = B_.size();
			const U* t = B_.knots();

			// localize based on x
			const U* lb = std::lower_bound<U>(t, t + n, x);
			if (lb == t + n)
				return 0;

			size_t i0 = lb - t;
			for (size_t i = i0; i < i0 + k + 2 && i < n_ - K - 1; ++i)
				p += a_[i]*B_.integral(i, x);

			return p;
		}
	};

	template<class T, class U, size_t K>
	inline std::function<T(T)> piecewise_polynomial(const T* a, size_t n, const U* t)
	{
		return [a,n,t](T x) -> T {
			return piecewise_polynomial_struct<T,U,K>(a, basis_spline_struct<U,K>(n, t))(x);
		};
	}

} // namespace functional
