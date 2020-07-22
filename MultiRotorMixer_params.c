/****************************************************************************
 *
 *   Copyright (c) 2013-2019 PX4 Development Team. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in
 *    the documentation and/or other materials provided with the
 *    distribution.
 * 3. Neither the name PX4 nor the names of its contributors may be
 *    used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 * COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 * BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS
 * OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 * AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
 * ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 ****************************************************************************/

/**
 * @file MultiRotorMixer_params.c
 *
 * Parameters defined by the MultirotorMixer lib
 *
 * @author Alexandre Letalenet
 */

/**
 * Mass of the drone / custom Mixer
 *
 * Plop
 *
 * @group Mixer Output
 */
PARAM_DEFINE_FLOAT(MASS, 1.95f);

/**
 * Thrust coeff in N/(rad/sec)**2  / custom Mixer
 *
 * Plop
 *
 *
 * @group Mixer Output
 */
PARAM_DEFINE_FLOAT(CT, 0.0000074f);

/**
 * Torque coeff in N/(rad/sec)**2  / custom Mixer
 *
 * Plop
 *
 */
PARAM_DEFINE_FLOAT(CM, 0.000000174f);

/**
 * IXX inertia  / custom Mixer
 *
 * Plop
 *
 */
PARAM_DEFINE_FLOAT(IXX, 0.03f);

/**
 * IYY inertia / custom Mixer
 *
 * Plop
 *
 */
PARAM_DEFINE_FLOAT(IYY, 0.03f);

/**
 * IZZ inertia / custom Mixer
 *
 * Plop
 *
 */
PARAM_DEFINE_FLOAT(IZZ, 0.05f);

/**
 * MOTOR_CONST_IN_RAD_PER_SEC_PER_VOLT / Custom Mixer
 *
 * Plop
 *
 */
PARAM_DEFINE_FLOAT(MOTOR_KV, 1689.0f);

/**
 * BATTERY_MAX_VOLTAGE / Custom Mixer
 *
 * Plop
 *

 */
PARAM_DEFINE_FLOAT(BATTERY_MAX_VOLT, 16.8f);

/**
 * TILT_MOTOR_SCALING / Custom Mixer
 *
 * Plop
 *
 */
PARAM_DEFINE_FLOAT(TILT_MOT_SCALE, 1.57f);
