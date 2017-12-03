#include"stdafx.h"
#include"mutex.h"

namespace ulib { namespace lock {

//���b�N�҂��^�C���A�E�g����
#define LOCK_WAIT_TIME_OUT 60000

/**
 * �R���X�g���N�^
 * @param lpMutexAttributes �Z�L�����e�B�L�q�q
 * @param lpName ����.(\�ȊO�Ȃ�悢�j. �ő咷��MAX_PATH
 */
Mutex::Mutex() :
_handle(nullptr),
_lockCount(0),
_lockfilename(""),
_unlockfilename(""),
_locklineno(0),
_unlocklineno(0),
_lockobjname("") {
}


/**
 * �R���X�g���N�^
 * �~���[�e�b�N�X�I�u�W�F�N�g�𐶐�����.
 *
 * @param lpMutexAttributes �Z�L�����e�B�L�q�q
 * @param lpName ����.(\�ȊO�Ȃ�悢�j. �ő咷��MAX_PATH
 */
Mutex::Mutex(LPSECURITY_ATTRIBUTES lpMutexAttributes, LPCTSTR lpName) :
_handle(nullptr),
_lockCount(0),
_lockfilename(""),
_unlockfilename(""),
_locklineno(0),
_unlocklineno(0){	
	_handle = ::CreateMutex(lpMutexAttributes, false, lpName);
	if( lpName != nullptr) {
		std::string name(lpName);
		_lockobjname = name;
	}
}


/**
 * �f�X�g���N�^.
 */
Mutex::~Mutex() {
	while( isLocked() ) {
		if( !unlock() ) {
			;
		}
	}
	if(this->_handle != nullptr) {
		::CloseHandle(this->_handle);
		this->_handle = nullptr;
	}
}

/**
 * ���b�N�I�u�W�F�N�g���J��.
 *
 * @param lpMutexAttributes �Z�L�����e�B�L�q�q. �w�肵�Ȃ��ꍇ��NULL
 * @param lpName ���b�N�I�u�W�F�N�g�̖���.
 *
 */
bool Mutex::open(LPSECURITY_ATTRIBUTES lpMutexAttributes, LPCTSTR lpName) {
	this->_handle = ::CreateMutex(lpMutexAttributes, false, lpName);
	return (this->_handle != nullptr);
}

/**
 * ���b�N���擾�ł���܂őҋ@����B
 */
bool Mutex::lock(const LPCTSTR pFile, uint32_t uiLine)
{
	std::string name(pFile);
	_lockfilename = name;
	_locklineno = uiLine;
	return lock();
}

/**
 * ���b�N���擾�ł���܂őҋ@����.
 */
bool Mutex::lock() {
	//return lock(INFINITE);
	int cnt = 0;
	while( lock( LOCK_WAIT_TIME_OUT ) != TRUE ) {
		cnt++;
	}

	_lockfilename = "";
	_locklineno = 0;

	return true;
}

/**
 * ���b�N���擾�ł��邩�A�^�C���A�E�g����������܂őҋ@����.
 *
 * @param �ҋ@���Ԃ��~���b�Ŏw�肷��.
 * @return ���b�N�l����������true�A����ȊO�̏ꍇ��false.
 */
bool Mutex::lock(uint32_t waitTime) {
	// �ҋ@����.
	uint32_t result = WaitForSingleObject(_handle, waitTime);
	if( (result == WAIT_OBJECT_0) || (result == WAIT_ABANDONED) ) {
		_lockCount++;
		if( result == WAIT_ABANDONED ) {
			;
		}
		_usLockedThreadID = GetCurrentThreadId();	// �~���[�e�b�N�X��ۗL���Ă���X���b�hID��ێ�
	}

	return ( (result == WAIT_OBJECT_0) || (result == WAIT_ABANDONED) );
}

/**
 * ���b�N���������B
 */
bool Mutex::unlock(const LPCTSTR pFile, uint32_t uiLine)
{
	std::string name(pFile);
	_unlockfilename = name;
	_unlocklineno = uiLine;
	return unlock();
}

/**
 * ���b�N���������.
 */
bool Mutex::unlock() {
	_lockCount--;
	BOOL result = ReleaseMutex( _handle );
	if( result == FALSE) {
		;
	}

	_unlockfilename = "";
	_unlocklineno = 0;

	return (result != FALSE);
}

}} // end namespace