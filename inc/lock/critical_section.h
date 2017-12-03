#ifndef ULIB_HEADER_CRITICAL_SETION
#define ULIB_HEADER_CRITICAL_SETION

#pragma once

#include"syncer.h"

namespace ulib { namespace util {

class CriticalSection : public Syncer {
private:
	CRITICAL_SECTION m_section;
	int m_lockCount;

private:
	// �֎~.
	CriticalSection(const CriticalSection&);
	CriticalSection& operator=(const CriticalSection&);

public:
	CriticalSection();
	virtual ~CriticalSection();

public:
	int getLockCount() const;
	virtual bool lock();
	virtual bool lock(DWORD waitTime);
	virtual bool unlock();
	virtual bool isLocked() const;

};

}}

#endif // !ULIB_HEADER_CRITICAL_SETION