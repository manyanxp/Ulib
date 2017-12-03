#ifndef ULIB_HEADER_COUNTER
#define ULIB_HEADER_COUNTER

#pragma once

#include"critical_setion.h"

namespace ulib { namespace util {

class Counter {
	private:
		int m_count;
		int m_max;
		mutable CriticalSection m_lock;

	private:
		Counter(const Counter& other);
		Counter& operator=(const Counter& other);

	public:
		explicit Counter(int max);
		~Counter();
		Counter();

	public:
		void countUp();
		void countDown();
		void reset();
		void resetMax();
		bool isMax() const;

		void setMax(int max);
		int getMax() const;
		int getCount() const;

		void doMax();
	};

}}

#endif // !ULIB_HEADER_COUNTER
