#ifndef UTIL_HEADER_LOCK_SYNCER
#define UTIL_HEADER_LOCK_SYNCER

#pragma once

namespace ulib { namespace lock {

class Syncer {
public:
	Syncer() {}
	virtual ~Syncer(){}
};

}} // end namespace

#endif // !UTIL_HEADER_LOCK_SYNCER
