// grassmann.h - Grassman algebra
#pragma once
#include "../include/ensure.h"
#include <bitset>
#include <map>
#include <utility>

using namespace std::rel_ops;

#define T double

namespace grassmann {

	// a P_i1 .. P_ik, i1 < ... < ik
	template<size_t N>
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
		extensor(const extensor&& A)
			: std::bitset<N>(std::move(A))
		{ }
		extensor& operator=(const extensor& A)
		{
			return std::bitset::operator=(A);
		}
		extensor& operator=(const extensor&& A)
		{
			return std::bitset::operator=(std::move(A));
		}
		~extensor()
		{ }
		bool operator==(const extensor<N>& A) const
		{
			return std::bitset<N>::operator==(A);
		}
		bool operator<(const extensor<N>& A) const
		{
			std::bitset<N> a(A);
			a &= ~A;

			return operator!=(A) && a == 0;
		}
	};

	template<size_t N>
	struct element : public std::map<extensor<N>,T> {
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
		element(const element<N>&& A)
			: std::map<extensor<N>,T>(std::move(A))
		{
		}
		element& operator=(const extensor<N>& A)
		{
			if (this != &A) {
				clear();
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
		element& operator=(const element<N>&& A)
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

			return i != end() ? i->second : 0;
		}
		T& operator[](const extensor<N>& A)
		{
			auto i = find(A);

			if (i == end())
				return insert(std::make_pair(A, 0)).first->second;

			return i->second;
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
		element& operator*=(const extensor<N>& A)
		{
			element<N> B;

			for (auto i = begin(); i != end(); ++i)
				for (auto j = A.begin(); j != A.end(); ++j) {
					T s = sign(i->first, j->first);
					if (s)
						B[i->first | j->first] = s * i->second * j->second;
				}

			std::swap(*this, B);

			return *this;
		}
		element& operator*=(const element<N>& A)
		{
			element<N> B(*this);

			for (auto j = A.begin(); j != A.end(); ++j) {
				B 
			}

			std::swap(*this, B);

			return *this;
		}
	};

	template<size_t I, size_t N>
	struct point : public element<N> {
		point()
			: element(extensor<N>(1<<I))
		{ }
	};
} // namespace grassmann

template<size_t N>
inline T sign(size_t i, const grassmann::extensor<N>& B)
{
	if (B[i])
		return 0;

	T s(1);
	for (size_t j = 0; j < N; ++j)
		if (B[j] && i > j)
			s = -s;

	return s;
}

template<size_t N>
inline T sign(const grassmann::extensor<N>& A, const grassmann::extensor<N>& B)
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
template<size_t N>
inline grassmann::element<N> operator+(const grassmann::element<N>& A, const grassmann::element<N>& B)
{
	return grassmann::element<N>(A) += B;
}
template<size_t N>
inline grassmann::element<N> operator-(const grassmann::element<N>& A, const grassmann::element<N>& B)
{
	return grassmann::element<N>(A) -= B;
}
template<size_t N>
inline grassmann::element<N> operator*(const grassmann::element<N>& A, const grassmann::element<N>& B)
{
	return grassmann::element<N>(A) *= B;
}
