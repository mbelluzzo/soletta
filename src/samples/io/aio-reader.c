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
 * AIO reader example
 *
 * Open an AIO device/pin and output it's value to 'stdout' each half second.
 */

#include <stdlib.h>

#include <sol-aio.h>
#include <sol-log.h>
#include <sol-mainloop.h>

static void
_read_cb(void *cb_data, struct sol_aio *aio, int32_t ret)
{
    SOL_INT_CHECK(ret, < 0);
    SOL_INF("%d", ret);
}

static bool
_on_timeout(void *data)
{
    struct sol_aio *aio = data;

    if (sol_aio_busy(aio))
        return true;

    if (!sol_aio_get_value(aio, _read_cb, NULL)) {
        SOL_ERR("Failed to issue read operation. Exiting.");
        sol_quit();
    }

    return true;
}

int
main(int argc, char *argv[])
{
    int device, pin, ret = 0;
    unsigned int precision;
    struct sol_aio *aio;
    struct sol_timeout *timer;

    sol_init();

    if (argc < 4) {
        SOL_INF("Usage: %s <device> <pin> <precision>", argv[0]);
        goto end;
    }

    device = atoi(argv[1]);
    pin = atoi(argv[2]);
    precision = atoi(argv[3]);

    aio = sol_aio_open(device, pin, precision);
    SOL_NULL_CHECK_MSG_GOTO(aio, end, "Couldn't open aio device <%s, %s>.",
        argv[1], argv[2]);

    timer = sol_timeout_add(500, _on_timeout, aio);

    ret = sol_run();

    if (timer)
        sol_timeout_del(timer);

    sol_aio_close(aio);

end:
    sol_shutdown();

    return ret;
}
