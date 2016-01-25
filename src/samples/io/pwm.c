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
 * PWM example
 *
 * Open a pwm channel to output a signal with 100ms of period and duty cycle
 * changing from 20 to 80% each second in 10% increments. An oscilloscope may
 * be necessary to check the output of this sample.
 */

#include <stdlib.h>

#include <sol-log.h>
#include <sol-mainloop.h>
#include <sol-pwm.h>

#define PERIOD 100000000 //100ms in ns
#define DUTY_STEP 10000000  //10ms in ns

static unsigned int curr_duty = 2; // 20% duty cycle

static bool
_on_timeout(void *data)
{
    struct sol_pwm *pwm = data;

    curr_duty++;
    if (curr_duty > 8)
        curr_duty = 2;

    if (!sol_pwm_set_duty_cycle(pwm, DUTY_STEP * curr_duty)) {
        SOL_WRN("Failed to set duty cycle to %u.", DUTY_STEP * curr_duty);
        sol_quit();
    }

    return true;
}

int
main(int argc, char *argv[])
{
    int device, channel, ret = 0;
    struct sol_pwm *pwm;
    struct sol_timeout *timer;

    // For a more detailed explanation of the configuration options, please check the API docs
    struct sol_pwm_config config = {
        .api_version = SOL_PWM_CONFIG_API_VERSION,
        .period_ns = PERIOD,
        .duty_cycle_ns = DUTY_STEP * curr_duty,
        .alignment = SOL_PWM_ALIGNMENT_LEFT,
        .polarity = SOL_PWM_POLARITY_NORMAL,
        .enabled = true
    };

    sol_init();

    if (argc < 3) {
        SOL_INF("Usage: %s <device> <channel>", argv[0]);
        goto end;
    }

    device = atoi(argv[1]);
    channel = atoi(argv[2]);

    pwm = sol_pwm_open(device, channel, &config);
    SOL_NULL_CHECK_MSG_GOTO(pwm, end, "Couldn't open pwm device <%s, %s>.",
        argv[1], argv[2]);

    timer = sol_timeout_add(1000, _on_timeout, pwm);

    ret = sol_run();

    if (timer)
        sol_timeout_del(timer);

end:
    sol_shutdown();

    return ret;
}
