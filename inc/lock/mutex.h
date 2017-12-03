#ifndef UTIL_HEADER_LOCK_MUTEX
#define UTIL_HEADER_LOCK_MUTEX

#pragma once

#include"syncer.h"
#include<string>

namespace ulib { namespace lock {

/**
* ミューテックス.
*/
class Mutex : public Syncer {
private:
	/** ミューテックス本体. */
	HANDLE _handle;
	/** 同一スレッドでロックされた回数. */
	int _lockCount;
	//ミューテックスを保有しているスレッドID
	uint32_t _usLockedThreadID;
private:
	// 禁止.
	Mutex(const Mutex&);
	Mutex& operator=(const Mutex&);

	std::string _lockobjname;
	std::string _lockfilename;
	uint32_t   _locklineno;
	std::string _unlockfilename;
	uint32_t   _unlocklineno;

public:
	Mutex();
	Mutex(LPSECURITY_ATTRIBUTES lpMutexAttributes, LPCTSTR lpName);
	virtual ~Mutex();

public:
	virtual bool open(LPSECURITY_ATTRIBUTES lpMutexAttributes, LPCTSTR lpName);
	virtual bool lock();
	virtual bool lock(const LPCTSTR pFile, uint32_t uiLine);
	virtual bool lock(uint32_t waitTime);
	virtual bool unlock();
	virtual bool unlock(const LPCTSTR pFile, uint32_t uiLine);
	/**
	* このオブジェクトがロックされているかどうか.
	* (正確には自スレッドがロックをかけているかどうかを調べる.)
	*/
	virtual	bool Mutex::isLocked() const {
		return _lockCount > 0;
	}

	/**
	* 現在のロック数を取得する.
	* 主にデバッグに利用するため.
	*/
	int Mutex::getLockCount() const {
		return _lockCount;
	}
};

}} // end namespace

#endif