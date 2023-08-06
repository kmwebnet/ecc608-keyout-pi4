/* Auto-generated config file atca_config.h */
#ifndef ATCA_CONFIG_H
#define ATCA_CONFIG_H

/* Included HALS */

#define ATCA_HAL_I2C

/* Included device support */
#define ATCA_ATECC608_SUPPORT

/** Device Override - Library Assumes ATECC608B support in checks */
#define ATCA_ATECC608A_SUPPORT

/** Define to enable compatibility with legacy HALs
   (HALs with embedded device logic)*/


/** To use dynamically registered HALs without any of the provided
implementations its necessary to specify a value here - using this
in addition to specifying a provide hal may result in compilation
problems - it will need to be the same as the number of the hal options
selected plus however additional slots one would like */


/** Define if cryptoauthlib is to use the maximum execution time method */


/* \brief How long to wait after an initial wake failure for the POST to
 *         complete.
 * If Power-on self test (POST) is enabled, the self test will run on waking
 * from sleep or during power-on, which delays the wake reply.
 */
#ifndef ATCA_POST_DELAY_MSEC
#define ATCA_POST_DELAY_MSEC 25
#endif

/***************** Diagnostic & Test Configuration Section *****************/

/** Enable debug messages */
#define ATCA_PRINTF

/** Enable to build in test hooks */




/** Define Software Crypto Library to Use - if none are defined use the
    cryptoauthlib version where applicable */
#define ATCA_OPENSSL

#ifdef ATCA_WOLFSSL
#cmakedefine WOLFSSL_USER_SETTINGS
#ifdef WIN32
#define WIN32_LEAN_AND_MEAN
#endif
#endif

/** Additional Runtime Configuration */

/** Define to build atcab_ functions rather that defining them as macros */
#define ATCA_USE_ATCAB_FUNCTIONS

/** Define to enable older API forms that have been replaced */

/** Enable Strict ISO/C99 compliance */




/******************** Platform Configuration Section ***********************/


/** Define platform provided functions */
#define ATCA_PLATFORM_MALLOC      malloc
#define ATCA_PLATFORM_FREE        free
#define ATCA_PLATFORM_STRCASESTR  strcasestr
#define ATCA_PLATFORM_MEMSET_S    memset_s

#define atca_delay_ms   hal_delay_ms
#define atca_delay_us   hal_delay_us

#endif // ATCA_CONFIG_H
