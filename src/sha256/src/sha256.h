#ifndef _SHA256_H_
#define _SHA256_H_

#include "BinaryNumber_lib/src/binarynumber.h"

namespace sh {

	bn::BinaryNumber* createBinaryMessage(std::string message);
	void sha256(std::string message);
	
}

#endif // !_SHA256_H_
