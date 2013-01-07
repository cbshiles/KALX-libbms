// srng.h - simple random number generator with optionally stored seed
// Copyright (c) 2011 KALX, LLC. All rights reserved. No warranty is made.
#pragma once
#include <ctime>
#include <cstdint>
#ifdef _WIN32
#include "../win/registry.h"
#define SRNG_SUBKEY _T("Software\\KALX\\bms")
#else
#include <fstream>
#endif
#include <xutility>

using namespace std::rel_ops;

namespace numerical {

	class srng { 
		static const uint32_t min = 0;
		static const uint32_t max = 0xFFFFFFFF;
		uint32_t s_[2];
	public:
		srng(bool stored = false)
		{
			if (stored) {
				load();
			}
			else {
				s_[0] = static_cast<uint32_t>(::time(0));
				s_[1] = ~s_[0];

				save();
			}
		}
		srng(uint32_t s0, uint32_t s1)
		{
			s_[0] = s0;
			s_[1] = s1;
		}
		srng(const srng& rng)
		{
			s_[0] = rng.s_[0];
			s_[1] = rng.s_[1];
		}
		srng& operator=(const srng& rng)
		{
			if (this != &rng) {
				s_[0] = rng.s_[0];
				s_[1] = rng.s_[1];
			}

			return *this;
		}
		~srng()
		{ }

		bool operator==(const srng& rng) const
		{
			return s_[0] == rng.s_[0] && s_[1] == rng.s_[1];
		}
		bool operator<(const srng& rng) const
		{
			return s_[0] < rng.s_[0] || s_[0] == rng.s_[0] && s_[1] < rng.s_[1];
		}
#ifdef _WIN32
		// store state in registry
		void save(void) const
		{
			Reg::CreateKey<TCHAR>(HKEY_CURRENT_USER, SRNG_SUBKEY).SetValue<DWORD>(_T("seed0"), s_[0]);
			Reg::CreateKey<TCHAR>(HKEY_CURRENT_USER, SRNG_SUBKEY).SetValue<DWORD>(_T("seed1"), s_[1]);
		}
		void load(void)
		{
			Reg::Object<TCHAR, DWORD> s0(HKEY_CURRENT_USER, SRNG_SUBKEY, _T("seed0"), 521288629);
			Reg::Object<TCHAR, DWORD> s1(HKEY_CURRENT_USER, SRNG_SUBKEY, _T("seed1"), 362436069);
			s_[0] = s0;
			s_[1] = s1;
		}
#else
		void save(void) const 
		{
			ofstream ofs("srng.seed");

			ofs << s_[0] << " " << s_[1];
		}
		void load(void)
		{
			ifstream ifs("srng.seed");

			if (ifs.good())
				ifs >> s_[0] >> s_[1]; 
			else {
				s_[0] = 521288629;
				s_[1] = 362436069;

				save();
			}
		}
#endif // _WIN32
		void seed(uint32_t s0, uint32_t s1)
		{ 
			s_[0] = s0;
			s_[1] = s1;

			save();
		}
		void seed(uint32_t s[2])
		{
			s_[0] = s[0];
			s_[1] = s[1];

			save();
		}
		const uint32_t* seed(void) const
		{
			return s_;
		}

		// uniform unsigned int in the range [0, 2^32)
		// See http://www.bobwheeler.com/statistics/Password/MarsagliaPost.txt
		uint32_t uint()
		{
			s_[1] = 36969 * (s_[1] & 0xFFFF) + (s_[1] >> 0x10);
			s_[0] = 18000 * (s_[0] & 0xFFFF) + (s_[0] >> 0x10);

			return (s_[1] << 0x10) + s_[0];
		}

		// uniform double in the range [0, 1)
		double real()
		{
			// 0 <= u < 2^32
			double u = uint();

			return u/max;
		}
		// uniform int in [a, b]
		int between(int a, int b)
		{
			return static_cast<int>(a + ((b - a + 1)*real()));
		}
		// uniform double in [a, b)
		double uniform(double a = 0, double b = 1)
		{
			return a + (b - a)*real();
		}


	};
} // namespace utility
