#include <stdlib.h>
#include <stdio.h>
#include "des.h"

void init(pData key, char *sbox_map, char *pbox_map, char *key_map, char *kep_compress_map);
void des_round(pData roundInput, pData expandData, pData roundKey, char *sbox_map, char *pbox_map, char *key_map, char *key_compress_map, pData roundOutput, int r);
void des_round_decrypt(pData roundInput, pData expandData, pData roundKey, char *sbox_map, char *pbox_map, char *key_map, char *key_compress_map, pData roundOutput, int r);
void expand(pData roundInput, pData expandData);
void expand_decrypt(pData roundInput, pData expandData);
void key_mapping(pData roundKey, char *key_map);
void key_mapping_decrypt(pData roundKey, char *key_map);
void leftShift(pData roundKey, int r);
void leftShift_decrypt(pData roundKey, int r);
//compress 포함.
void xor(char *key_compress_map, pData roundKey, pData expandData);
void sbox(pData expandData, char *sbox_map, pData roundOutput);
void sbox_decrypt(pData expandData, char *sbox_map, pData roundOutput);
void pbox(char *pbox_map, pData roundOutput);
void pbox_decrypt(char *pbox_map, pData roundOutput);
void swap(pData roundInput, pData roundOutput);
void swap_decrypt(pData roundInput, pData roundOutput);

char str[] = "                                                                      ";

void des_encrypt(pData input, pData key, pData output, int rounds){
    int block_index_max = input->size * input->typeSize / 8;
    pData expandData = Data_create_zeros(6,1);
    pData roundKey = Data_create_copy(key);
    pData roundInput = Data_create_zeros(8,1);
    pData roundOutput = Data_create_zeros(8,1);
    
    char *sbox_map = malloc(64);
    char *pbox_map = malloc(32);
    char *key_map = malloc(56);
    char *key_compress_map = malloc(48);
    init(key, sbox_map, pbox_map, key_map, key_compress_map);

    printf("block = %d\n", block_index_max);

    for(int block_index = 0; block_index < block_index_max; block_index ++){
        Data_memcpy(key, roundKey);
        Data_memcpy_option(input, roundOutput, block_index * 8, 0, 8);
        for(int r = 0; r < rounds; r++){
            printf("%d : %s\n", r, Data_tostring(roundOutput, str, sizeof(str)));
            Data_memcpy(roundOutput, roundInput);
            des_round(roundInput, expandData, roundKey, sbox_map, pbox_map, key_map, key_compress_map, roundOutput, r);}
        printf("%d : %s\n", rounds, Data_tostring(roundOutput, str, sizeof(str)));
        Data_memcpy_option(roundOutput, output, 0, block_index * 8, 8);}
    
    // 0 으로 초기화 후 free 하면 더 안전해짐.
    free(sbox_map);
    free(pbox_map);
    free(key_map);
    free(key_compress_map);
    Data_release_safe(roundInput);
    Data_release_safe(roundOutput);
    Data_release_safe(expandData);
    Data_release_safe(roundKey);}

void des_decrypt(pData input, pData key, pData output, int rounds){
   int block_index_max = input->size * input->typeSize / 8;
    pData expandData = Data_create_zeros(6,1);
    pData roundKey = Data_create_copy(key);
    pData finalRoundKey = Data_create_copy(key);
    pData roundInput = Data_create_zeros(8,1);
    pData roundOutput = Data_create_zeros(8,1);
    
    char *sbox_map = malloc(64);
    char *pbox_map = malloc(32);
    char *key_map = malloc(56);
    char *key_compress_map = malloc(48);
    init(key, sbox_map, pbox_map, key_map, key_compress_map);
    Data_memcpy(key, finalRoundKey);
    for(int r = 0; r < rounds; r++){
        key_mapping(finalRoundKey, key_map);
        leftShift(finalRoundKey, r);}

    for(int block_index = 0; block_index < block_index_max; block_index ++){
        Data_memcpy(finalRoundKey, roundKey);
        Data_memcpy_option(input, roundOutput, block_index * 8, 0, 8);
        for(int r = rounds - 1; r > -1; r--){
            printf("%d : %s\n", r, Data_tostring(roundOutput, str, sizeof(str)));
            Data_memcpy(roundOutput, roundInput);
            des_round_decrypt(roundInput, expandData, roundKey, sbox_map, pbox_map, key_map, key_compress_map, roundOutput, r);}
        printf("%d : %s\n", rounds, Data_tostring(roundOutput, str, sizeof(str)));
        Data_memcpy_option(roundOutput, output, 0, block_index * 8, 8);}

    
    // 0 으로 초기화 후 free 하면 더 안전해짐.
    free(sbox_map);
    free(pbox_map);
    free(key_map);
    free(key_compress_map);
    Data_release_safe(roundInput);
    Data_release_safe(roundOutput);
    Data_release_safe(expandData);
    Data_release_safe(finalRoundKey);
    Data_release_safe(roundKey);}

void init(pData key, char *sbox_map, char * pbox_map, char * key_map, char * key_compress_map){
    int *keydata = key->data;
    // 보안성 강화.
    srand(keydata[0]);
    for(int i = 0; i < 64; i++){
        sbox_map[i] = rand() & 0b1111;}
    for(int i = 0; i < 32; i++){
        pbox_map[i] = i;}
    for(int i = 0; i < 56; i++)
        key_map[i] = i;
    for(int i = 0; i < 48; i++)
        key_compress_map[i] = rand() % 56;
    
    char tmp = 0;
    int random_index = 0;
    for(int i = 0; i < 32; i++){
        random_index = (rand() % (32 - i)) + i;
        tmp = pbox_map[i];
        pbox_map[i] = pbox_map[random_index];
        pbox_map[random_index] = tmp;
    }

    for(int i = 0; i < 56; i++){
        random_index = (rand() % (56 - i)) + i;
        tmp = key_map[i];
        key_map[i] = key_map[random_index];
        key_map[random_index] = tmp;
    }
}

void des_round(pData roundInput, pData expandData, pData roundKey, char *sbox_map, char *pbox_map, char *key_map, char *key_compress_map, pData roundOutput, int r){
    expand(roundInput, expandData);
    key_mapping(roundKey, key_map);
    leftShift(roundKey, r);
    xor(key_compress_map, roundKey, expandData);
    sbox(expandData, sbox_map, roundOutput);
    pbox(pbox_map, roundOutput);
    swap(roundInput, roundOutput);}

void des_round_decrypt(pData roundInput, pData expandData, pData roundKey, char *sbox_map, char *pbox_map, char *key_map, char *key_compress_map, pData roundOutput, int r){
    expand_decrypt(roundInput, expandData);
    //leftShift(roundKey, r); 
    xor(key_compress_map, roundKey, expandData);
    leftShift_decrypt(roundKey, r);
    key_mapping_decrypt(roundKey, key_map);
    sbox_decrypt(expandData, sbox_map, roundOutput);
    pbox_decrypt(pbox_map, roundOutput);
    swap_decrypt(roundInput, roundOutput);}

void expand(pData roundInput, pData expandData){
    int round_index = -1;
    for(int expand_index = 0, expand_index_max = 48; expand_index < expand_index_max; expand_index++){
        Data_set_bit(expandData, expand_index, Data_get_bit(roundInput, ((expand_index - 1 - expand_index / 6  * 2) % 32) + 32));}}

void expand_decrypt(pData roundInput, pData expandData){
    int round_index = -1;
    for(int expand_index = 0, expand_index_max = 48; expand_index < expand_index_max; expand_index++){
        Data_set_bit(expandData, expand_index, Data_get_bit(roundInput, (expand_index - 1 - expand_index / 6 * 2) % 32));}}

void key_mapping(pData roundKey, char *key_map){
    pData tmpKey = Data_create_copy(roundKey);
    for(int i = 0; i < 56; i ++){
        Data_set_bit(roundKey, i, Data_get_bit(tmpKey, key_map[i]));
    }
    Data_release_safe(tmpKey);
}
void key_mapping_decrypt(pData roundKey, char *key_map){
    pData tmpKey = Data_create_copy(roundKey);
    for(int i = 0; i < 56; i ++){
        Data_set_bit(roundKey, key_map[i], Data_get_bit(tmpKey, i));
    }
    Data_release_safe(tmpKey);
}

void leftShift(pData roundKey, int r){
    int shift_count = 1;
    if(r == 1 || r == 2 || r == 9 || r == 16){
        shift_count = 2;}
    int left_overwrite = 0;
    int right_overwrite = 0;
    for(int i = 0; i < shift_count; i++){
        left_overwrite += Data_get_bit(roundKey, i);
        right_overwrite += Data_get_bit(roundKey, i + 32);
        left_overwrite = left_overwrite << 1;
        right_overwrite = right_overwrite << 1;}

    for(int i = 0; i < 28 - shift_count; i ++){
        Data_set_bit(roundKey, i, Data_get_bit(roundKey, (i + shift_count)));
        Data_set_bit(roundKey, i + 28, Data_get_bit(roundKey, (i + shift_count) + 28));}
    
    for(int i = 0; i < shift_count; i++){
        Data_set_bit(roundKey, 27 - i, left_overwrite & 1);
        Data_set_bit(roundKey, 56 - i, right_overwrite & 1);
        left_overwrite = left_overwrite >> 1;
        right_overwrite = right_overwrite >> 1;}}

void leftShift_decrypt(pData roundKey, int r){
    int shift_count = 1;
    if(r == 1 || r == 2 || r == 9 || r == 16){
        shift_count = 2;}
    int left_overwrite = 0;
    int right_overwrite = 0;
    for(int i = 0; i < shift_count; i++){
        left_overwrite += Data_get_bit(roundKey, 27 - i);
        right_overwrite += Data_get_bit(roundKey, 55 - i);
        left_overwrite = left_overwrite << 1;
        right_overwrite = right_overwrite << 1;}

    for(int i = 27; i > shift_count - 1; i --){
        Data_set_bit(roundKey, i, Data_get_bit(roundKey, (i - shift_count)));
        Data_set_bit(roundKey, i + 28, Data_get_bit(roundKey, (i - shift_count) + 28));}
    
    for(int i = 0; i < shift_count; i++){
        Data_set_bit(roundKey, i, (left_overwrite >> (shift_count - i - 1)) & 1);
        Data_set_bit(roundKey, i + 28, (right_overwrite >> (shift_count - i - 1)) & 1);}}

void xor(char *key_compress_map, pData roundKey, pData expandData){
    for(int i = 0; i < 48; i ++){
        Data_set_bit(expandData, i, Data_get_bit(expandData, i) ^ Data_get_bit(roundKey, key_compress_map[i]));
    }
}

void sbox(pData expandData, char *sbox_map, pData roundOutput){
    int value = 0;
    int value2 = 0;
    for(int i = 0; i < 8; i ++){
        value = Data_get_bit(expandData, 6 * i + 0);
        value += Data_get_bit(expandData, 6 * i + 1) << 1;
        value += Data_get_bit(expandData, 6 * i + 2) << 2;
        value += Data_get_bit(expandData, 6 * i + 3) << 3;
        value += Data_get_bit(expandData, 6 * i + 4) << 4;
        value += Data_get_bit(expandData, 6 * i + 5) << 5;
        if(value > 0b111111){
            value /= 0;
        }
        value2 = sbox_map[value];
        Data_set_bit(roundOutput, 4 * i + 32, value >> 0 & 1);
        Data_set_bit(roundOutput, 4 * i + 33, value >> 1 & 1);
        Data_set_bit(roundOutput, 4 * i + 34, value >> 2 & 1);
        Data_set_bit(roundOutput, 4 * i + 35, value >> 3 & 1);
    }
}

void sbox_decrypt(pData expandData, char *sbox_map, pData roundOutput){
    int value = 0;
    int value2 = 0;
    for(int i = 0; i < 8; i ++){
        value = Data_get_bit(expandData, 6 * i + 0);
        value += Data_get_bit(expandData, 6 * i + 1) << 1;
        value += Data_get_bit(expandData, 6 * i + 2) << 2;
        value += Data_get_bit(expandData, 6 * i + 3) << 3;
        value += Data_get_bit(expandData, 6 * i + 4) << 4;
        value += Data_get_bit(expandData, 6 * i + 5) << 5;
        if(value > 0b111111){
            value /= 0;
        }
        value2 = sbox_map[value];
        Data_set_bit(roundOutput, 4 * i + 0, value >> 0 & 1);
        Data_set_bit(roundOutput, 4 * i + 1, value >> 1 & 1);
        Data_set_bit(roundOutput, 4 * i + 2, value >> 2 & 1);
        Data_set_bit(roundOutput, 4 * i + 3, value >> 3 & 1);
    }
}

// 주의! roundOutput의 left가 손실됨. mapping할때 임시 저장소로 사용되기 때문에.
void pbox(char *pbox_map, pData roundOutput){
    int value = 0;
    for(int i = 0; i < 32; i ++){
        Data_set_bit(roundOutput, i, Data_get_bit(roundOutput, pbox_map[i] + 32));
    }
}
void pbox_decrypt(char *pbox_map, pData roundOutput){
    int value = 0;
    for(int i = 0; i < 32; i ++){
        Data_set_bit(roundOutput, i + 32, Data_get_bit(roundOutput, pbox_map[i]));
    }
}

void swap(pData roundInput, pData roundOutput){
    //printf("roundInput size : %d, roundOutput size : %d\n", roundInput->size * roundInput->typeSize, roundOutput->size * roundOutput->typeSize);
    for(int i = 0; i < 32; i ++){
        //printf("%s\n", Data_tostring(roundOutput, str, sizeof(str)));
        Data_set_bit(roundOutput, i + 32, Data_get_bit(roundOutput, i) ^ Data_get_bit(roundInput, i));
        Data_set_bit(roundOutput, i, Data_get_bit(roundInput, i + 32));
    }
}

void swap_decrypt(pData roundInput, pData roundOutput){
    for(int i = 0; i < 32; i ++){
        Data_set_bit(roundOutput, i, Data_get_bit(roundOutput, i + 32) ^ Data_get_bit(roundInput, i + 32));
        Data_set_bit(roundOutput, i + 32, Data_get_bit(roundInput, i));
    }
}