#include <stdio.h>
#include <libakrypt.h>
#include <unistd.h>
#include <getopt.h>

void read_text_from_file(char* dest, char* filename);
void write_text_to_file(const char* src, char* filename);

int main(int argc, char** argv) {

    int opt = -1;
    char *password = malloc(256), *filename = malloc(252), *openText = malloc(20*1024), *cipherText = malloc(20*1024);
    
    memset(password, 0, 256); 
    memset(filename, 0, 252); 

    while((opt = getopt(argc, argv, ":p:f:")) != -1) {
        switch (opt) {
            case 'p':
                password = optarg;
                break;
            case 'f':
                filename = optarg;
                break;
            case ':':
                printf("option needs a value \n");
                break;
            case '?':
                printf("unknown option: %c\n", opt);
                break;
        }
    } 

    read_text_from_file(openText, filename);

    if (ak_libakrypt_create(NULL) != ak_true) {
        ak_libakrypt_destroy();
        return EXIT_FAILURE;
    }

    struct bckey ctx;
    ak_uint8 iv[8] = { 0x01, 0x02, 0x03, 0x04, 0x11, 0xaa, 0x4e, 0x12 };

    ak_bckey_create_magma(&ctx);
    ak_bckey_set_key_from_password(&ctx, password, 256, "randomiz", 8);
    
    int code = 1;
    if ((code = ak_bckey_ofb(&ctx, openText, cipherText, strlen(openText), iv, 8)) != ak_error_ok) {
        printf("error while encrypting");
        return code;
    };

    write_text_to_file(ak_ptr_to_hexstr(cipherText, strlen(cipherText), ak_false), filename);
    

    // decipher
    // char* a = malloc(20*1024);
    // if ((code = ak_bckey_ofb(&ctx, cipherText, a, strlen(cipherText), iv, 8)) != ak_error_ok) {
    //     printf("error while encrypting");
    //     return code;
    // };

    // printf("deciphertext: %s\n", a);

    ak_libakrypt_destroy();
    return EXIT_SUCCESS;
}

void read_text_from_file(char* dest, char* filename) {
    FILE* inputFile;
    if ((inputFile = fopen(filename, "r")) == NULL) {
        printf("Can't open file %s\n", filename);
        exit(EXIT_FAILURE);
    }

    unsigned i = 0;
    while ((dest[i] = fgetc(inputFile)) != EOF) { ++i; }

    dest[i] = '\0';

    fclose(inputFile);
}

void write_text_to_file(const char* src, char* filename) {
    char* ecnryptedFilename = malloc(256);
    char ext[4] = ".enc";
    memcpy(ecnryptedFilename, filename, 252);
    strcat(ecnryptedFilename, ext);

    FILE* outputFile;
    if ((outputFile = fopen(ecnryptedFilename, "w")) == NULL) {
        printf("Can't open file %s\n", filename);
        exit(EXIT_FAILURE);
    }

    int i = 0;
    while (src[i] != '\0') {
        fputc(src[i], outputFile);
        ++i;
    }

    fputc('\n', outputFile);

    fclose(outputFile);
}
