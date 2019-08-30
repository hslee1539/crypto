#include <stdio.h>
#include "../data.h"
#include "../symmetric_key/a5_1.h"

void main(){
    pData origen = Data_create("hi hello", 1, 9);
    pData encryption = Data_create("        ", 1 , 9);
    pData decryption = Data_create_copy(encryption);
    pData key = Data_create("this is", 1, 8); // 1 * 8byte = 64bit
    char str[] = "                                                                                              ";
    printf("init state____________________\n");
    printf("origen      : %s\n", origen->data);
    printf("encryption  : %s\n", encryption->data);
    printf("decryption  : %s\n", decryption->data);
    printf("origen      : %s\n", Data_tostring(origen, str, sizeof(str)));
    printf("encryption  : %s\n", Data_tostring(encryption, str, sizeof(str)));
    printf("decryption  : %s\n", Data_tostring(decryption, str, sizeof(str)));

    printf("\nencrypt state____________________\n");
    a5_1(origen, key, encryption);
    printf("origen      : %s\n", origen->data);
    printf("encryption  : %s\n", encryption->data);
    printf("decryption  : %s\n", decryption->data);
    printf("origen      : %s\n", Data_tostring(origen, str, sizeof(str)));
    printf("encryption  : %s\n", Data_tostring(encryption, str, sizeof(str)));
    printf("decryption  : %s\n", Data_tostring(decryption, str, sizeof(str)));

    printf("\ndecrypt state____________________\n");
    a5_1(encryption, key, decryption);
    printf("origen      : %s\n", origen->data);
    printf("encryption  : %s\n", encryption->data);
    printf("decryption  : %s\n", decryption->data);
    printf("origen      : %s\n", Data_tostring(origen, str, sizeof(str)));
    printf("encryption  : %s\n", Data_tostring(encryption, str, sizeof(str)));
    printf("decryption  : %s\n", Data_tostring(decryption, str, sizeof(str)));

}