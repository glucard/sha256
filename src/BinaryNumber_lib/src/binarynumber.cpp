#include "binarynumber.h"


void letterToBinaryInMessageFragment(unsigned char letter, int* message_fragment) {
	for (int i = bn::word_size - 1; i >= 0; i--) {
		message_fragment[i] = letter % 2;
		letter /= 2;
	}
}

namespace bn {

	void promptData(BinaryNumber* b) {
		for (int i = 0; i < b->lenght; i++) {
			printf("%d", b->data[i]);
		}
		printf("\n");
	}

	void destroyBinaryNumber(BinaryNumber* b) {
		free(b->data);
		free(b);
	}

	BinaryNumber* stringToBinary(std::string str) {
		int str_lenght = str.length();
		BinaryNumber* b = (BinaryNumber*)malloc(sizeof(BinaryNumber));
		b->lenght = str_lenght * word_size;
		b->data = (int*)malloc(b->lenght * sizeof(int));

		for (int i = 0; i < str_lenght; i++) {
			letterToBinaryInMessageFragment(str[i], &b->data[i * word_size]);
		}

		return b;
	}

	void appendBit(BinaryNumber* b, int bit, int n_bits) {
		b->lenght += n_bits;
		b->data = (int*)realloc(b->data, b->lenght * sizeof(int));
		
		for (int i = 1; i <= n_bits; i++) {
			b->data[b->lenght - i] = bit;
		}
	}

	void append64bitNumber(BinaryNumber* b, int n) {
		b->lenght += 64;
		b->data = (int*)realloc(b->data, b->lenght * sizeof(int));

		for (int i = 1; i <= 64; i++) {
			b->data[b->lenght - i] = n % 2;
			n /= 2;
		}

	}

}