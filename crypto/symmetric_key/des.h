#pragma once
#include "../data.h"

void des_encrypt(pData input, pData key, pData output, int rounds);
void des_decrypt(pData input, pData key, pData output, int rounds);