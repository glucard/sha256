#ifndef _BINARY_NUMBER_H_
#define _BINARY_NUMBER_H_

#include <string>

namespace bn {
	
	static int word_size = 8;

	typedef struct binarynumber {
		int lenght;
		int* data;
	} BinaryNumber;

	void promptData(BinaryNumber* b);
	void destroyBinaryNumber(BinaryNumber* b);
	BinaryNumber* stringToBinary(std::string str);
	void appendBit(BinaryNumber* b, int bit, int n_bits = 1);
	void append64bitNumber(BinaryNumber* b, int n);

}

#endif // !_BINARY_NUMBER_H_