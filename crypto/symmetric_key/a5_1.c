#include "a5_1.h"

int a5_1_shift_key(pData key){
    //8
    //29
    //51
    //19
    //41
    //64
    int k8 = Data_get_bit(key, 8);
    int k0 = 0;

    int k29 = Data_get_bit(key, 29);
    int k19 = 0;

    int k51 = Data_get_bit(key, 51);
    int k41 = 0;

    int m = (k8 + k29 + k51) > 1;
    int tmp = 0;
    if(k8 == m){
        for(int i = 0; i < 18; i ++){
            tmp = Data_get_bit(key, i);
            Data_set_bit(key, i + 1, tmp);}
        k0 = Data_get_bit(key, 13) ^ Data_get_bit(key, 16) ^ Data_get_bit(key, 17) ^ Data_get_bit(key, 18);
        Data_set_bit(key, 0, k0);}

    if(k29 == m){
        for(int i = 19; i < 40; i ++){
            tmp = Data_get_bit(key, i);
            Data_set_bit(key, i + 1, tmp);}
        k19 = Data_get_bit(key, 39) ^ Data_get_bit(key, 40); 
        Data_set_bit(key, 19, k19);}

    if(k51 == m){
        for(int i = 41; i < 63; i ++){
            tmp = Data_get_bit(key, i);
            Data_set_bit(key, i + 1, tmp);}
        k41 = Data_get_bit(key, 48) ^ Data_get_bit(key, 61) ^ Data_get_bit(key, 62) ^ Data_get_bit(key, 63);
        Data_set_bit(key, 41, k41);}
    return Data_get_bit(key, 18) ^ Data_get_bit(key, 40) ^ Data_get_bit(key, 63);}

int a5_1(pData input, pData key, pData out){
    int index = 0;
    int index_max = input->size * input->typeSize * 8;
    pData tmpKey = Data_create_copy(key);

    
    for(; index < index_max; index++){
        Data_set_bit(out, index, a5_1_shift_key(tmpKey) ^ Data_get_bit(input, index));}

    Data_release_safe(tmpKey); // 0으로 값을 바꾸고 메모리 반환
    return 0;}