/*
 *	ユーティリティ
 */
#ifndef ULIB_HEADER_STOPWATCH
#define ULIB_HEADER_STOPWATCH

#pragma once

#include <crtdbg.h>     // For _ASSERTE
#include <Windows.h>    // For high-performance timers

namespace ulib { namespace util {


//------------------------------------------------------------------------------
// Class to measure time intervals, for benchmarking portions of code.
// It's a convenient wrapper around the Win32 high-resolution timer APIs
// QueryPerformanceCounter() and QueryPerformanceFrequency().
//------------------------------------------------------------------------------
class Stopwatch
{
public:
	// Initialize the stopwatch to a safe initial state
	Stopwatch() noexcept;

	// Clear the stopwatch state
	void Reset() noexcept;

	// Start measuring time.
	// When finished, call Stop().
	// Can call ElapsedTime() also before calling Stop(): in this case,
	// the elapsed time is measured since the Start() call.
	void Start() noexcept;

	// Stop measuring time.
	// Call ElapsedMilliseconds() to get the elapsed time from the Start() call.
	void Stop() noexcept;

	// Return elapsed time interval duration, in milliseconds.
	// Can be called both after Stop() and before it. 
	// (Start() must have been called to initiate time interval measurements).
	double ElapsedMilliseconds() const noexcept;


	//
	// Ban copy
	//
private:
	Stopwatch(const Stopwatch&) = delete;
	Stopwatch& operator=(const Stopwatch&) = delete;


	//
	// *** IMPLEMENTATION ***
	//
private:
	bool _running;                 // is the timer running?
	long long _start;              // start tick count
	long long _finish;             // end tick count
	const long long _frequency;    // cached frequency value

									//
									// According to MSDN documentation:
									// https://msdn.microsoft.com/en-us/library/windows/desktop/ms644905(v=vs.85).aspx
									//
									// The frequency of the performance counter is fixed at system boot and 
									// is consistent across all processors. 
									// Therefore, the frequency need only be queried upon application 
									// initialization, and the result can be cached.
									//

									// Wrapper to Win32 API QueryPerformanceCounter()
	static long long Counter() noexcept;

	// Wrapper to Win32 API QueryPerformanceFrequency()
	static long long Frequency() noexcept;

	// Calculate elapsed time in milliseconds,
	// given a start tick and end tick counts.
	double ElapsedMilliseconds(long long start, long long finish) const noexcept;
};



//
// Inline implementations
//


inline Stopwatch::Stopwatch() noexcept
	: _running{ false }
	, _start{ 0 }
	, _finish{ 0 }
	, _frequency{ Frequency() }
{}


inline void Stopwatch::Reset() noexcept
{
	_finish = _start = 0;
	_running = false;
}


inline void Stopwatch::Start() noexcept
{
	_running = true;
	_finish = 0;

	_start = Counter();
}


inline void Stopwatch::Stop() noexcept
{
	_finish = Counter();
	_running = false;
}


inline double Stopwatch::ElapsedMilliseconds() const noexcept
{
	if (_running)
	{
		const long long current{ Counter() };
		return ElapsedMilliseconds(_start, current);
	}

	return ElapsedMilliseconds(_start, _finish);
}

/*
 *
 */
inline long long Stopwatch::Counter() noexcept
{
	LARGE_INTEGER li;
	::QueryPerformanceCounter(&li);
	return li.QuadPart;
}


inline long long Stopwatch::Frequency() noexcept
{
	LARGE_INTEGER li;
	::QueryPerformanceFrequency(&li);
	return li.QuadPart;
}


inline double Stopwatch::ElapsedMilliseconds(long long start, long long finish) const noexcept
{
	_ASSERTE(start >= 0);
	_ASSERTE(finish >= 0);
	_ASSERTE(start <= finish);

	return ((finish - start) * 1000.0) / _frequency;
}

}} // end namespace

#endif // ! ULIB_HEADER_STOPWATCH