#include "sha256.h"

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

	void sha256(std::string message) {
		bn::BinaryNumber* message_blocks = createBinaryMessage(message);

		int n_blocks = message_blocks->lenght / 512;

		int*** w_binary_blocks = (int***)malloc(n_blocks * sizeof(int**));
		int* aux_binary;


		for (int i_block = 0; i_block < n_blocks; i_block++) {
			w_binary_blocks[i_block] = (int**)malloc(64 * sizeof(int*));
			

			// copying the 16 first w.
			for (int i_w = 0; i_w < 16; i_w++) {
				w_binary_blocks[i_block][i_w] = (int*)malloc(32 * sizeof(int));
				for (int i_binary = 0; i_binary < 32; i_binary++) {
					w_binary_blocks[i_block][i_w][i_binary] = message_blocks->data[i_w * 32 + i_binary];
				}
				print32bits(w_binary_blocks[i_block][i_w]);
			}
			
			// generating the 48 others w.
			for (int i_w = 16; i_w < 64; i_w++) {
				w_binary_blocks[i_block][i_w] = (int*)malloc(32 * sizeof(int));

				aux_binary = o1(w_binary_blocks[i_block][i_w - 2]);
				copyTo32bits(w_binary_blocks[i_block][i_w], aux_binary);
				free(aux_binary);

				sumBinaryMod32bits(w_binary_blocks[i_block][i_w], w_binary_blocks[i_block][i_w - 7]);

				aux_binary = o0(w_binary_blocks[i_block][i_w - 15]);
				sumBinaryMod32bits(w_binary_blocks[i_block][i_w], aux_binary);
				free(aux_binary);

				sumBinaryMod32bits(w_binary_blocks[i_block][i_w], w_binary_blocks[i_block][i_w - 16]);
				
				print32bits(w_binary_blocks[i_block][i_w]);
			}
		}

		// freeing.
		for (int i_block = 0; i_block < n_blocks; i_block++) {


			// copying the 16 first w.
			for (int i_w = 0; i_w < 16; i_w++) {
				free(w_binary_blocks[i_block][i_w]);
			}
			free(w_binary_blocks[i_block]);
		}
		free(w_binary_blocks);
	

		// should be.
		bn::destroyBinaryNumber(message_blocks);
	}
}