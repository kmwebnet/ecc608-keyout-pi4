#include "configure.h"

static uint8_t atecc608_configuration[] = {
    0xC0, 0x00, 0x00, 0x01, 0x8F, 0x20, 0xC4, 0x44, 0x87, 0x20, 0x87, 0x20, 0x8F, 0x0F, 0xC4, 0x00,
    0x9F, 0x0F, 0x82, 0x20, 0x0F, 0x0F, 0xC4, 0x44, 0x0F, 0x0F, 0x0F, 0x0F, 0x0F, 0x0F, 0x0F, 0x0F,
    0x0F, 0x0F, 0x0F, 0x0F, 0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0xFF,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0xD7, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0x06, 0x40, 0x00, 0x00, 0x00, 0x00,
    0x33, 0x00, 0x1c, 0x00, 0x13, 0x00, 0x13, 0x00, 0x7C, 0x00, 0x18, 0x00, 0x3C, 0x00, 0x33, 0x00,
    0x3C, 0x00, 0x3C, 0x00, 0x3C, 0x00, 0x30, 0x00, 0x3C, 0x00, 0x3C, 0x00, 0x3C, 0x00, 0x30, 0x00,
};


int atca_configure(ATCAIfaceCfg *cfg)
{
    ATCA_STATUS status;
    uint8_t buf[ATCA_BLOCK_SIZE];
    bool    lock = false;

    /* Initialize the interface */
    if (ATCA_SUCCESS != (status = atcab_init(cfg))) // modified to I2C structure
    {
        printf("Unable to initialize interface: %x\r\n", status);
        goto exit;
    }

    /* Check the config zone lock status */
    if (ATCA_SUCCESS != (status = atcab_is_locked(ATCA_ZONE_CONFIG, &lock)))
    {
        printf("Unable to get config lock status: %x\r\n", status);
        goto exit;
    }

    /* Program the configuration zone */
    if (!lock)
    {

        if (ATCA_SUCCESS != (status = atcab_write_bytes_zone(
            ATCA_ZONE_CONFIG, 0, 16, (uint8_t *)atecc608_configuration, sizeof(atecc608_configuration))));
        {
            goto exit;
        }


        /* Lock the config zone */
        if (ATCA_SUCCESS != (status = atcab_lock_config_zone()))
        {
            goto exit;
        }
    }

    /* Check data zone lock */
    if (ATCA_SUCCESS != (status = atcab_is_locked(LOCK_ZONE_DATA, &lock)))
    {
        goto exit;
    }

    /* Lock the data zone */
    if (!lock)
    {
        if (ATCA_SUCCESS != (status = atcab_lock_data_zone()))
        {
            goto exit;
        }

    /* get random and store as AES key at the first time*/
    if (ATCA_SUCCESS != (status = atcab_random(buf)))
    {
        goto exit;
    }

    if (ATCA_SUCCESS != (status = atcab_write_zone(ATCA_ZONE_DATA, 5,  0, 0,  buf, 32)))
    {
        goto exit;
    }

    status = ATCA_SUCCESS;

    }



exit:
    return status;
}