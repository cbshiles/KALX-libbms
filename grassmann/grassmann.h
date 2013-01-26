// grassmann.h - Grassman algebra
// Copyright (c) 2013 KALX, LLC. All rights reserved.
#pragma once
#include "../include/ensure.h"
#include <algorithm>
#include <bitset>
#include <map>
#include <utility>

using namespace std::rel_ops;

namespace grassmann {

	// P_i P_j ... has bits set at i, j,...
	template<size_t N, class T = double>
	struct extensor : public std::bitset<N> {
		extensor()
			: std::bitset<N>()
		{ }
		extensor(unsigned long long u)
			: std::bitset<N>(u)
		{ }
		extensor(const extensor& A)
			: std::bitset<N>(A)
		{ }
		extensor(const std::bitset<N>& A)
			: std::bitset<N>(A)
		{ }
		extensor(const extensor&& A)
			: std::bitset<N>(std::move(A))
		{ }
		extensor(std::bitset<N>&& A)
			: std::bitset<N>(std::move(A))
		{ }
		extensor& operator=(const extensor& A)
		{
			return std::bitset<N>::operator=(A);
		}
		extensor& operator=(extensor&& A)
		{
			return std::bitset<N>::operator=(std::move(A));
		}
		~extensor()
		{ }
		// strict weak ordering
		bool operator<(const extensor<N>& A) const
		{
			if (std::bitset<N>::count() != A.count())
				return std::bitset<N>::count() < A.count();

			for (size_t i = 0; i < N; ++i)
				if (std::bitset<N>::operator[](i) != A[i])
					return A[i];

			return false;
		}
		size_t grade(void) const
		{
			return std::bitset<N>::count();
		}
	};

	template<size_t N, class T = double>
	struct element : public std::map<extensor<N>,T> {
		typedef typename std::map<extensor<N>,T> map;
		typedef typename map::iterator iterator;
		typedef typename map::value_type value_type;
		element()
			: std::map<extensor<N>,T>()
		{ }
		element(const extensor<N>& A)
			: std::map<extensor<N>,T>()
		{
			operator[](A) = 1;
		}
		element(const element<N>& A)
			: std::map<extensor<N>,T>(A)
		{
		}
		element(element<N>&& A)
			: std::map<extensor<N>,T>(std::move(A))
		{
		}
		element& operator=(const extensor<N>& A)
		{
			if (this != &A) {
				map::clear();
				operator[](A) = 1;
			}

			return *this;
		}
		element& operator=(const element<N>& A)
		{
			if (this != &A) {
				std::map<extensor<N>,T>::operator=(A);
			}

			return *this;
		}
		element& operator=(element<N>&& A)
		{
			if (this != &A) {
				std::map<extensor<N>,T>::operator=(std::move(A));
			}

			return *this;
		}
		~element()
		{ }

		T operator[](const extensor<N>& A) const
		{
			auto i = find(A);

			return i != map::end() ? i->second : 0;
		}
		T& operator[](const extensor<N>& A)
		{
			auto i = find(A);

			if (i == map::end())
				i = insert(std::make_pair(A, 0)).first;

			return i->second;
		}
		T operator[](const element<N>& A) const
		{
			ensure (A.size() == 1);

			auto a = A.begin();
			ensure (a->second == 1);

			return operator[](a->first);
		}
		T& operator[](const element<N>& A)
		{
			ensure (A.size() == 1);

			auto a = A.begin();
			ensure (a->second == 1);

			return operator[](a->first);
		}

		element& operator+=(const element<N>& A)
		{
			for (auto i = A.begin(); i != A.end(); ++i)
				operator[](i->first) += i->second;

			return *this;
		}
		element& operator-=(const element<N>& A)
		{
			for (auto i = A.begin(); i != A.end(); ++i)
				operator[](i->first) -= i->second;

			return *this;
		}
		element& operator*=(T a)
		{
			std::for_each(map::begin(), map::end(), [a](value_type& i) { i.second *= a; });

			return *this;
		}
		element& operator*=(const extensor<N>& B)
		{
			element<N> A;

			for (auto i = map::begin(); i != map::end(); ++i) {
				T s = sign(i->first, B);
				if (s)
					A[i->first | B] = s * i->second;
			}

			swap(A);

			return *this;
		}
		element& operator*=(const element<N>& B)
		{
			element<N> A;

			for (auto i = map::begin(); i != map::end(); ++i) {
				for (auto j = B.begin(); j != B.end(); ++j) {
					T w = i->second * j->second;
					if (w) {
						T s = sign(i->first, j->first);
						if (s)
							A[i->first | j->first] += s * w;
					}
				}
			}

			swap(A);

			return *this;
		}
		T operator/(const element<N>& A) const
		{
			if (map::size() == 1 && A.size() == 1 && map::begin()->first == A.begin()->first)
				return map::begin()->second/A.begin()->second;

			return std::numeric_limits<T>::quiet_NaN();
		}
		element operator~() const
		{
			element A;

			for (auto i = map::begin(); i != map::end(); ++i) {
				extensor<N> a(~(i->first));

				A[a] = sign(i->first,a)/i->second;
			}

			return A;
		}
	};

	template<size_t N>
	struct point : public element<N> {
		point(size_t i)
			: element<N>(extensor<N>(1<<i))
		{ }
	};

} // namespace grassmann

template<size_t N, class T>
inline T sign(size_t i, const grassmann::extensor<N,T>& B)
{
	if (B[i])
		return 0;

	T s(1);
	for (size_t j = 0; j < N; ++j)
		if (B[j] && i > j)
			s = -s;

	return s;
}

template<size_t N, class T>
inline T sign(const grassmann::extensor<N,T>& A, const grassmann::extensor<N,T>& B)
{
	T s(1);

	for (size_t i = 0; i < N; ++i) {
		if (A[i]) {
			if (B[i])
				return 0;
			s *= sign(i, B);
		}
	}

	return s;
}
template<size_t N, class T>
inline grassmann::element<N,T> operator+(const grassmann::element<N,T>& A, const grassmann::element<N,T>& B)
{
	return grassmann::element<N,T>(A) += B;
}
template<size_t N, class T>
inline grassmann::element<N,T> operator-(const grassmann::element<N,T>& A, const grassmann::element<N,T>& B)
{
	return grassmann::element<N,T>(A) -= B;
}
template<size_t N, class T>
inline grassmann::element<N,T> operator*(const grassmann::element<N,T>& A, const grassmann::element<N,T>& B)
{
	return grassmann::element<N,T>(A) *= B;
}
template<size_t N, class T>
inline grassmann::element<N,T> operator*(const T& a, const grassmann::element<N,T>& B)
{
	return grassmann::element<N,T>(B) *= a;
}
template<size_t N, class T>
inline grassmann::element<N,T> operator*(int a, const grassmann::element<N,T>& B)
{
	return grassmann::element<N,T>(B) *= a;
}
template<size_t N, class T>
inline grassmann::element<N,T> operator*(const grassmann::element<N,T>& A, const T& b)
{
	return grassmann::element<N,T>(A) *= b;
}
template<size_t N, class T>
inline grassmann::element<N,T> operator*(const grassmann::element<N,T>& A, int b)
{
	return grassmann::element<N,T>(A) *= b;
}

template<size_t N, class T>
inline grassmann::element<N-1,T> boundary(const grassmann::extensor<N,T>& A)
{
	T s(1);
	grassmann::element<N,T> B;

	for (size_t i = 0; i < N; ++i) {
		if (A[i]) {
			B += s*grassmann::extensor<N,T>(A&~std::bitset<N>(1<<i));
			s = -s;
		}
 	}

	return B;
}
