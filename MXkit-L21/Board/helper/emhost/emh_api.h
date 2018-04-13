/**
 ******************************************************************************
 * @file    emh_arg.c
 * @author  William Xu
 * @version V1.0.0
 * @date    9-Apr-2018
 * @brief   Emhost SDK aAPIs
 ******************************************************************************
 *
 * Copyright (c) 2009-2018 MXCHIP Co.,Ltd.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 ******************************************************************************
 */

#ifndef _EMH_API_H_
#define _EMH_API_H_

#include "mx_common.h"
#include "mx_debug.h"
#include "emh_arg.h"

#ifdef __cplusplus
extern "C"
{
#endif

/** \addtogroup emhost */
/** @{*/

/******************************************************************************
 *                             Module controller
 ******************************************************************************/

/** \addtogroup module */
/** @{*/

/**
 * @brief	Emhost SDK initialize and EMW module initialize
 * 
 * @return	status
 */
mx_status	emh_init				(void);

/**
 * @brief	Emhost SDK initialize and EMW module initialize
 * 
 * @return	none
 */
void		emh_runloop				(void);

/**
 * @brief	Software reset EMW module
 * 
 * @return	status
 */
mx_status	ewh_module_reset			(void);

/**
 * @brief	Read EMW module firmware version
 * 
 * @return	Point to firmware version string
 */
const char* emh_module_get_fw_version	(void);

/**
 * @brief	Read EMW module internal tick
 * 
 * @return	Tick value
 */
uint32_t    emh_module_get_tick			(void);

/**
 * @brief	Read EMW module internal tick
 * 
 * @return	Tick value
 */
mx_status	emh_module_restore_settings	(void);

/** \addtogroup event */
/** @{*/

/**
 * @brief	Receive an unknown event, or data format error
 * 
 * @return	none
 */
MX_WEAK void emh_ev_unknown(void);

/** @}*/
/** @}*/
/******************************************************************************
 *                              Wlan management
 ******************************************************************************/

/** \addtogroup wlan */
/** @{*/

/**
 * @brief		Read current configured AP's SSID and Password
 * 
 * @param[out] 	timeout: AT receive timeout, also changable by  #ATCmdParser_set_timeout
 * @param[out]	debug: Enable debug mode or not
 * 
 * @return	status
 */
mx_status			emh_wlan_get_para			(char ssid[33], char pwd[33]);

/**
 * @brief		Read wlan connection status
 * 
 * @return		Refer to #emh_arg_wlan_sta_t
 */
emh_arg_wlan_sta_t 	emh_wlan_get_sta_status		(void);

/** \addtogroup event */
/** @{*/

/**
 * @brief		Read wlan connection status
 * 
 * @return		Refer to #emh_arg_wlan_sta_t
 */
MX_WEAK void emh_ev_wlan(emh_arg_wlan_ev_t event);
/** @}*/
/** @}*/
/******************************************************************************
 *                              Alicoud SDS service
 ******************************************************************************/

/** \addtogroup alicloud_sds */
/** @{*/

typedef struct{
	struct{
		const char*				name;			/**< ALINKTEST */
		const char*				module;			/**< ALINKTEST_LUA */
		const char*				key;			/**< LIVING */
		const char*				secret;			/**< LIGHT */
		emh_arg_ali_format_t	format;			/**< ALINKTEST_LIVING_LIGHT_ALINK_TEST_LUA */
	} product_info;
	struct{
		const char*				type;
		const char*				category;
		const char*				manufacture;
	} dev_info ;
} emh_alisds_config_t;

typedef struct {
	int32_t len;
	uint8_t* data;
	emh_arg_ali_format_t format;
} emh_alisds_msg;

mx_status				emh_alisds_config			(const emh_alisds_config_t *config);
mx_status				emh_alisds_start_service	(void);
mx_status				emh_alisds_provision		(bool on);
emh_arg_ali_status_t	emh_alisds_get_status		(void);
mx_status				emh_alisds_set_cloud_atts	(emh_arg_ali_format_t format, uint8_t *data, int32_t len);
mx_status				emh_alisds_set_key			(const char *dev_key, const char *dev_sec);

mx_status				emh_alisds_unbound			(void);

MX_WEAK void emh_ev_alisds_connection(emh_arg_ali_conn_t conn);
MX_WEAK void emh_ev_alisds_get_local_atts(emh_alisds_msg *attrs);
MX_WEAK void emh_ev_alisds_set_local_atts(emh_alisds_msg *attrs);

/** @}*/
/** @}*/

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif