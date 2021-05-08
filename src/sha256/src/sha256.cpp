#include "sha256.h"



int binaryToInt4bits(int* n) {
	int sum = 0;
	for (int i = 0; i < 4; i++) {
		sum += n[i] * pow(2, 3 - i);
	}
	return sum;
}

std::string binaryToHex32bits(int* n) {
	std::stringstream hex;
	for (int i = 0; i < 32; i += 4) {
		hex << sh::hex_base[binaryToInt4bits(&n[i])];
	}
	return hex.str();
}

int* intToBinary32bits(unsigned int n) {
	int* b = (int*)malloc(32 * sizeof(int));

	for (int i = 31; i >= 0; i--) {
		b[i] = n % 2;
		n /= 2;
	}

	return b;
}

void sumBinaryMod32bits(int* a, int* b) {
	int sum = 0;
	for (int i = 31; i >= 0; i--) {
		sum = a[i] + b[i] + (sum < 2 ? 0 : 1);
		a[i] = (sum < 2 ? sum : 0) + (sum == 3 ? 1 : 0);
	}
}

int xorgate(int x, int y) {
	return (x && not(y) || not(x) && y);
}

void xor32bits(int* x, int *y) {
	for (int i = 0; i < 32; i++) {
		x[i] = xorgate(x[i], y[i]);
	}
}

void print32bits(int* fragment) {
	for (int i = 0; i < 32; i++) {
		printf("%d", fragment[i]);
	}
	printf("\n");
}

void copyTo32bits(int* to, int* from) {
	for (int i = 0; i < 32; i++) {
		to[i] = from[i];
	}
}

int* rotr32bits(int* b, int times) {
	int new_position;

	int* y = (int*)malloc(32 * sizeof(int));

	for (int i = 0; i < 32; i++) {
		new_position = (i + times) % 32;
		y[new_position] = b[i];
	}
	return y;
}
int* shr32bits(int* b, int times) {
	int* y = (int*)malloc(32 * sizeof(int));
	
	int new_position;
	for (int j = 31; j >= times; j--) {
		new_position = j - times;
		y[j] = b[new_position];
	}
	if (times > 32) times = 32;
	for (int i = 0; i < times; i++) {
		y[i] = 0;
	}
	return y;
}

int* o0(int* a) {
	int* aux0, * aux1;

	aux0 = shr32bits(a, 3);

	aux1 = rotr32bits(a, 18);
	xor32bits(aux0, aux1);
	free(aux1);

	aux1 = rotr32bits(a, 7);
	xor32bits(aux0, aux1);
	free(aux1);

	return aux0;
}

int* o1(int* a) {
	int* aux0, * aux1;

	aux0 = rotr32bits(a, 17);

	aux1 = rotr32bits(a, 19);
	xor32bits(aux0, aux1);
	free(aux1);

	aux1 = shr32bits(a, 10);
	xor32bits(aux0, aux1);
	free(aux1);

	return aux0;
}

int* e1(int* a) {
	int* aux0, * aux1;

	aux0 = rotr32bits(a, 6);

	aux1 = rotr32bits(a, 11);
	xor32bits(aux0, aux1);
	free(aux1);

	aux1 = rotr32bits(a, 25);
	xor32bits(aux0, aux1);
	free(aux1);

	return aux0;
}

int* e0(int* a) {
	int* aux0, * aux1;

	aux0 = rotr32bits(a, 22);

	aux1 = rotr32bits(a, 13);
	xor32bits(aux0, aux1);
	free(aux1);

	aux1 = rotr32bits(a, 2);
	xor32bits(aux0, aux1);
	free(aux1);

	return aux0;
}

int* maj32bits(int* a, int* b, int* c) {
	int* y = (int*)malloc(32 * sizeof(int));
	int count_1;
	for (int i = 0; i < 32; i++) {
		count_1 = (a[i] == 1 ? 1 : 0) + (b[i] == 1 ? 1 : 0) + (c[i] == 1 ? 1 : 0);
		y[i] = (count_1 > 1 ? 1 : 0);
	}
	return y;
}

int* ch32bits(int* a, int* b, int* c) {
	int* y = (int*)malloc(32 * sizeof(int));
	for (int i = 0; i < 32; i++) {
		y[i] = (a[i] ? b[i] : c[i]);
	}
	return y;
}
namespace sh {

	bn::BinaryNumber* createBinaryMessage(std::string message) {
		// converting message into binary.
		bn::BinaryNumber *binary_message = bn::stringToBinary(message);
		int start_binary_message_lenght = binary_message->lenght;

		// adding 1 after binary message.
		bn::appendBit(binary_message, 1);

		// count message blocks.
		int n_message_blocks = 1;
		while (n_message_blocks * 512 < binary_message->lenght + 64) n_message_blocks++;

		// padding.
		int padding = n_message_blocks * 512 - binary_message->lenght - 64;
		bn::appendBit(binary_message, 0, padding);

		// append the lenght.
		bn::append64bitNumber(binary_message, start_binary_message_lenght);


		return binary_message;
	}

	std::string sha256(std::string message) {
		bn::BinaryNumber* message_blocks = createBinaryMessage(message);

		int n_blocks = message_blocks->lenght / 512;


		// buffers.
		int** k = (int**)malloc(64 * sizeof(int*));

		int** s_hi = (int**)malloc(8 * sizeof(int*));
		int** hi = (int**)malloc(8 * sizeof(int*));

		// converting buffers.
		for (int i = 0; i < 64; i++) {
			k[i] = intToBinary32bits(K[i]);
		}

		for (int i = 0; i < 8; i++) {
			s_hi[i] = (int*)malloc(32 * sizeof(int));
			hi[i] = intToBinary32bits(HI[i]);
		}

		// initializing w.
		int** w = (int**)malloc(64 * sizeof(int*));
		for (int i_w = 0; i_w < 64; i_w++) {
			w[i_w] = (int*)malloc(32 * sizeof(int));
		}
		// initializing blocks.
		int* aux_binary, * t1, * t2;

		for (int i_block = 0; i_block < n_blocks; i_block++) {
			
			// starting starters. 
			for (int i = 0; i < 8; i++) {
				copyTo32bits(s_hi[i], hi[i]);
			}
			/////////////
			

			// copying the 16 first w.
			for (int i_w = 0; i_w < 16; i_w++) {
				for (int i_binary = 0; i_binary < 32; i_binary++) {
					w[i_w][i_binary] = message_blocks->data[i_block * 512 + i_w * 32 + i_binary];
				}
			}
			
			// generating the 48 others w.
			for (int i_w = 16; i_w < 64; i_w++) {

				aux_binary = o1(w[i_w - 2]);
				copyTo32bits(w[i_w], aux_binary);
				free(aux_binary);

				sumBinaryMod32bits(w[i_w], w[i_w - 7]);

				aux_binary = o0(w[i_w - 15]);
				sumBinaryMod32bits(w[i_w], aux_binary);
				free(aux_binary);

				sumBinaryMod32bits(w[i_w], w[i_w - 16]);
				
			}

			// compression.
			
			for (int i = 0; i < 64; i++) {
				///////// t1 ////////////
				t1 = e1(hi[4]);

				aux_binary = ch32bits(hi[4], hi[5], hi[6]);
				sumBinaryMod32bits(t1, aux_binary);
				free(aux_binary);

				sumBinaryMod32bits(t1, hi[7]);

				sumBinaryMod32bits(t1, k[i]);
				sumBinaryMod32bits(t1, w[i]);
				////////////////////////
				///////// t2 ////////////
				t2 = e0(hi[0]);

				aux_binary = maj32bits(hi[0], hi[1], hi[2]);
				sumBinaryMod32bits(t2, aux_binary);
				free(aux_binary);

				////////////////////////

				// steer down hi.
				free(hi[7]);
				for (int i = 7; i > 0; i--) {
					hi[i] = hi[i - 1];
				}
				hi[0] = t1;
				sumBinaryMod32bits(hi[4], t1);
				sumBinaryMod32bits(hi[0], t2);

				free(t2);
			}
			for (int i = 0; i < 8; i++) {
				sumBinaryMod32bits(hi[i], s_hi[i]);
			}
		}

		
		for (int i = 0; i < 64; i++) {
			free(k[i]);
		}
		free(k);
		
		std::stringstream str;

		for (int i = 0; i < 8; i++) {
			str << binaryToHex32bits(hi[i]);
			print32bits(hi[i]);
			free(hi[i]);
			free(s_hi[i]);
		}
		free(hi);
		free(s_hi);


		// freeing.
		// copying the 16 first w.
		for (int i_w = 0; i_w < 16; i_w++) {
			free(w[i_w]);
		}
		free(w);

		// should be.
		bn::destroyBinaryNumber(message_blocks);
		return str.str();
	}
}