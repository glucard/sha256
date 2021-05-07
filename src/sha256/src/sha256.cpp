#include "sha256.h"

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

}