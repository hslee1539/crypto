#include <stdio.h>
#include "data.h"
#include  "symmetric_key/a5_1.h"

void main(void){
    long long key1 = 152380912978;
    char txt1[] = "Wow hihihihi";
    pData txt = Data_create(txt1, 1, sizeof(txt1));
    pData txt_en = Data_create_copy(txt);
    pData txt_de = Data_create_copy(txt_en);
    pData key = Data_create(&key1, sizeof(key1), 1);
    char out[] = "                                                                                                                                 ";
    
    printf("%s start\n", out);
    printf("%s origen\n", Data_tostring(txt_en, out, sizeof(out)));
    a5_1(txt, key, txt_en);
    printf("%s en\n", Data_tostring(txt_en, out, sizeof(out)));
    a5_1(txt_en, key, txt_de);
    printf("%s de\n", Data_tostring(txt_de, out, sizeof(out)));
}