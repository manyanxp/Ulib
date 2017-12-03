#ifndef UTIL_HEADER_LOCK_MUTEX
#define UTIL_HEADER_LOCK_MUTEX

#pragma once

#include"syncer.h"
#include<string>

namespace ulib { namespace lock {

/**
* �~���[�e�b�N�X.
*/
class Mutex : public Syncer {
private:
	/** �~���[�e�b�N�X�{��. */
	HANDLE _handle;
	/** ����X���b�h�Ń��b�N���ꂽ��. */
	int _lockCount;
	//�~���[�e�b�N�X��ۗL���Ă���X���b�hID
	uint32_t _usLockedThreadID;
private:
	// �֎~.
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
	* ���̃I�u�W�F�N�g�����b�N����Ă��邩�ǂ���.
	* (���m�ɂ͎��X���b�h�����b�N�������Ă��邩�ǂ����𒲂ׂ�.)
	*/
	virtual	bool Mutex::isLocked() const {
		return _lockCount > 0;
	}

	/**
	* ���݂̃��b�N�����擾����.
	* ��Ƀf�o�b�O�ɗ��p���邽��.
	*/
	int Mutex::getLockCount() const {
		return _lockCount;
	}
};

}} // end namespace

#endif