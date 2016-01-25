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

#include <sol-uart.h>
#include <sol-log.h>
#include <sol-mainloop.h>

static void
_rx_cb(void *user_data, struct sol_uart *uart, unsigned char byte_read)
{
}

int
main(int argc, char *argv[])
{
    int ret = 0;
    struct sol_uart *uart;

    // For a more detailed explanation of the configuration options, please check the API docs
    struct sol_uart_config config = {
        .api_version = SOL_UART_CONFIG_API_VERSION,
        .baud_rate = SOL_UART_BAUD_RATE_9600,
        .parity = SOL_UART_PARITY_NONE,
        .stop_bits = SOL_UART_STOP_BITS_ONE,
        .rx_cb = _rx_cb,
        .rx_cb_user_data = NULL,
        .flow_control = false
    };

    sol_init();

    if (argc < 2) {
        SOL_INF("Usage: %s <port name>", argv[0]);
        goto end;
    }

    uart = sol_uart_open(argv[1], &config);
    SOL_NULL_CHECK_MSG_GOTO(uart, end, "Couldn't open uard device: %s.", argv[1]);

    ret = sol_run();

end:
    sol_shutdown();

    return ret;
}
