/****************************************************************************
 *
 *   Copyright (C) 2012-2019 PX4 Development Team. All rights reserved.
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

#pragma once

#include <mixer/Mixer/Mixer.hpp>

#include <mathlib/mathlib.h>
#include <math.h>
#include <matrix/math.hpp>

#define _MULTIROTOR_COUNT_ 4

/**
 * Supported multirotor geometries.
 *
 * Values are generated by the px_generate_mixers.py script and placed to mixer_multirotor_normalized.generated.h
 */
typedef uint8_t MultirotorGeometryUnderlyingType;
enum class MultirotorGeometry : MultirotorGeometryUnderlyingType;

/**
 * Multi-rotor mixer for pre-defined vehicle geometries.
 *
 * Collects four inputs (roll, pitch, yaw, thrust) and mixes them to
 * a set of outputs based on the configured geometry.
 */
class MultirotorMixer : public Mixer
{
public:
	/**

	 * Precalculated rotor mix.
	 */
	struct Rotor {
		float	roll_scale;		/**< scales roll for this rotor */
		float	pitch_scale;	/**< scales pitch for this rotor */
		float	yaw_scale;		/**< scales yaw for this rotor */
		float	thrust_scale;	/**< scales thrust for this rotor */
	};

    struct MultirotorStructure {
        float arm_angle[_MULTIROTOR_COUNT_]; //arm angle in direct sens
        float arm_length; //arm length
        int lookup_table[_MULTIROTOR_COUNT_];
        float tau_lift;
        float tau_drag;
        matrix::Matrix<float,4,_MULTIROTOR_COUNT_> D;
        matrix::Matrix<float,_MULTIROTOR_COUNT_,4> Dinv;

        float min_speed;
        float max_speed;
    };





	/**
	 * Constructor.
	 *
	 * @param control_cb		Callback invoked to read inputs.
	 * @param cb_handle		Passed to control_cb.
	 * @param geometry		The selected geometry.
	 * @param roll_scale		Scaling factor applied to roll inputs
	 *				compared to thrust.
	 * @param pitch_scale		Scaling factor applied to pitch inputs
	 *				compared to thrust.
	 * @param yaw_wcale		Scaling factor applied to yaw inputs compared
	 *				to thrust.
	 * @param idle_speed		Minimum rotor control output value; usually
	 *				tuned to ensure that rotors never stall at the
	 * 				low end of their control range.
	 */
	MultirotorMixer(ControlCallback control_cb, uintptr_t cb_handle, MultirotorGeometry geometry,
			float roll_scale, float pitch_scale, float yaw_scale, float idle_speed);

	/**
	 * Constructor (for testing).
	 *
	 * @param control_cb		Callback invoked to read inputs.
	 * @param cb_handle		Passed to control_cb.
	 * @param rotors		control allocation matrix
	 * @param rotor_count		length of rotors array (= number of motors)
	 */
	MultirotorMixer(ControlCallback control_cb, uintptr_t cb_handle, const Rotor *rotors, unsigned rotor_count);
	virtual ~MultirotorMixer();

	// no copy, assignment, move, move assignment
	MultirotorMixer(const MultirotorMixer &) = delete;
	MultirotorMixer &operator=(const MultirotorMixer &) = delete;
	MultirotorMixer(MultirotorMixer &&) = delete;
	MultirotorMixer &operator=(MultirotorMixer &&) = delete;

	/**
	 * Factory method.
	 *
	 * Given a pointer to a buffer containing a text description of the mixer,
	 * returns a pointer to a new instance of the mixer.
	 *
	 * @param control_cb		The callback to invoke when fetching a
	 *				control value.
	 * @param cb_handle		Handle passed to the control callback.
	 * @param buf			Buffer containing a text description of
	 *				the mixer.
	 * @param buflen		Length of the buffer in bytes, adjusted
	 *				to reflect the bytes consumed.
	 * @return			A new MultirotorMixer instance, or nullptr
	 *				if the text format is bad.
	 */
	static MultirotorMixer *from_text(Mixer::ControlCallback control_cb, uintptr_t cb_handle, const char *buf,
					  unsigned &buflen);

	unsigned		mix(float *outputs, unsigned space) override;

	uint16_t		get_saturation_status() override { return _saturation_status.value; }

	void			groups_required(uint32_t &groups) override { groups |= (1 << 0); }

	/**
	 * @brief      Update slew rate parameter. This tells the multicopter mixer
	 *             the maximum allowed change of the output values per cycle.
	 *             The value is only valid for one cycle, in order to have continuous
	 *             slew rate limiting this function needs to be called before every call
	 *             to mix().
	 *
	 * @param[in]  delta_out_max  Maximum delta output.
	 *
	 */
	void 			set_max_delta_out_once(float delta_out_max) override { _delta_out_max = delta_out_max; }

	unsigned		set_trim(float trim) override { return _rotor_count; }
	unsigned		get_trim(float *trim) override { return _rotor_count; }

	/**
	 * @brief      Sets the thrust factor used to calculate mapping from desired thrust to motor control signal output.
	 *
	 * @param[in]  val   The value
	 */
	void			set_thrust_factor(float val) override { _thrust_factor = math::constrain(val, 0.0f, 1.0f); }

	void 			set_airmode(Airmode airmode) override { _airmode = airmode; }

	unsigned		get_multirotor_count() override { return _rotor_count; }

	union saturation_status {
		struct {
			uint16_t valid		: 1; // 0 - true when the saturation status is used
			uint16_t motor_pos	: 1; // 1 - true when any motor has saturated in the positive direction
			uint16_t motor_neg	: 1; // 2 - true when any motor has saturated in the negative direction
			uint16_t roll_pos	: 1; // 3 - true when a positive roll demand change will increase saturation
			uint16_t roll_neg	: 1; // 4 - true when a negative roll demand change will increase saturation
			uint16_t pitch_pos	: 1; // 5 - true when a positive pitch demand change will increase saturation
			uint16_t pitch_neg	: 1; // 6 - true when a negative pitch demand change will increase saturation
			uint16_t yaw_pos	: 1; // 7 - true when a positive yaw demand change will increase saturation
			uint16_t yaw_neg	: 1; // 8 - true when a negative yaw demand change will increase saturation
			uint16_t thrust_pos	: 1; // 9 - true when a positive thrust demand change will increase saturation
			uint16_t thrust_neg	: 1; //10 - true when a negative thrust demand change will increase saturation
		} flags;
		uint16_t value;
	};

protected:
    /**
     * @brief Compute the rotor speed from the desired torque and thrust command(in SI unit) and store them in outputs.
     *
     * @param roll   Roll torque command (N.m)
     * @param pitch  Pitch torque command (N.m)
     * @param yaw    Yaw torque command (N.m)
     * @param thrust Thrust torque command (N)
     * @param roll Roll torque command (N.m)
     * @param outputs Motors speed (rad/s)
     * @param safe Bool to recompute or not in case of motor speed limit reached. If set, in case of limit reached, the thrust is recompute in order to keep each motor speed in their limits, the torque are not recomputed.
     * @param n Number of iteration allowed for the safe recomputation. If reached the function return false.
     * @return	Returns true if a solution was found in the number of iteration allowed(n), Otherwise it returns false.
     */
    bool compute_rotor_speed(float roll, float pitch, float yaw, float thrust, float *outputs, bool safe=true, int n=10);

    /**
     * @brief Apply linear coeficient to each motor speed to pass from rad/sec to px4 rotation coef...
     * @param outputs Motors speed (rad/s)
     */
    void compute_outputs(float *outputs);
    //void compute_pwm_motors(float* W, float* W_pwm, float offset); // faudrait ajouter des

    void set_structure_params();

private:

	float				_roll_scale{1.0f};
	float				_pitch_scale{1.0f};
	float				_yaw_scale{1.0f};
	float				_idle_speed{0.0f};
	float 				_delta_out_max{0.0f};
	float 				_thrust_factor{0.0f};

    float               _A_speed;
    float               _B_speed;



	Airmode				_airmode{Airmode::disabled};

	saturation_status		_saturation_status{};

	unsigned			_rotor_count;
    const Rotor			*_rotors;

    //coeficient to get approximate SI unit for the torque and thrust control command
    Rotor               _dynamic = {.roll_scale=2,
                                    .pitch_scale=2,
                                    .yaw_scale=1,
                                    .thrust_scale=1.5f*9.81f/0.55f};

    //this structure describe the drone structue to comute its allocation matrix
    MultirotorStructure _structure = {.arm_angle={1.04f, 2.1f, 4.18f, 5.25f},
                                      .arm_length=0.255f,
                                      .lookup_table={0, 2, 3, 1},
                                      .tau_lift=8.54858e-06,
                                      .tau_drag=1e-06,
                                      .min_speed=1,
                                      .max_speed=1200};


	float 				*_outputs_prev{nullptr};
	float 				*_tmp_array{nullptr};
};
