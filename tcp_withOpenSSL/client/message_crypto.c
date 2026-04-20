#include "message_crypto.h"

static const char SECRET_KEY = 0x5A; 

void apply_xor_cipher(char* buffer, int length) {
    for (int i = 0; i < length; ++i) {
        if (buffer[i] != '\n') {
            buffer[i] = buffer[i] ^ SECRET_KEY;
        }
    }
}