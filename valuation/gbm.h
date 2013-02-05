// gbm.h - geometric brownian motion
// Copyright (c) 2013 KALX, LLC. All rights reserved.
#pragma once
#include <cmath>
#include <functional>

namespace valuation {

	template<class T>
	class gbm {
		T mu_, sigma_;
	public:
		gbm(T mu, T sigma)
			: mu_(mu), sigma_(sigma)
		{ }
	};

	template<class T>
	inline std::function<T(T)> pdf(const gbm<T>& m)
	{
		return [m](T t) -> T {
			return exp(-(log(t) - m.mu_)*(t - m.mu_)/(2*m.sigma_*m.sigma_))/(t*m.sigma_*sqrt(M_2PI));
		};
	}
	template<class T>
	inline std::function<T(T)> cdf(const gbm<T>& m)
	{
		return [m](T t) -> T {
			return exp(t);
		};
	}

	template<class T, class I, class M>
	inline T value(I i, M m);

	template<class T>
	struct put {
		T t, k;
		put(T _t, T _k)
			: t(_t), k(_k)
		{ }
	};

	template<class T>
	struct bms {
		T f, s;
		bms(T _f, T _s)
			: f(_f), s(_s)
		{ }
	};

	template<class T>
	inline T value(put<T> p, bms<T> m)
	{
		return p.k*cdf(gbm<T>(0,m.s))(p.k);// - m.f*cdf(gbm<T>(bms<T>(m.f,m.s)))(p.k);
	}

} // namespace valuation
