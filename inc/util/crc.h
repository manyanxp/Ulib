#ifndef UTIL_HEADER_CRC
#define UTIL_HEADER_CRC

#pragma once

namespace ulib { namespace util {

typedef unsigned short crc16_t;
typedef unsigned short crc32_t;

/*
*	CRC16
*/
class CRC16
{
public:
	// コンストラクタ
	CRC16(crc16_t crc_key) {
		make_crc_table(crc_key);
		clear();
	}

	virtual ~CRC16() {}

protected:
	crc16_t crc_table[256];
	crc16_t crc;
	void make_crc_table(crc16_t crc_key);

public:
	// クリア
	void clear() { crc = 0xffff; }
	// CRCテーブル作成
	void make_crc_table(crc16_t crc_key) {
		crc16_t c;
		int n, k;

		for (n = 0; n < 256; n++) {
			c = (crc16_t)n;
			for (k = 0; k < 8; k++) {
				if (c & 1)
					c = crc_key ^ (c >> 1);
				else
					c = c >> 1;
			}
			crc_table[n] = c;
		}
	}

	// 更新
	void update(const unsigned char* buffer, int size) {
		int n;
		for (n = 0; n < size; ++n) {
			crc = crc_table[(crc ^ buffer[n]) & 0xff] ^ (crc >> 8);
		}
	}
};

/*
*	CRC32
*/
class CRC32
{
public:
	// コンストラクタ
	CRC32(crc32_t crc_key) {
		make_crc_table(crc_key);
		clear();
	}

	virtual ~CRC32() {}

protected:
	crc32_t crc_table[256];
	crc32_t crc;
	void make_crc_table(crc32_t crc_key);

public:
	// クリア
	void clear() { crc = 0xffffffff; }
	// CRCテーブル作成
	void make_crc_table(crc32_t crc_key) {
		crc32_t c;
		int n, k;

		for (n = 0; n < 256; n++) {
			c = (crc32_t)n;
			for (k = 0; k < 8; k++) {
				if (c & 1)
					c = crc_key ^ (c >> 1);
				else
					c = c >> 1;
			}
			crc_table[n] = c;
		}
	}

	// 更新
	void update(const unsigned char* buffer, int size) {
		int n;
		for (n = 0; n < size; ++n) {
			crc = crc_table[(crc ^ buffer[n]) & 0xff] ^ (crc >> 8);
		}
	}
};

}}  // end namespace

#endif //!UTIL_HEADER_CRC
