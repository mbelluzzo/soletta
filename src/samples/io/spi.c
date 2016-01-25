/*
 * This file is part of the Soletta Project
 *
 * Copyright (C) 2016 Intel Corporation. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 *   * Redistributions of source code must retain the above copyright
 *     notice, this list of conditions and the following disclaimer.
 *   * Redistributions in binary form must reproduce the above copyright
 *     notice, this list of conditions and the following disclaimer in
 *     the documentation and/or other materials provided with the
 *     distribution.
 *   * Neither the name of Intel Corporation nor the names of its
 *     contributors may be used to endorse or promote products derived
 *     from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

/*
 * SPI example
 */

#include <errno.h>
#include <stdlib.h>

#include <sol-log.h>
#include <sol-mainloop.h>
#include <sol-spi.h>

static uint8_t tx_buf[] = { 'H', 'e', 'l', 'l', 'o' };
static uint8_t rx_buf[5];

static void
_tx_cb(void *data, struct sol_spi *spi, const uint8_t *tx, uint8_t *rx, ssize_t status)
{
    if (status != sizeof(tx_buf))
        SOL_INF("Transmission failed!");
    else
        SOL_INF("Transmission success!");

    sol_quit();
}

int
main(int argc, char *argv[])
{
    int ret = 0;
    unsigned int bus;
    struct sol_spi *spi;

    // For a more detailed explanation of the configuration options, please check the API docs
    struct sol_spi_config config = {
        .api_version = SOL_SPI_CONFIG_API_VERSION,
        .chip_select = 0,
        .mode = SOL_SPI_MODE_0,
        .frequency = 9600,
        .bits_per_word = 8
    };

    sol_init();

    if (argc < 2) {
        SOL_INF("Usage: %s <bus>", argv[0]);
        goto end;
    }

    bus = atoi(argv[1]);

    spi = sol_spi_open(bus, &config);
    SOL_NULL_CHECK_MSG_GOTO(spi, end, "Couldn't open spi device <%s>.", argv[1]);

    sol_spi_transfer(spi, tx_buf, rx_buf, sizeof(tx_buf), _tx_cb, NULL);

    ret = sol_run();

end:
    sol_shutdown();

    return ret;
}
