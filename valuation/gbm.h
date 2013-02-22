// gbm.h - geometric brownian motion
// Copyright (c) 2013 KALX, LLC. All rights reserved.
#pragma once
#include <cmath>
#include <functional>

namespace valuation {

	template<class T = double>
	class gbm {
		T mu_, sigma_;
	public:
		gbm(T mu, T sigma)
			: mu_(mu), sigma_(sigma)
		{ }
	};

	template<class T = double>
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

	template<class T = double>
	struct put {
		T t, k;
		put(T _t, T _k)
			: t(_t), k(_k)
		{ }
	};
	template<class T = double>
	struct call {
		T t, k;
		call(T _t, T _k)
			: t(_t), k(_k)
		{ }
	};

	template<class T = double>
	struct bms {
		T f, s;
		bms(T _f, T _s)
			: f(_f), s(_s)
		{ }
	};

	// k P(F <= k) - f P(F exp(sigma^2t) <= k)
	template<class T>
	inline T value(put<T> i, bms<T> m)
	{
		return i.k * cdf(gbm<T>(0,m.s))(i.k) - m.f * cdf(gbm<T>(m.s*m.s*i.t,m.s))(i.k);
	}

} // namespace valuation
