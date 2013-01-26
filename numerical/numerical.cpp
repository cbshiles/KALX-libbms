// numerical.cpp - C++/CLI interface
using namespace System;
#include "numerical.h"

namespace cppcli {

public ref class numerical1 {
	/// <summary>Units in the last place</summary>
	/// <param name="x">is the first floating point value</param>
	/// <param name="y">is the second floating point value</param>
	/// <remarks>
	/// Reurns the number of floating point values that can be represented between x and y.
	/// </remarks>
	public static System::Integer64 ulp(double x, double y)
	{
		return numerical::ulp<double>(x, y);
	}
	//public ref class root1d {
}; // numerical
}