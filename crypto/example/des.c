#include <stdio.h>
#include "../data.h"
#include "../symmetric_key/des.h"

void main(){
    pData origen = Data_create("wow wow", 1, 8);
    pData encryption = Data_create("       ", 1 , 8);
    pData decryption = Data_create_copy(encryption);
    pData key = Data_create("my des", 1, 7); // 1 * 7byte = 58bit
    printf("key : %p\n", key);
    char str[] = "                                                                                              ";
    printf("init state____________________\n");
    printf("key : %p\n", key);
    printf("origen      : %s\n", origen->data);
    printf("encryption  : %s\n", encryption->data);
    printf("decryption  : %s\n", decryption->data);
    printf("key : %p\n", key);
    printf("origen      : %s\n", Data_tostring(origen, str, sizeof(str)));
    printf("key : %p\n", key);
    printf("encryption  : %s\n", Data_tostring(encryption, str, sizeof(str)));
    printf("key : %p\n", key);
    printf("decryption  : %s\n", Data_tostring(decryption, str, sizeof(str)));
    printf("key : %p\n", key);

    printf("\nencrypt state____________________\n");
    printf("key : %p\n", key);
    des_encrypt(origen, key, encryption, 16);
    printf("origen      : %s\n", origen->data);
    printf("encryption  : %s\n", encryption->data);
    printf("decryption  : %s\n", decryption->data);
    printf("origen      : %s\n", Data_tostring(origen, str, sizeof(str)));
    printf("encryption  : %s\n", Data_tostring(encryption, str, sizeof(str)));
    printf("decryption  : %s\n", Data_tostring(decryption, str, sizeof(str)));

    printf("\ndecrypt state____________________\n");
    des_decrypt(encryption, key, decryption, 16);
    printf("origen      : %s\n", origen->data);
    printf("encryption  : %s\n", encryption->data);
    printf("decryption  : %s\n", decryption->data);
    printf("origen      : %s\n", Data_tostring(origen, str, sizeof(str)));
    printf("encryption  : %s\n", Data_tostring(encryption, str, sizeof(str)));
    printf("decryption  : %s\n", Data_tostring(decryption, str, sizeof(str)));

}