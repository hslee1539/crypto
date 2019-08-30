#pragma once

struct Data{
    void * data;
    int typeSize;
    int size;
};

typedef struct Data *pData;

void Data_shiftLeft(pData input, int count, pData out);
void Data_shiftLeftWithOption(pData input, int count, pData out, int input_pos, int out_pos, int out_pos_max);
void Data_shiftRight(pData input, int count, pData out);
void Data_shiftRightWithOption(pData input, int count, pData out, int input_pos, int out_pos, int out_pos_max);
void Data_xor(pData *inputs, int size, pData out);
int Data_get_bit(pData data, int pos);
void Data_set_bit(pData data, int pos, int value);

void Data_memcpy(pData source, pData out);
void Data_memcpy_option(pData source, pData out, int source_pos, int out_pos, int copy_length);
pData Data_create_copy(pData source);
pData Data_create(void * source, int typesize, int size);
pData Data_create_zeros(int typeSize, int size);
void Data_release(pData data);
void Data_release_safe(pData data);
char* Data_tostring(pData data, char *out, int out_size);