CC := gcc

LDLIBS := -lssl -lcrypto -lpthread -lrt

DEFS= \
-D_GNU_SOURCE
INC= \
-Icryptoauthlib \
-Icryptoauthlib/lib \
-I ./

CFLAGS=-g -Wall $(INC) $(DEFS)

OBJS := $(patsubst %.c,%.o,$(wildcard *.c)) \
        $(patsubst %.c,%.o,$(wildcard cryptoauthlib/lib/*.c)) \
        $(patsubst %.c,%.o,$(wildcard cryptoauthlib/lib/atcacert/*.c)) \
        $(patsubst %.c,%.o,$(wildcard cryptoauthlib/lib/calib/*.c)) \
        $(patsubst %.c,%.o,$(wildcard cryptoauthlib/lib/crypto/*.c)) \
        $(patsubst %.c,%.o,$(wildcard cryptoauthlib/lib/crypto/hashes/*.c)) \
        $(patsubst %.c,%.o,$(wildcard cryptoauthlib/lib/host/*.c)) \
        $(patsubst %.c,%.o,$(wildcard cryptoauthlib/lib/hal/atca_hal.c)) \
        $(patsubst %.c,%.o,$(wildcard cryptoauthlib/lib/openssl/*.c)) \
        cryptoauthlib/lib/hal/atca_hal.o \
		cryptoauthlib/lib/hal/hal_linux.o \
        cryptoauthlib/lib/hal/hal_linux_i2c_userspace.o

keyout: $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^ $(LDLIBS)

clean:
	$(RM) keyout
	$(RM) -f $(OBJS)