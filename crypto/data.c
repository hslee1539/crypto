#include "./data.h"
#include <string.h>
#include <malloc.h>
//
// 중괄호를 잘 안보이게 함.
// 파이썬 코드처럼 보면 됨.
//

void Data_shiftLeft(pData input, int count, pData out){
    int value_index_max = input->size * input->typeSize;
    int value_index_start = (count / 8);
    int value_index_size = value_index_max - value_index_start;
    int value_index = 0;
    char *inputData = input->data;
    char *outData = out->data;
    char overflow = 0;

    count %= 8;

    for(; value_index < value_index_size; value_index++)
        outData[value_index] = 0;

    for(; value_index < value_index_max; value_index++){
        outData[value_index] = (( 0xFF & inputData[value_index - value_index_start]) << count) + overflow;
        overflow = ( 0xFF & inputData[value_index - value_index_start]) >> (8 - count);}}

void Data_shiftLeftWithOption(pData input, int count, pData out, int input_pos, int out_pos, int out_pos_max){
    int value_index_max = (out_pos_max - out_pos) / 8;
    int value_index_start = (count / 8);
    int value_index_size = value_index_max - value_index_start;
    int value_index = 0;
    char *inputData = input->data;
    char *outData = out->data;
    char overflow = 0;

    count %= 8;


}

void Data_shiftRight(pData input, int count, pData out){
    int value_index_max = input->size * input->typeSize;
    int value_index_start = count / 8;
    int value_index_size = value_index_max - value_index_start;
    int value_index = 0;
    char *inputData = input->data;
    char *outData = out->data;
    char overflow = 0;

    count &= 8;

    for(; value_index < value_index_size; value_index ++){
        outData[value_index] = (( 0xFF &inputData[value_index + value_index_start]) >> (8 - count)) + overflow;
        overflow = ( 0xFF &inputData[value_index + value_index_start]) << (count);}

    if(value_index < value_index_max){
        outData[value_index++] = overflow;}

    for(; value_index < value_index_max; value_index++)
        outData[value_index] = 0;}

void Data_xor(pData *inputs, int size, pData out){
    int byte_index_max = out->size * out->typeSize;
    char *x;
    char *y;
    char *o = out->data;

    x = inputs[0]->data;
    y = inputs[1]->data;
    for(int byte_index = 0; byte_index < byte_index_max; byte_index ++){
        o[byte_index] = x[byte_index] ^ y[byte_index];}
    y = o;
    for (int inputs_index = 1; inputs_index < size - 1; inputs_index++){
        x = inputs[inputs_index]->data;
        for (int byte_index = 0; byte_index < byte_index_max; byte_index++){
            o[byte_index] = x[byte_index] ^ y[byte_index];}}}

int Data_get_bit(pData data, int pos){
    char *datadata = data->data;
    return (datadata[pos / 8] >> (pos % 8)) & 0x01;}

void Data_set_bit(pData data, int pos, int value){
    value = value & 1;
    char *datadata = data->data;
    datadata[pos / 8] ^= value << (pos % 8);}

void Data_memcpy(pData source, pData out){
    memcpy(out->data, source->data, out->size * out->typeSize);}

void Data_memcpy_option(pData source, pData out, int source_pos, int out_pos, int copy_length){
    printf("data : %p, data + pos : %p, pos : %d, copy_length : %d\n", out->data, out->data + out_pos, out_pos, copy_length);
    char * dataPoint = out->data + out_pos;
    char * sourcePoint = source->data + source_pos;

    memcpy(dataPoint, sourcePoint, copy_length);}

pData Data_create_copy(pData source){
    void* data = malloc(source->size * source->typeSize);
    memcpy(data, source->data, source->size * source->typeSize);
    pData returnValue = malloc(sizeof(struct Data));
    returnValue->data = data;
    returnValue->size = source->size;
    returnValue->typeSize = source->typeSize;
    return returnValue;}

pData Data_create(void * source, int typesize, int size){
    void *data = malloc(size * typesize);
    memcpy(data, source, size * typesize);
    pData returnValue = malloc(sizeof(struct Data));
    returnValue->data = data;
    returnValue->size = size;
    returnValue->typeSize = typesize;
    return returnValue;}

pData Data_create_zeros(int typeSize, int size){
    void* data = calloc(size, typeSize);
    pData returnValue = malloc(sizeof(struct Data));
    returnValue->data = data;
    returnValue->size = size;
    returnValue->typeSize = typeSize;
    return returnValue;
}

void Data_release(pData data){
    free(data->data);
    free(data);}

void Data_release_safe(pData data){
    char *datadata = data->data;
    for(int i = 0, max = data->size * data->typeSize; i < max; i ++){
        datadata[i] = 0;}
    free(data->data);
    free(data);}

char* Data_tostring(pData data, char *out, int out_size){
    int data_str_size = data->size * data->typeSize * 8;
    int sw = out_size > data_str_size;
    int out_index_max = sw * data_str_size + (1 - sw) * out_size;
    char * datadata = data->data;
    char left;
    char right;

    out[--out_index_max] = 0;

    for(int out_index = 0; out_index < out_index_max; out_index += 8){
        out[out_index_max - out_index - 0] = '0' + (1 & (datadata[out_index/8] >> 0));
        out[out_index_max - out_index - 1] = '0' + (1 & (datadata[out_index/8] >> 1));
        out[out_index_max - out_index - 2] = '0' + (1 & (datadata[out_index/8] >> 2));
        out[out_index_max - out_index - 3] = '0' + (1 & (datadata[out_index/8] >> 3));
        out[out_index_max - out_index - 4] = '0' + (1 & (datadata[out_index/8] >> 4));
        out[out_index_max - out_index - 5] = '0' + (1 & (datadata[out_index/8] >> 5));
        out[out_index_max - out_index - 6] = '0' + (1 & (datadata[out_index/8] >> 6));
        out[out_index_max - out_index - 7] = '0' + (1 & (datadata[out_index/8] >> 7));}
    return out;}

