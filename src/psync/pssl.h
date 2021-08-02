/*
 * This file is part of the pCloud Console Client.
 *
 * (c) 2021 Serghei Iakovlev <egrep@protonmail.ch>
 * (c) 2013-2014 Anton Titov <anton@pcloud.com>
 * (c) 2013-2014 pCloud Ltd
 *
 * For the full copyright and license information, please view
 * the LICENSE file that was distributed with this source code.
 */

#ifndef PCLOUD_PSYNC_PSSL_H_
#define PCLOUD_PSYNC_PSSL_H_

#include "config.h"

#include "pcloudcc/psync/compiler.h"
#include "pcloudcc/psync/compat.h"

#define PSYNC_AES256_BLOCK_SIZE 16
#define PSYNC_AES256_KEY_SIZE 32

#if defined(P_SSL_OPENSSL)
#include "pssl-openssl.h"
#elif defined(P_SSL_MBEDTLS)
#include "pssl-mbedtls.h"
#elif defined(P_SSL_SECURETRANSPORT)
#include "pssl-securetransport.h"
#else
#error "Please specify SSL library to use"
#include "pssl-openssl.h"
#endif

extern PSYNC_THREAD int psync_ssl_errno;

#define PSYNC_SSL_ERR_WANT_READ  1
#define PSYNC_SSL_ERR_WANT_WRITE 2
#define PSYNC_SSL_ERR_UNKNOWN    3

#define PSYNC_SSL_NEED_FINISH (-2)
#define PSYNC_SSL_FAIL        (-1)
#define PSYNC_SSL_SUCCESS       0

typedef struct {
  size_t datalen;
  unsigned char data[];
} psync_encrypted_data_struct_t, *psync_encrypted_data_t;

typedef psync_encrypted_data_t psync_encrypted_symmetric_key_t;
typedef psync_encrypted_data_t psync_binary_rsa_key_t;

#define PSYNC_INVALID_ENC_SYM_KEY NULL
#define PSYNC_INVALID_ENCODER NULL
#define PSYNC_INVALID_BIN_RSA NULL

#define psync_ssl_alloc_binary_rsa psync_ssl_alloc_encrypted_symmetric_key

int psync_ssl_init();
void psync_ssl_memclean(void *ptr, size_t len);
int psync_ssl_connect(psync_socket_t sock, void **sslconn, const char *hostname);
int psync_ssl_connect_finish(void *sslconn, const char *hostname);
void psync_ssl_free(void *sslconn);
int psync_ssl_shutdown(void *sslconn);
size_t psync_ssl_pendingdata(void *sslconn);
int psync_ssl_read(void *sslconn, void *buf, int num);
int psync_ssl_write(void *sslconn, const void *buf, int num);

void psync_ssl_rand_strong(unsigned char *buf, int num);
void psync_ssl_rand_weak(unsigned char *buf, int num);

psync_rsa_t psync_ssl_gen_rsa(int bits);
void psync_ssl_free_rsa(psync_rsa_t rsa);
psync_rsa_publickey_t psync_ssl_rsa_get_public(psync_rsa_t rsa);
void psync_ssl_rsa_free_public(psync_rsa_publickey_t key);
psync_rsa_privatekey_t psync_ssl_rsa_get_private(psync_rsa_t rsa);
void psync_ssl_rsa_free_private(psync_rsa_privatekey_t key);
psync_binary_rsa_key_t psync_ssl_rsa_public_to_binary(psync_rsa_publickey_t rsa);
psync_binary_rsa_key_t psync_ssl_rsa_private_to_binary(psync_rsa_privatekey_t rsa);
psync_rsa_publickey_t psync_ssl_rsa_load_public(const unsigned char *keydata, size_t keylen);
psync_rsa_privatekey_t psync_ssl_rsa_load_private(const unsigned char *keydata, size_t keylen);
psync_rsa_publickey_t psync_ssl_rsa_binary_to_public(psync_binary_rsa_key_t bin);
psync_rsa_privatekey_t psync_ssl_rsa_binary_to_private(psync_binary_rsa_key_t bin);
void psync_ssl_rsa_free_binary(psync_binary_rsa_key_t bin);

psync_symmetric_key_t psync_ssl_gen_symmetric_key_from_pass(const char *password, size_t keylen, const unsigned char *salt, size_t saltlen, size_t iterations);
char *psync_ssl_derive_password_from_passphrase(const char *username, const char *passphrase);
psync_encrypted_symmetric_key_t psync_ssl_alloc_encrypted_symmetric_key(size_t len);
psync_encrypted_symmetric_key_t psync_ssl_copy_encrypted_symmetric_key(psync_encrypted_symmetric_key_t src);
void psync_ssl_free_symmetric_key(psync_symmetric_key_t key);

psync_encrypted_symmetric_key_t psync_ssl_rsa_encrypt_data(psync_rsa_publickey_t rsa, const unsigned char *data, size_t datalen);
psync_symmetric_key_t psync_ssl_rsa_decrypt_data(psync_rsa_privatekey_t rsa, const unsigned char *data, size_t datalen);
psync_encrypted_symmetric_key_t psync_ssl_rsa_encrypt_symmetric_key(psync_rsa_publickey_t rsa, const psync_symmetric_key_t key);
psync_symmetric_key_t psync_ssl_rsa_decrypt_symmetric_key(psync_rsa_privatekey_t rsa, const psync_encrypted_symmetric_key_t enckey);

psync_aes256_encoder psync_ssl_aes256_create_encoder(psync_symmetric_key_t key);
void psync_ssl_aes256_free_encoder(psync_aes256_encoder aes);
psync_aes256_encoder psync_ssl_aes256_create_decoder(psync_symmetric_key_t key);
void psync_ssl_aes256_free_decoder(psync_aes256_encoder aes);

#endif  /* PCLOUD_PSYNC_PSSL_H_ */
