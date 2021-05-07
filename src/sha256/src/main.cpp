#include "sha256.h"

int main() {
	bn::BinaryNumber* b = sh::createBinaryMessage("abc");
	bn::promptData(b);
	bn::destroyBinaryNumber(b);

	return 0;
}