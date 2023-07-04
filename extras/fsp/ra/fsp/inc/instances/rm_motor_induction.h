/*******************************************************************************************************************//**
 * @addtogroup MOTOR_INDUCTION
 * @{
 **********************************************************************************************************************/

#ifndef RM_MOTOR_INDUCTION_H
#define RM_MOTOR_INDUCTION_H

/***********************************************************************************************************************
 * Includes
 **********************************************************************************************************************/
#include "bsp_api.h"
#include "rm_motor_api.h"
#include "rm_motor_speed.h"
#include "rm_motor_current.h"

/* Common macro for FSP header files. There is also a corresponding FSP_FOOTER macro at the end of this file. */
FSP_HEADER

/***********************************************************************************************************************
 * Macro definitions
 **********************************************************************************************************************/

/***********************************************************************************************************************
 * Typedef definitions
 **********************************************************************************************************************/
typedef enum  e_motor_induction_ctrl
{
    MOTOR_INDUCTION_CTRL_STOP  = 0,    ///< Stop mode
    MOTOR_INDUCTION_CTRL_RUN   = 1,    ///< Run mode
    MOTOR_INDUCTION_CTRL_ERROR = 2,    ///< Error mode
} motor_induction_ctrl_t;

typedef enum  e_motor_induction_ctrl_event
{
    MOTOR_INDUCTION_CTRL_EVENT_STOP  = 0, ///< Stop event
    MOTOR_INDUCTION_CTRL_EVENT_RUN   = 1, ///< Run event
    MOTOR_INDUCTION_CTRL_EVENT_ERROR = 2, ///< Error event
    MOTOR_INDUCTION_CTRL_EVENT_RESET = 3  ///< Reset event
} motor_induction_ctrl_event_t;

typedef struct  rm_motor_induction_statemachine
{
    motor_induction_ctrl_t       status;        ///< The current system status
    motor_induction_ctrl_t       next_status;   ///< The next system status
    motor_induction_ctrl_event_t current_event; ///< The current event index

    uint16_t u2_error_status;                   ///< The error information
} motor_induction_statemachine_t;

typedef struct st_motor_induction_extended_cfg
{
    float f_overcurrent_limit;         ///< Over-current limit [A]
    float f_overvoltage_limit;         ///< Over-voltage limit [V]
    float f_overspeed_limit;           ///< Over-speed limit [rpm]
    float f_lowvoltage_limit;          ///< Low-voltage limit [V]
} motor_induction_extended_cfg_t;

typedef struct st_motor_induction_instance_ctrl
{
    uint32_t open;                            ///< Used to determine if the channel is configured

    uint16_t u2_error_info;                   ///< Happened error

    motor_induction_statemachine_t st_statem; ///< Statemachine structure

    /* Speed control <=> Current control interface */
    motor_speed_input_t    st_speed_input;    ///< Speed input data buffer
    motor_speed_output_t   st_speed_output;   ///< Speed output data buffer
    motor_current_input_t  st_current_input;  ///< Current input data buffer
    motor_current_output_t st_current_output; ///< Current output data buffer

    motor_cfg_t const * p_cfg;                ///< Pointer of configuration structure
} motor_induction_instance_ctrl_t;

/**********************************************************************************************************************
 * Exported global variables
 **********************************************************************************************************************/

/** @cond INC_HEADER_DEFS_SEC */
/** Filled in interface API structure for this instance. */
extern const motor_api_t g_motor_on_motor_induction;

/** @endcond */

/***********************************************************************************************************************
 * Exported global functions (to be accessed by other files)
 ***********************************************************************************************************************/

/**********************************************************************************************************************
 * Public function prototypes
 **********************************************************************************************************************/
fsp_err_t RM_MOTOR_INDUCTION_Open(motor_ctrl_t * const p_ctrl, motor_cfg_t const * const p_cfg);

fsp_err_t RM_MOTOR_INDUCTION_Close(motor_ctrl_t * const p_ctrl);

fsp_err_t RM_MOTOR_INDUCTION_Run(motor_ctrl_t * const p_ctrl);

fsp_err_t RM_MOTOR_INDUCTION_Stop(motor_ctrl_t * const p_ctrl);

fsp_err_t RM_MOTOR_INDUCTION_Reset(motor_ctrl_t * const p_ctrl);

fsp_err_t RM_MOTOR_INDUCTION_ErrorSet(motor_ctrl_t * const p_ctrl, motor_error_t const error);

fsp_err_t RM_MOTOR_INDUCTION_SpeedSet(motor_ctrl_t * const p_ctrl, float const speed_rpm);

fsp_err_t RM_MOTOR_INDUCTION_PositionSet(motor_ctrl_t * const                      p_ctrl,
                                         motor_speed_position_data_t const * const p_position);

fsp_err_t RM_MOTOR_INDUCTION_StatusGet(motor_ctrl_t * const p_ctrl, uint8_t * const p_status);

fsp_err_t RM_MOTOR_INDUCTION_AngleGet(motor_ctrl_t * const p_ctrl, float * const p_angle_rad);

fsp_err_t RM_MOTOR_INDUCTION_SpeedGet(motor_ctrl_t * const p_ctrl, float * const p_speed_rpm);

fsp_err_t RM_MOTOR_INDUCTION_ErrorCheck(motor_ctrl_t * const p_ctrl, uint16_t * const p_error);

fsp_err_t RM_MOTOR_INDUCTION_WaitStopFlagGet(motor_ctrl_t * const p_ctrl, motor_wait_stop_flag_t * const p_flag);

/*******************************************************************************************************************//**
 * @} (end addtogroup MOTOR_INDUCTION)
 **********************************************************************************************************************/

/* Common macro for SSP header files. There is also a corresponding FSP_HEADER macro at the top of this file. */
FSP_FOOTER

#endif                                 // RM_MOTOR_INDUCTION_H
