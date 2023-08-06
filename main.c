#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <linux/string.h>

#include <openssl/sha.h>

#define BUF_SIZE 4096

#include "cryptoauthlib.h"

#include "host/atca_host.h"
#include "configure.h"

static volatile void * (*memset_func)(void *, int, size_t) =
	(volatile void * (*)(void *, int, size_t))&memset;

void memzero_explicit(void *s, size_t count)
{
	memset_func(s, 0, count);
}

void get_atecc608cfg(ATCAIfaceCfg *cfg)
{
//	config for Raspberry pi 4
		        cfg->iface_type             = ATCA_I2C_IFACE;
                cfg->devtype                = ATECC608A;
                cfg->atcai2c.address        = 0XC0;
                cfg->atcai2c.bus            = 1;
                cfg->atcai2c.baud           = 100000;
                cfg->wake_delay             = 1500;
                cfg->rx_retries             = 20;

return;
}

int main() {

    // read serial number from /proc/device-tree/serial-number
    FILE *fp = fopen("/proc/device-tree/serial-number", "r");
    if (fp == NULL) {
        perror("Error opening /proc/device-tree/serial-number");
        goto error;
    }

    char buffer[32];
    unsigned char khash[SHA256_DIGEST_LENGTH];
    while (fgets(buffer, sizeof(buffer), fp) != NULL) {

    // read mmcblk0p1 for making hash
    /*
    int fd = open("/dev/mmcblk0p1", O_RDONLY);
    if (fd == -1) {
        perror("Error opening /dev/mmcblk0p1");
        goto error;
    }

    unsigned char pbuffer[BUF_SIZE];
    */


    SHA256_CTX sha256;
    SHA256_Init(&sha256);

    SHA256_Update(&sha256, buffer, 16);
    memzero_explicit(buffer, sizeof(buffer));

    /*

    ssize_t bytesRead;
    while ((bytesRead = read(fd, pbuffer, sizeof(pbuffer))) > 0) {
        SHA256_Update(&sha256, pbuffer, bytesRead);
    }

    if (bytesRead == -1) {
        perror("Error reading /dev/mmcblk0p1");
        close(fd);
        goto error;
    }

    memzero_explicit(pbuffer, sizeof(pbuffer));
    */

    SHA256_Final(khash, &sha256);

    //close(fd);

    }

    fclose(fp);

// atecc608b access code

    ATCAIfaceCfg cfg;

    get_atecc608cfg(&cfg);

    ATCA_STATUS status = atcab_init(&cfg);
	
    if (status != ATCA_SUCCESS) {
        fprintf(stderr, "atcab_init() failed with ret=0x%08d\n", status);
        exit(1);
    }

    //AES key gen fuction
    atca_configure(&cfg);

    uint8_t encryptkey[] = {
    0x77 , 0x72 , 0x69 , 0x74 , 0x69 , 0x6e , 0x67 , 0x20 , 0x70 , 0x75 , 0x62 , 0x20 , 0x6b , 0x65 , 0x79 , 0x20 , 
    0x62 , 0x79 , 0x20 , 0x75 , 0x73 , 0x69 , 0x6e , 0x67 , 0x20 , 0x65 , 0x6e , 0x63 , 0x72 , 0x79 , 0x70 , 0x74 ,
    };

    if (ATCA_SUCCESS != (status = atcab_write_zone(ATCA_ZONE_DATA, 4,  0, 0,  encryptkey, ATCA_KEY_SIZE)))
    {
        fprintf(stderr, "writing IO Protection Key by using atcab_write_zone() on slot 4 failed: %x\r\n", status);
        exit(1);
    }
    
    atcab_release();
    status = atcab_init(&cfg);
	
    if (status != ATCA_SUCCESS) {
        fprintf(stderr, "atcab_init() failed with ret=0x%08d\n", status);
        exit(1);
    }

//key read function
    uint8_t keyread[32];

    if (ATCA_SUCCESS != (status = atcab_aes(AES_MODE_ENCRYPT, 5, khash, keyread)))
    {
        printf("atcab_aes_1 failed\n");
    }
    
    if (ATCA_SUCCESS != (status = atcab_aes(AES_MODE_ENCRYPT, 5, &khash[16], &keyread[16])))
    {
        printf("atcab_aes_2 failed\n");
    }
  
    memzero_explicit(khash, sizeof(khash));

    // write to stdout
    fwrite(keyread , sizeof(uint8_t) , 32, stdout);
    fflush(stdout);
    memzero_explicit(keyread, sizeof(keyread));

	atcab_release();

    return 0;

    error:
        fclose(fp);
        return 1;

    }
