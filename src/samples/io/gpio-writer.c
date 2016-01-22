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
 * GPIO writer example
 *
 * Open an GPIO pin and change it's output value each half second.
 */

#include <stdlib.h>

#include <sol-gpio.h>
#include <sol-log.h>
#include <sol-mainloop.h>

static bool output = false;

static bool
_on_timeout(void *data)
{
    struct sol_gpio *gpio = data;

    if (!sol_gpio_write(gpio, !output)) {
        SOL_ERR("Failed to issue write operation. Exiting");
        sol_quit();
    }

    return true;
}

int
main(int argc, char *argv[])
{
    int pin, ret = 0;
    struct sol_gpio *gpio;
    struct sol_timeout *timer;

    // For a more detailed explanation of the configuration options, please check the API docs
    struct sol_gpio_config config = {
        .api_version = SOL_GPIO_CONFIG_API_VERSION,
        .dir = SOL_GPIO_DIR_IN,
        .out.value = false
    };

    sol_init();

    if (argc < 2) {
        SOL_INF("Usage: %s <pin>", argv[0]);
        goto end;
    }

    pin = atoi(argv[1]);

    gpio = sol_gpio_open(pin, &config);
    SOL_NULL_CHECK_MSG_GOTO(gpio, end, "Couldn't open gpio pin: %s.", argv[1]);

    timer = sol_timeout_add(500, _on_timeout, gpio);

    ret = sol_run();

    if (timer)
        sol_timeout_del(timer);

    sol_gpio_close(gpio);

end:
    sol_shutdown();

    return ret;
}
