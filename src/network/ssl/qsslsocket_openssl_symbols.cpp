/****************************************************************************
**
** Copyright (C) 2015 The Qt Company Ltd.
** Contact: http://www.qt.io/licensing/
**
** This file is part of the QtNetwork module of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:LGPL$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see http://www.qt.io/terms-conditions. For further
** information use the contact form at http://www.qt.io/contact-us.
**
** GNU Lesser General Public License Usage
** Alternatively, this file may be used under the terms of the GNU Lesser
** General Public License version 2.1 or version 3 as published by the Free
** Software Foundation and appearing in the file LICENSE.LGPLv21 and
** LICENSE.LGPLv3 included in the packaging of this file. Please review the
** following information to ensure the GNU Lesser General Public License
** requirements will be met: https://www.gnu.org/licenses/lgpl.html and
** http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
** As a special exception, The Qt Company gives you certain additional
** rights. These rights are described in The Qt Company LGPL Exception
** version 1.1, included in the file LGPL_EXCEPTION.txt in this package.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 3.0 as published by the Free Software
** Foundation and appearing in the file LICENSE.GPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU General Public License version 3.0 requirements will be
** met: http://www.gnu.org/copyleft/gpl.html.
**
** $QT_END_LICENSE$
**
****************************************************************************/


#include <QtCore/qglobal.h>

#ifndef QT_NO_OPENSSL

#include "qsslsocket_openssl_symbols_p.h"

#ifdef Q_OS_WIN
# include <qsystemlibrary_p.h>
#else
# include <QtCore/qlibrary.h>
#endif
#include <QtCore/qmutex.h>
#include <qmutexpool_p.h>
#include <QtCore/qdatetime.h>
#if defined(Q_OS_UNIX)
#include <QtCore/qdir.h>
#endif
#ifdef Q_OS_LINUX
#include <link.h>
#endif

QT_BEGIN_NAMESPACE

/*
    Note to maintainer:
    -------------------

    We load OpenSSL symbols dynamically. Because symbols are known to
    disappear, and signatures sometimes change, between releases, we need to
    be careful about how this is done. To ensure we don't end up dereferencing
    null function pointers, and continue running even if certain functions are
    missing, we define helper functions for each of the symbols we load from
    OpenSSL, all prefixed with "q_" (declared in
    qsslsocket_openssl_symbols_p.h). So instead of calling SSL_connect
    directly, we call q_SSL_connect, which is a function that checks if the
    actual SSL_connect fptr is null, and returns a failure if it is, or calls
    SSL_connect if it isn't.

    This requires a somewhat tedious process of declaring each function we
    want to call in OpenSSL thrice: once with the q_, in _p.h, once using the
    DEFINEFUNC macros below, and once in the function that actually resolves
    the symbols, below the DEFINEFUNC declarations below.

    There's one DEFINEFUNC macro declared for every number of arguments
    exposed by OpenSSL (feel free to extend when needed). The easiest thing to
    do is to find an existing entry that matches the arg count of the function
    you want to import, and do the same.

    The first macro arg is the function return type. The second is the
    verbatim name of the function/symbol. Then follows a list of N pairs of
    argument types with a variable name, and just the variable name (char *a,
    a, char *b, b, etc). Finally there's two arguments - a suitable return
    statement for the error case (for an int function, return 0 or return -1
    is usually right). Then either just "return" or DUMMYARG, the latter being
    for void functions.

    Note: Take into account that these macros and declarations are processed
    at compile-time, and the result depends on the OpenSSL headers the
    compiling host has installed, but the symbols are resolved at run-time,
    possibly with a different version of OpenSSL.
*/

#ifdef SSLEAY_MACROS
DEFINEFUNC3(void *, ASN1_dup, i2d_of_void *a, a, d2i_of_void *b, b, char *c, c, return 0, return)
#endif
DEFINEFUNC(long, ASN1_INTEGER_get, ASN1_INTEGER *a, a, return 0, return)
DEFINEFUNC(unsigned char *, ASN1_STRING_data, ASN1_STRING *a, a, return 0, return)
DEFINEFUNC(int, ASN1_STRING_length, ASN1_STRING *a, a, return 0, return)
DEFINEFUNC2(int, ASN1_STRING_to_UTF8, unsigned char **a, a, ASN1_STRING *b, b, return 0, return);
DEFINEFUNC4(long, BIO_ctrl, BIO *a, a, int b, b, long c, c, void *d, d, return -1, return)
DEFINEFUNC(int, BIO_free, BIO *a, a, return 0, return)
DEFINEFUNC(BIO *, BIO_new, BIO_METHOD *a, a, return 0, return)
DEFINEFUNC2(BIO *, BIO_new_mem_buf, void *a, a, int b, b, return 0, return)
DEFINEFUNC3(int, BIO_read, BIO *a, a, void *b, b, int c, c, return -1, return)
DEFINEFUNC(BIO_METHOD *, BIO_s_mem, void, DUMMYARG, return 0, return)
DEFINEFUNC3(int, BIO_write, BIO *a, a, const void *b, b, int c, c, return -1, return)
DEFINEFUNC(int, BN_num_bits, const BIGNUM *a, a, return 0, return)
DEFINEFUNC(int, CRYPTO_num_locks, DUMMYARG, DUMMYARG, return 0, return)
DEFINEFUNC(void, CRYPTO_set_locking_callback, void (*a)(int, int, const char *, int), a, return, DUMMYARG)
DEFINEFUNC(void, CRYPTO_set_id_callback, unsigned long (*a)(), a, return, DUMMYARG)
DEFINEFUNC(void, CRYPTO_free, void *a, a, return, DUMMYARG)
DEFINEFUNC(void, DSA_free, DSA *a, a, return, DUMMYARG)
#if  OPENSSL_VERSION_NUMBER < 0x00908000L
DEFINEFUNC3(X509 *, d2i_X509, X509 **a, a, unsigned char **b, b, long c, c, return 0, return)
#else // 0.9.8 broke SC and BC by changing this signature.
DEFINEFUNC3(X509 *, d2i_X509, X509 **a, a, const unsigned char **b, b, long c, c, return 0, return)
#endif
DEFINEFUNC2(char *, ERR_error_string, unsigned long a, a, char *b, b, return 0, return)
DEFINEFUNC(unsigned long, ERR_get_error, DUMMYARG, DUMMYARG, return 0, return)
DEFINEFUNC(const EVP_CIPHER *, EVP_des_ede3_cbc, DUMMYARG, DUMMYARG, return 0, return)
DEFINEFUNC3(int, EVP_PKEY_assign, EVP_PKEY *a, a, int b, b, char *c, c, return -1, return)
DEFINEFUNC2(int, EVP_PKEY_set1_RSA, EVP_PKEY *a, a, RSA *b, b, return -1, return)
DEFINEFUNC2(int, EVP_PKEY_set1_DSA, EVP_PKEY *a, a, DSA *b, b, return -1, return)
DEFINEFUNC(void, EVP_PKEY_free, EVP_PKEY *a, a, return, DUMMYARG)
DEFINEFUNC(DSA *, EVP_PKEY_get1_DSA, EVP_PKEY *a, a, return 0, return)
DEFINEFUNC(RSA *, EVP_PKEY_get1_RSA, EVP_PKEY *a, a, return 0, return)
DEFINEFUNC(EVP_PKEY *, EVP_PKEY_new, DUMMYARG, DUMMYARG, return 0, return)
DEFINEFUNC(int, EVP_PKEY_type, int a, a, return NID_undef, return)
DEFINEFUNC2(int, i2d_X509, X509 *a, a, unsigned char **b, b, return -1, return)
DEFINEFUNC(const char *, OBJ_nid2sn, int a, a, return 0, return)
DEFINEFUNC(int, OBJ_obj2nid, const ASN1_OBJECT *a, a, return NID_undef, return)
#ifdef SSLEAY_MACROS
DEFINEFUNC6(void *, PEM_ASN1_read_bio, d2i_of_void *a, a, const char *b, b, BIO *c, c, void **d, d, pem_password_cb *e, e, void *f, f, return 0, return)
DEFINEFUNC6(void *, PEM_ASN1_write_bio, d2i_of_void *a, a, const char *b, b, BIO *c, c, void **d, d, pem_password_cb *e, e, void *f, f, return 0, return)
#else
DEFINEFUNC4(DSA *, PEM_read_bio_DSAPrivateKey, BIO *a, a, DSA **b, b, pem_password_cb *c, c, void *d, d, return 0, return)
DEFINEFUNC4(RSA *, PEM_read_bio_RSAPrivateKey, BIO *a, a, RSA **b, b, pem_password_cb *c, c, void *d, d, return 0, return)
DEFINEFUNC7(int, PEM_write_bio_DSAPrivateKey, BIO *a, a, DSA *b, b, const EVP_CIPHER *c, c, unsigned char *d, d, int e, e, pem_password_cb *f, f, void *g, g, return 0, return)
DEFINEFUNC7(int, PEM_write_bio_RSAPrivateKey, BIO *a, a, RSA *b, b, const EVP_CIPHER *c, c, unsigned char *d, d, int e, e, pem_password_cb *f, f, void *g, g, return 0, return)
#endif
DEFINEFUNC4(DSA *, PEM_read_bio_DSA_PUBKEY, BIO *a, a, DSA **b, b, pem_password_cb *c, c, void *d, d, return 0, return)
DEFINEFUNC4(RSA *, PEM_read_bio_RSA_PUBKEY, BIO *a, a, RSA **b, b, pem_password_cb *c, c, void *d, d, return 0, return)
DEFINEFUNC2(int, PEM_write_bio_DSA_PUBKEY, BIO *a, a, DSA *b, b, return 0, return)
DEFINEFUNC2(int, PEM_write_bio_RSA_PUBKEY, BIO *a, a, RSA *b, b, return 0, return)
DEFINEFUNC2(void, RAND_seed, const void *a, a, int b, b, return, DUMMYARG)
DEFINEFUNC(int, RAND_status, void, DUMMYARG, return -1, return)
DEFINEFUNC(void, RSA_free, RSA *a, a, return, DUMMYARG)
DEFINEFUNC(int, sk_num, STACK *a, a, return -1, return)
DEFINEFUNC2(void, sk_pop_free, STACK *a, a, void (*b)(void*), b, return, DUMMYARG)
#if OPENSSL_VERSION_NUMBER >= 0x10000000L
DEFINEFUNC(void, sk_free, _STACK *a, a, return, DUMMYARG)
DEFINEFUNC2(void *, sk_value, STACK *a, a, int b, b, return 0, return)
#else
DEFINEFUNC(void, sk_free, STACK *a, a, return, DUMMYARG)
DEFINEFUNC2(char *, sk_value, STACK *a, a, int b, b, return 0, return)
#endif
DEFINEFUNC(int, SSL_accept, SSL *a, a, return -1, return)
DEFINEFUNC(int, SSL_clear, SSL *a, a, return -1, return)
DEFINEFUNC3(char *, SSL_CIPHER_description, SSL_CIPHER *a, a, char *b, b, int c, c, return 0, return)
DEFINEFUNC(int, SSL_connect, SSL *a, a, return -1, return)
#if OPENSSL_VERSION_NUMBER >= 0x00908000L
// 0.9.8 broke SC and BC by changing this function's signature.
DEFINEFUNC(int, SSL_CTX_check_private_key, const SSL_CTX *a, a, return -1, return)
#else
DEFINEFUNC(int, SSL_CTX_check_private_key, SSL_CTX *a, a, return -1, return)
#endif
DEFINEFUNC4(long, SSL_CTX_ctrl, SSL_CTX *a, a, int b, b, long c, c, void *d, d, return -1, return)
DEFINEFUNC(void, SSL_CTX_free, SSL_CTX *a, a, return, DUMMYARG)
#if OPENSSL_VERSION_NUMBER >= 0x10000000L
DEFINEFUNC(SSL_CTX *, SSL_CTX_new, const SSL_METHOD *a, a, return 0, return)
#else
DEFINEFUNC(SSL_CTX *, SSL_CTX_new, SSL_METHOD *a, a, return 0, return)
#endif
DEFINEFUNC2(int, SSL_CTX_set_cipher_list, SSL_CTX *a, a, const char *b, b, return -1, return)
DEFINEFUNC(int, SSL_CTX_set_default_verify_paths, SSL_CTX *a, a, return -1, return)
DEFINEFUNC3(void, SSL_CTX_set_verify, SSL_CTX *a, a, int b, b, int (*c)(int, X509_STORE_CTX *), c, return, DUMMYARG)
DEFINEFUNC2(void, SSL_CTX_set_verify_depth, SSL_CTX *a, a, int b, b, return, DUMMYARG)
DEFINEFUNC2(int, SSL_CTX_use_certificate, SSL_CTX *a, a, X509 *b, b, return -1, return)
DEFINEFUNC3(int, SSL_CTX_use_certificate_file, SSL_CTX *a, a, const char *b, b, int c, c, return -1, return)
DEFINEFUNC2(int, SSL_CTX_use_PrivateKey, SSL_CTX *a, a, EVP_PKEY *b, b, return -1, return)
DEFINEFUNC2(int, SSL_CTX_use_RSAPrivateKey, SSL_CTX *a, a, RSA *b, b, return -1, return)
DEFINEFUNC3(int, SSL_CTX_use_PrivateKey_file, SSL_CTX *a, a, const char *b, b, int c, c, return -1, return)
DEFINEFUNC(void, SSL_free, SSL *a, a, return, DUMMYARG)
#if OPENSSL_VERSION_NUMBER >= 0x00908000L
// 0.9.8 broke SC and BC by changing this function's signature.
DEFINEFUNC(STACK_OF(SSL_CIPHER) *, SSL_get_ciphers, const SSL *a, a, return 0, return)
#else
DEFINEFUNC(STACK_OF(SSL_CIPHER) *, SSL_get_ciphers, SSL *a, a, return 0, return)
#endif
#if OPENSSL_VERSION_NUMBER >= 0x10000000L
DEFINEFUNC(const SSL_CIPHER *, SSL_get_current_cipher, SSL *a, a, return 0, return)
#else
DEFINEFUNC(SSL_CIPHER *, SSL_get_current_cipher, SSL *a, a, return 0, return)
#endif
DEFINEFUNC2(int, SSL_get_error, SSL *a, a, int b, b, return -1, return)
DEFINEFUNC(STACK_OF(X509) *, SSL_get_peer_cert_chain, SSL *a, a, return 0, return)
DEFINEFUNC(X509 *, SSL_get_peer_certificate, SSL *a, a, return 0, return)
#if OPENSSL_VERSION_NUMBER >= 0x00908000L
// 0.9.8 broke SC and BC by changing this function's signature.
DEFINEFUNC(long, SSL_get_verify_result, const SSL *a, a, return -1, return)
#else
DEFINEFUNC(long, SSL_get_verify_result, SSL *a, a, return -1, return)
#endif
DEFINEFUNC(int, SSL_library_init, void, DUMMYARG, return -1, return)
DEFINEFUNC(void, SSL_load_error_strings, void, DUMMYARG, return, DUMMYARG)
DEFINEFUNC(SSL *, SSL_new, SSL_CTX *a, a, return 0, return)
#if OPENSSL_VERSION_NUMBER >= 0x0090806fL && !defined(OPENSSL_NO_TLSEXT)
DEFINEFUNC4(long, SSL_ctrl, SSL *a, a, int cmd, cmd, long larg, larg, void *parg, parg, return -1, return)
#endif
DEFINEFUNC3(int, SSL_read, SSL *a, a, void *b, b, int c, c, return -1, return)
DEFINEFUNC3(void, SSL_set_bio, SSL *a, a, BIO *b, b, BIO *c, c, return, DUMMYARG)
DEFINEFUNC(void, SSL_set_accept_state, SSL *a, a, return, DUMMYARG)
DEFINEFUNC(void, SSL_set_connect_state, SSL *a, a, return, DUMMYARG)
DEFINEFUNC(int, SSL_shutdown, SSL *a, a, return -1, return)
#if OPENSSL_VERSION_NUMBER >= 0x10000000L
#ifndef OPENSSL_NO_SSL2
DEFINEFUNC(const SSL_METHOD *, SSLv2_client_method, DUMMYARG, DUMMYARG, return 0, return)
#endif
DEFINEFUNC(const SSL_METHOD *, SSLv3_client_method, DUMMYARG, DUMMYARG, return 0, return)
DEFINEFUNC(const SSL_METHOD *, SSLv23_client_method, DUMMYARG, DUMMYARG, return 0, return)
DEFINEFUNC(const SSL_METHOD *, TLSv1_client_method, DUMMYARG, DUMMYARG, return 0, return)
#ifndef OPENSSL_NO_SSL2
DEFINEFUNC(const SSL_METHOD *, SSLv2_server_method, DUMMYARG, DUMMYARG, return 0, return)
#endif
DEFINEFUNC(const SSL_METHOD *, SSLv3_server_method, DUMMYARG, DUMMYARG, return 0, return)
DEFINEFUNC(const SSL_METHOD *, SSLv23_server_method, DUMMYARG, DUMMYARG, return 0, return)
DEFINEFUNC(const SSL_METHOD *, TLSv1_server_method, DUMMYARG, DUMMYARG, return 0, return)
#else
DEFINEFUNC(SSL_METHOD *, SSLv2_client_method, DUMMYARG, DUMMYARG, return 0, return)
DEFINEFUNC(SSL_METHOD *, SSLv3_client_method, DUMMYARG, DUMMYARG, return 0, return)
DEFINEFUNC(SSL_METHOD *, SSLv23_client_method, DUMMYARG, DUMMYARG, return 0, return)
DEFINEFUNC(SSL_METHOD *, TLSv1_client_method, DUMMYARG, DUMMYARG, return 0, return)
DEFINEFUNC(SSL_METHOD *, SSLv2_server_method, DUMMYARG, DUMMYARG, return 0, return)
DEFINEFUNC(SSL_METHOD *, SSLv3_server_method, DUMMYARG, DUMMYARG, return 0, return)
DEFINEFUNC(SSL_METHOD *, SSLv23_server_method, DUMMYARG, DUMMYARG, return 0, return)
DEFINEFUNC(SSL_METHOD *, TLSv1_server_method, DUMMYARG, DUMMYARG, return 0, return)
#endif
DEFINEFUNC3(int, SSL_write, SSL *a, a, const void *b, b, int c, c, return -1, return)
DEFINEFUNC2(int, X509_cmp, X509 *a, a, X509 *b, b, return -1, return)
#ifndef SSLEAY_MACROS
DEFINEFUNC(X509 *, X509_dup, X509 *a, a, return 0, return)
#endif
DEFINEFUNC(ASN1_OBJECT *, X509_EXTENSION_get_object, X509_EXTENSION *a, a, return 0, return)
DEFINEFUNC(void, X509_free, X509 *a, a, return, DUMMYARG)
DEFINEFUNC2(X509_EXTENSION *, X509_get_ext, X509 *a, a, int b, b, return 0, return)
DEFINEFUNC(int, X509_get_ext_count, X509 *a, a, return 0, return)
DEFINEFUNC4(void *, X509_get_ext_d2i, X509 *a, a, int b, b, int *c, c, int *d, d, return 0, return)
DEFINEFUNC(X509_NAME *, X509_get_issuer_name, X509 *a, a, return 0, return)
DEFINEFUNC(X509_NAME *, X509_get_subject_name, X509 *a, a, return 0, return)
DEFINEFUNC(int, X509_verify_cert, X509_STORE_CTX *a, a, return -1, return)
DEFINEFUNC(int, X509_NAME_entry_count, X509_NAME *a, a, return 0, return)
DEFINEFUNC2(X509_NAME_ENTRY *, X509_NAME_get_entry, X509_NAME *a, a, int b, b, return 0, return)
DEFINEFUNC(ASN1_STRING *, X509_NAME_ENTRY_get_data, X509_NAME_ENTRY *a, a, return 0, return)
DEFINEFUNC(ASN1_OBJECT *, X509_NAME_ENTRY_get_object, X509_NAME_ENTRY *a, a, return 0, return)
DEFINEFUNC(EVP_PKEY *, X509_PUBKEY_get, X509_PUBKEY *a, a, return 0, return)
DEFINEFUNC(void, X509_STORE_free, X509_STORE *a, a, return, DUMMYARG)
DEFINEFUNC(X509_STORE *, X509_STORE_new, DUMMYARG, DUMMYARG, return 0, return)
DEFINEFUNC2(int, X509_STORE_add_cert, X509_STORE *a, a, X509 *b, b, return 0, return)
DEFINEFUNC(void, X509_STORE_CTX_free, X509_STORE_CTX *a, a, return, DUMMYARG)
DEFINEFUNC4(int, X509_STORE_CTX_init, X509_STORE_CTX *a, a, X509_STORE *b, b, X509 *c, c, STACK_OF(X509) *d, d, return -1, return)
DEFINEFUNC2(int, X509_STORE_CTX_set_purpose, X509_STORE_CTX *a, a, int b, b, return -1, return)
DEFINEFUNC(int, X509_STORE_CTX_get_error, X509_STORE_CTX *a, a, return -1, return)
DEFINEFUNC(int, X509_STORE_CTX_get_error_depth, X509_STORE_CTX *a, a, return -1, return)
DEFINEFUNC(X509 *, X509_STORE_CTX_get_current_cert, X509_STORE_CTX *a, a, return 0, return)
DEFINEFUNC(STACK_OF(X509) *, X509_STORE_CTX_get_chain, X509_STORE_CTX *a, a, return 0, return)
DEFINEFUNC(X509_STORE_CTX *, X509_STORE_CTX_new, DUMMYARG, DUMMYARG, return 0, return)
#ifdef SSLEAY_MACROS
DEFINEFUNC2(int, i2d_DSAPrivateKey, const DSA *a, a, unsigned char **b, b, return -1, return)
DEFINEFUNC2(int, i2d_RSAPrivateKey, const RSA *a, a, unsigned char **b, b, return -1, return)
DEFINEFUNC3(RSA *, d2i_RSAPrivateKey, RSA **a, a, unsigned char **b, b, long c, c, return 0, return)
DEFINEFUNC3(DSA *, d2i_DSAPrivateKey, DSA **a, a, unsigned char **b, b, long c, c, return 0, return)
#endif
DEFINEFUNC(void, OPENSSL_add_all_algorithms_noconf, void, DUMMYARG, return, DUMMYARG)
DEFINEFUNC(void, OPENSSL_add_all_algorithms_conf, void, DUMMYARG, return, DUMMYARG)
DEFINEFUNC3(int, SSL_CTX_load_verify_locations, SSL_CTX *ctx, ctx, const char *CAfile, CAfile, const char *CApath, CApath, return 0, return)
DEFINEFUNC(long, SSLeay, void, DUMMYARG, return 0, return)


bool q_resolveOpenSslSymbols()
{
#ifdef QT_NO_OPENSSL
    return false;
#endif
    return true;
}

//==============================================================================
// contributed by Jay Case of Sarvega, Inc.; http://sarvega.com/
// Based on X509_cmp_time() for intitial buffer hacking.
//==============================================================================
QDateTime q_getTimeFromASN1(const ASN1_TIME *aTime)
{
    size_t lTimeLength = aTime->length;
    char *pString = (char *) aTime->data;

    if (aTime->type == V_ASN1_UTCTIME) {

        char lBuffer[24];
        char *pBuffer = lBuffer;

        if ((lTimeLength < 11) || (lTimeLength > 17))
            return QDateTime();

        memcpy(pBuffer, pString, 10);
        pBuffer += 10;
        pString += 10;

        if ((*pString == 'Z') || (*pString == '-') || (*pString == '+')) {
            *pBuffer++ = '0';
            *pBuffer++ = '0';
        } else {
            *pBuffer++ = *pString++;
            *pBuffer++ = *pString++;
            // Skip any fractional seconds...
            if (*pString == '.') {
                pString++;
                while ((*pString >= '0') && (*pString <= '9'))
                    pString++;
            }
        }

        *pBuffer++ = 'Z';
        *pBuffer++ = '\0';

        time_t lSecondsFromUCT;
        if (*pString == 'Z') {
            lSecondsFromUCT = 0;
        } else {
            if ((*pString != '+') && (*pString != '-'))
                return QDateTime();

            lSecondsFromUCT = ((pString[1] - '0') * 10 + (pString[2] - '0')) * 60;
            lSecondsFromUCT += (pString[3] - '0') * 10 + (pString[4] - '0');
            lSecondsFromUCT *= 60;
            if (*pString == '-')
                lSecondsFromUCT = -lSecondsFromUCT;
        }

        tm lTime;
        lTime.tm_sec = ((lBuffer[10] - '0') * 10) + (lBuffer[11] - '0');
        lTime.tm_min = ((lBuffer[8] - '0') * 10) + (lBuffer[9] - '0');
        lTime.tm_hour = ((lBuffer[6] - '0') * 10) + (lBuffer[7] - '0');
        lTime.tm_mday = ((lBuffer[4] - '0') * 10) + (lBuffer[5] - '0');
        lTime.tm_mon = (((lBuffer[2] - '0') * 10) + (lBuffer[3] - '0')) - 1;
        lTime.tm_year = ((lBuffer[0] - '0') * 10) + (lBuffer[1] - '0');
        if (lTime.tm_year < 50)
            lTime.tm_year += 100; // RFC 2459

        QDate resDate(lTime.tm_year + 1900, lTime.tm_mon + 1, lTime.tm_mday);
        QTime resTime(lTime.tm_hour, lTime.tm_min, lTime.tm_sec);

        QDateTime result(resDate, resTime, Qt::UTC);
        result = result.addSecs(lSecondsFromUCT);
        return result;

    } else if (aTime->type == V_ASN1_GENERALIZEDTIME) {

        if (lTimeLength < 15)
            return QDateTime(); // hopefully never triggered

        // generalized time is always YYYYMMDDHHMMSSZ (RFC 2459, section 4.1.2.5.2)
        tm lTime;
        lTime.tm_sec = ((pString[12] - '0') * 10) + (pString[13] - '0');
        lTime.tm_min = ((pString[10] - '0') * 10) + (pString[11] - '0');
        lTime.tm_hour = ((pString[8] - '0') * 10) + (pString[9] - '0');
        lTime.tm_mday = ((pString[6] - '0') * 10) + (pString[7] - '0');
        lTime.tm_mon = (((pString[4] - '0') * 10) + (pString[5] - '0'));
        lTime.tm_year = ((pString[0] - '0') * 1000) + ((pString[1] - '0') * 100) +
                        ((pString[2] - '0') * 10) + (pString[3] - '0');

        QDate resDate(lTime.tm_year, lTime.tm_mon, lTime.tm_mday);
        QTime resTime(lTime.tm_hour, lTime.tm_min, lTime.tm_sec);

        QDateTime result(resDate, resTime, Qt::UTC);
        return result;

    } else {
        qWarning("unsupported date format detected");
        return QDateTime();
    }

}

QT_END_NAMESPACE

#endif // QT_NO_OPENSSL


