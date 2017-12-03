#include"stdafx.h"
#include"mutex.h"

namespace ulib { namespace lock {

//ロック待ちタイムアウト時間
#define LOCK_WAIT_TIME_OUT 60000

/**
 * コンストラクタ
 * @param lpMutexAttributes セキュリティ記述子
 * @param lpName 名称.(\以外ならよい）. 最大長はMAX_PATH
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
 * コンストラクタ
 * ミューテックスオブジェクトを生成する.
 *
 * @param lpMutexAttributes セキュリティ記述子
 * @param lpName 名称.(\以外ならよい）. 最大長はMAX_PATH
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
 * デストラクタ.
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
 * ロックオブジェクトを開く.
 *
 * @param lpMutexAttributes セキュリティ記述子. 指定しない場合はNULL
 * @param lpName ロックオブジェクトの名称.
 *
 */
bool Mutex::open(LPSECURITY_ATTRIBUTES lpMutexAttributes, LPCTSTR lpName) {
	this->_handle = ::CreateMutex(lpMutexAttributes, false, lpName);
	return (this->_handle != nullptr);
}

/**
 * ロックが取得できるまで待機する。
 */
bool Mutex::lock(const LPCTSTR pFile, uint32_t uiLine)
{
	std::string name(pFile);
	_lockfilename = name;
	_locklineno = uiLine;
	return lock();
}

/**
 * ロックが取得できるまで待機する.
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
 * ロックが取得できるか、タイムアウトが発生するまで待機する.
 *
 * @param 待機時間をミリ秒で指定する.
 * @return ロック獲得成功時にtrue、それ以外の場合はfalse.
 */
bool Mutex::lock(uint32_t waitTime) {
	// 待機する.
	uint32_t result = WaitForSingleObject(_handle, waitTime);
	if( (result == WAIT_OBJECT_0) || (result == WAIT_ABANDONED) ) {
		_lockCount++;
		if( result == WAIT_ABANDONED ) {
			;
		}
		_usLockedThreadID = GetCurrentThreadId();	// ミューテックスを保有しているスレッドIDを保持
	}

	return ( (result == WAIT_OBJECT_0) || (result == WAIT_ABANDONED) );
}

/**
 * ロックを解放する。
 */
bool Mutex::unlock(const LPCTSTR pFile, uint32_t uiLine)
{
	std::string name(pFile);
	_unlockfilename = name;
	_unlocklineno = uiLine;
	return unlock();
}

/**
 * ロックを解放する.
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