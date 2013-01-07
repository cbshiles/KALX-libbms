// ensure.h - works like assert, but throws an exception
// Copyright (c) 2006 KALX, LLC. All rights reserved. No warranty is made.
//
// #define ensure(x)
// before including to turn ensure checking off
//
// #define ENSURE_BREAK to set breakpoint instead of throwing exception
#pragma once
#include <iosfwd>
#include <stdexcept>

#ifndef ensure

#define ENSURE_HASH_(x) #x
#define ENSURE_STRZ_(x) ENSURE_HASH_(x)
#define ENSURE_FILE "file: " __FILE__
#define ENSURE_FUNC "function: " __FUNCTION__
#define ENSURE_LINE "line: " ENSURE_STRZ_(__LINE__)
#define ENSURE_SPOT ENSURE_FILE "\n" ENSURE_LINE "\n" ENSURE_FUNC

#ifdef ENSURE_BREAK
#ifdef _WIN32 // defined for 64 bit also
#define ensure(e) if (!(e)) { DebugBreak(); }
#elif defined(__GNUC__)
#define ensure(e) if (!(e)) { __builtin_trap(); }
#endif

#else // throw exception
#define ensure(e) if (!(e)) {throw std::runtime_error(ENSURE_SPOT "\nensure: \"" #e "\" failed");}
#endif

#define ENSURE_TRY(block) try { block } catch (const std::exception& ex) { std::cerr << ex.what() << std::endl; }

#endif // ensure

