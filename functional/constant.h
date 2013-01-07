// constant.h - constant function
// Copyright (c) 2013 KALX, LLC. All rights reserved.
#pragma once
#include <functional>

namespace functional {

	template<class T>
	struct constant {
		T c;
		constant(T _c)
			: c(_c)
		{ }
		T operator()(T) const
		{
			return c;
		}
	};
	template<class T>
	inline std::function<T(T)> integral(const constant<T>& c)
	{
		return [c](T t) -> T { return t*c(0); };
	}

} // namespace functional