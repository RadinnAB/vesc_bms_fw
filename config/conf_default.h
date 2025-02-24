// This file is autogenerated by VESC Tool

#ifndef CONF_DEFAULT_H_
#define CONF_DEFAULT_H_

// BMS ID
#ifndef CONF_CONTROLLER_ID
#define CONF_CONTROLLER_ID 104
#endif

// CAN Status Rate
#ifndef CONF_SEND_CAN_STATUS_RATE_HZ
#define CONF_SEND_CAN_STATUS_RATE_HZ 10
#endif

// CAN Baud Rate
#ifndef CONF_CAN_BAUD_RATE
#define CONF_CAN_BAUD_RATE 2
#endif

// Max Balance Channels
#ifndef CONF_MAX_BAL_CH
#define CONF_MAX_BAL_CH 11
#endif

// Distributed Balancing
#ifndef CONF_DIST_BAL
#define CONF_DIST_BAL 1
#endif

// Balancing Mode
#ifndef CONF_BALANCE_MODE
#define CONF_BALANCE_MODE 2
#endif

// Number of Cells
#ifndef CONF_CELL_NUM
#define CONF_CELL_NUM 12
#endif

// First Cell Index
#ifndef CONF_CELL_FIRST_INDEX
#define CONF_CELL_FIRST_INDEX 0
#endif

// Balance Starting Voltage
#ifndef CONF_BALANCE_START
#define CONF_BALANCE_START 0.015
#endif

// Balance End Voltage
#ifndef CONF_BALANCE_END
#define CONF_BALANCE_END 0.008
#endif

// Charge Start Voltage
#ifndef CONF_CHARGE_START
#define CONF_CHARGE_START 4.16
#endif

// Charge End Voltage
#ifndef CONF_CHARGE_END
#define CONF_CHARGE_END 4.21
#endif

// Charge Minimum Voltage
#ifndef CONF_CHARGE_MIN
#define CONF_CHARGE_MIN 2
#endif

// Balance Minimum Voltage
#ifndef CONF_BALANCE_MIN
#define CONF_BALANCE_MIN 3.5
#endif

// Balance Maximum Current
#ifndef CONF_BALANCE_MAX_CURRENT
#define CONF_BALANCE_MAX_CURRENT 3
#endif

// Minimum Counter Current
#ifndef CONF_MIN_CURRENT_AH_WH_CNT
#define CONF_MIN_CURRENT_AH_WH_CNT 0.8
#endif

// Minimum Sleep Current
#ifndef CONF_MIN_CURRENT_SLEEP
#define CONF_MIN_CURRENT_SLEEP 0.5
#endif

// Charger Detect Threshold Voltage
#ifndef CONF_V_CHARGE_DETECT
#define CONF_V_CHARGE_DETECT 30
#endif

// Charge Max Temp
#ifndef CONF_T_CHARGE_MAX
#define CONF_T_CHARGE_MAX 45
#endif

// Current Measurement Mode
#ifndef CONF_I_MEASURE_MODE
#define CONF_I_MEASURE_MODE 0
#endif

// Current Shunt Resistance
#ifndef CONF_EXT_SHUNT_RES
#define CONF_EXT_SHUNT_RES 0.0002
#endif

// Current Shunt Amp Gain
#ifndef CONF_EXT_SHUNT_GAIN
#define CONF_EXT_SHUNT_GAIN 50
#endif

// External Precharge R Div Top
#ifndef CONF_EXT_PCH_R_TOP
#define CONF_EXT_PCH_R_TOP 520000
#endif

// External Precharge R Div Bottom
#ifndef CONF_EXT_PCH_R_BOTTOM
#define CONF_EXT_PCH_R_BOTTOM 10000
#endif

// Max Charge Current
#ifndef CONF_MAX_CHARGE_CURRENT
#define CONF_MAX_CHARGE_CURRENT 16
#endif

// Sleep Timeout Reset Time
#ifndef CONF_SLEEP_TIMEOUT_MS
#define CONF_SLEEP_TIMEOUT_MS 240000
#endif

// SoC Filter Constant
#ifndef CONF_SOC_FILTER_CONST
#define CONF_SOC_FILTER_CONST 1e-05
#endif

// Temp Bal Lim Start
#ifndef CONF_T_BAL_LIM_START
#define CONF_T_BAL_LIM_START 60
#endif

// Temp Bal Lim End
#ifndef CONF_T_BAL_LIM_END
#define CONF_T_BAL_LIM_END 75
#endif

// Charge Min Temp
#ifndef CONF_T_CHARGE_MIN
#define CONF_T_CHARGE_MIN 0
#endif

// Charge Temp Mon
#ifndef CONF_T_CHARGE_MON_EN
#define CONF_T_CHARGE_MON_EN 1
#endif

// CONF_DEFAULT_H_
#endif

