#include <stdio.h>
#include <libakrypt.h>
#include <unistd.h>
#include <getopt.h>
#include <errno.h>
#include <string.h>

void read_text_from_file(char* dest, char* filename);
void write_text_to_file(const char* src, char* filename);

/*! 
 * \author Pavel Zilbershteyn
 * \date 15.10.2024
 */
int main(int argc, char** argv) {

    int opt = -1;
    char *password = malloc(256), *filename = malloc(252), *openText = malloc(20*1024), *cipherText = malloc(20*1024);
    
    memset(password, 0, 256); 
    memset(filename, 0, 252); 

    /// \brief parse arguments
    while((opt = getopt(argc, argv, ":p:f:")) != -1) {
        switch (opt) {
            case 'p':
                strncpy(password, optarg, 255);
                password[255] = '\0';
                break;
            case 'f':
                strncpy(filename, optarg, 251);
                filename[251] = '\0';
                break;
            case ':':
                printf("option needs a value \n");
                break;
            case '?':
                printf("unknown option: %c\n", opt);
                break;
        }
    } 

    /// \brief read plaintext from file
    read_text_from_file(openText, filename);

    if (ak_libakrypt_create(NULL) != ak_true) { ///< Create libakrypt instance
        ak_libakrypt_destroy();
        return EXIT_FAILURE;
    }

    struct bckey ctx; ///< context
    ak_uint8 iv[8] = { 0x01, 0x02, 0x03, 0x04, 0x11, 0xaa, 0x4e, 0x12 }; ///< syncholink

    ak_bckey_create_magma(&ctx); ///< Create Magma key instance
    ak_bckey_set_key_from_password(&ctx, password, 256, "randomiz", 8); ///< Generate key based on password
    
    int code = 1;

    /// We encrypt the plaintext in the gamming mode with output feedback
    if ((code = ak_bckey_ofb(&ctx, openText, cipherText, strlen(openText), iv, 8)) != ak_error_ok) {
        printf("error while encrypting with code: %d", code);
        return code;
    };

    /// Save ciphertext to file with enc extension
    write_text_to_file(ak_ptr_to_hexstr(cipherText, strlen(cipherText), ak_false), filename);
    

    /// decipher example just in case
    // char* a = malloc(20*1024);
    // if ((code = ak_bckey_ofb(&ctx, cipherText, a, strlen(cipherText), iv, 8)) != ak_error_ok) {
    //     printf("error while encrypting");
    //     return code;
    // };

    // printf("deciphertext: %s\n", a);

    ak_libakrypt_destroy();
    return EXIT_SUCCESS;
}

/*!
 \brief The function of reading text from a file
 \details The function that allows you to read text 
 from a file named "filename" and write it to "dest".
 It is needed to get plaintext
 \param[out] dest Target memory area
 \param[in] filename The name of file with plaintext
*/
void read_text_from_file(char* dest, char* filename) {
    FILE* inputFile;
    if ((inputFile = fopen(filename, "r")) == NULL) {
        printf("Can't open file %s\n%s", filename, strerror(errno));
        exit(EXIT_FAILURE);
    }

    unsigned i = 0;
    while ((dest[i] = fgetc(inputFile)) != EOF) { ++i; }

    dest[i] = '\0';

    fclose(inputFile);
}

/*!
 \brief The function of writing text to a file
 \details The function that allows you to write the src text 
 to a file named "filename". Creates a file with the
 enc extension that will store the ciphertext
 \param[in] src The text to be written to the file
 \param[in] filename The name of file with plaintext
*/
void write_text_to_file(const char* src, char* filename) {
    char* ecnryptedFilename = malloc(256);
    char ext[4] = ".enc";
    memcpy(ecnryptedFilename, filename, 252);
    memcpy(ecnryptedFilename + strlen(filename), ext, 4);
    

    FILE* outputFile;
    if ((outputFile = fopen(ecnryptedFilename, "w")) == NULL) {
        printf("Can't open file %s\n%s", filename, strerror(errno));
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
