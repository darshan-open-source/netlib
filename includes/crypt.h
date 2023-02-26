#include<iostream>
#include<openssl/evp.h>
#include <openssl/provider.h>
const char* getHash(std::string filename){

    
    BIO *b = BIO_new_file(filename.c_str(), "r");
    unsigned char digest[EVP_MAX_MD_SIZE];
    std::string sp;
    EVP_MD_CTX *ctx;
    ctx = EVP_MD_CTX_new();
    const EVP_MD *md;
    unsigned int len;
    md = EVP_sha256();
    
    EVP_DigestInit_ex(ctx, md, NULL);

    if (b != NULL)
    {
        while (BIO_eof(b) != 1)
        {
            char m[20480];
            size_t y;
            BIO_read_ex(b, (void *)m, 20480, &y);
            EVP_DigestUpdate(ctx, m, y);
        }

        EVP_DigestFinal_ex(ctx, digest, &len);
        for (unsigned int i = 0; i < len; i++)
        {
            char m[3];
            BIO_snprintf(m, 3, "%02x", digest[i]);
            sp.append(m);
        }
    }
    BIO_free(b);
    return sp.c_str();
}