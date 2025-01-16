/***************************************************************************
 *                      COPYRIGHT NOTICE
 *             Copyright(C) 2024, D-Robotics Co., Ltd.
 *                     All rights reserved.
 ***************************************************************************/

#ifndef __HBN_ISP_API_H__
#define __HBN_ISP_API_H__

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include "hbn_api.h"

/* common define */
typedef enum enum_isp_mode {
	HBN_ISP_MODE_AUTO = 0,
	HBN_ISP_MODE_MANUAL,
	HBN_ISP_MODE_BUTT
} hbn_isp_mode_e;

typedef struct isp_param_range {
	float min;
	float max;
} hbn_isp_param_range_t;

typedef struct isp_zone_weight_s {
	uint32_t x;
	uint32_t y;
	uint32_t h;
	uint32_t w;
	float weight;
} hbn_isp_zone_weight_t;

/* module control */
typedef enum enum_isp_module_version {
	HBN_ISP_MODULE_V0 = 0,
	HBN_ISP_MODULE_V1,
	HBN_ISP_MODULE_BUTT
} hbn_isp_module_version_e;

typedef union tag_isp_module_ctrl_u {
	uint32_t u32Key;
	struct {
		uint32_t bit_ccm : 1; /* RW;[0] */
		uint32_t bit_cnr : 1; /* RW;[1] */
		uint32_t bit_cproc : 1; /* RW;[2] */
		uint32_t bit_dg : 1; /* RW;[3] */
		uint32_t bit_demosaic : 1; /* RW;[4] */
		uint32_t bit_dpcc : 1; /* RW;[5] */
		uint32_t bit_2dnr : 1; /* RW;[6] */
		uint32_t bit_3dnr : 1; /* RW;[7] */
		uint32_t bit_ee : 1; /* RW;[8] */
		uint32_t bit_lsc : 1; /* RW;[9] */
		uint32_t bit_lut3d : 1; /* RW;[10] */
		uint32_t bit_wdr: 1; /* RW;[11] */
		uint32_t bit_ynr : 1; /* RW;[12] */
		uint32_t bit_ge : 1; /* RW;[13] */
		uint32_t bit_wb : 1; /* RW;[14] */
	};
} isp_module_ctrl_u;

typedef struct isp_module_ctrl_s {
	hbn_isp_module_version_e version;
	isp_module_ctrl_u module;
} hbn_isp_module_ctrl_t;

/* exposure attribute */
typedef enum enum_isp_exposure_version {
	HBN_ISP_EXP_V0 = 0,
	HBN_ISP_EXP_V1,
	HBN_ISP_EXP_V2,
} hbn_isp_exposure_version_e;

typedef enum enum_isp_auto_exposure_mode {
	HBN_ISP_AUTO_EXP_MODE_ADAPTIVE = 0,
	HBN_ISP_AUTO_EXP_MODE_FIX,
} hbn_isp_auto_exposure_mode_e;

typedef struct isp_exposure_auto_attr_s {
	hbn_isp_param_range_t exp_time_range;	// 使用s单位
	hbn_isp_param_range_t again_range;
	hbn_isp_param_range_t dgain_range;
	hbn_isp_param_range_t isp_dgain_range;
	float speed_over;			// 暗到亮速度
	float speed_under; 			// 亮到暗速度
	float tolerance;			// 偏差容忍度
	float target;				// 目标亮度值
	uint32_t anti_flicker_status;		// 抗频闪状态
	float flicker_freq;			// flicker 频率
	hbn_isp_auto_exposure_mode_e mode;
} hbn_isp_exposure_auto_attr_t;

typedef struct isp_exposure_manual_attr_s {
	float exp_time;		// 使用s单位
	float again;
	float dgain;
	float ispgain;
	float ae_exp;
	uint32_t cur_lux;	// 环境照度
	uint32_t frame_id;
	uint64_t timestamps;
} hbn_isp_exposure_manual_attr_t;

typedef struct hbn_isp_exposure_attr_s {
	hbn_isp_exposure_version_e version; //版本号
	hbn_isp_mode_e mode;
	hbn_isp_exposure_auto_attr_t auto_attr;
	hbn_isp_exposure_manual_attr_t manual_attr;
} hbn_isp_exposure_attr_t;

/* HDR exposure attribute */
typedef enum enum_isp_hdr_exposure_version {
	HBN_ISP_HDR_EXP_V0 = 0,
	HBN_ISP_HDR_EXP_V1,
	HBN_ISP_HDR_EXP_V2,
} hbn_isp_hdr_exposure_version_e;

typedef struct isp_hdr_exposure_auto_attr_s {
	float exp_ratio[4];
	hbn_isp_param_range_t exp_ratio_range;
} hbn_isp_hdr_exposure_auto_attr_t;

typedef struct isp_hdr_exposure_manual_attr_s {
	float exp_time; // 使用s单位
	float again;
	float dgain;
	float ispgain;
	uint32_t ae_exp;
} hbn_isp_hdr_exposure_manual_attr_t;

typedef struct isp_hdr_exposure_attr_s {
	hbn_isp_hdr_exposure_version_e version;
	hbn_isp_mode_e mode;
	hbn_isp_hdr_exposure_auto_attr_t auto_attr;
	hbn_isp_hdr_exposure_manual_attr_t manual_attr;
} hbn_isp_hdr_exposure_attr_t;

/* AWB attribute */
typedef enum enum_isp_awb_version {
	HBN_ISP_WB_V0 = 0,
	HBN_ISP_WB_V1,
} hbn_isp_awb_version_e;

/* AWB gain parameter */
typedef struct hbn_isp_awb_gain_s {
	float rgain;
	float grgain;
	float gbgain;
	float bgain;
} hbn_isp_awb_gain_t;

typedef struct hbn_isp_awb_auto_attr_s {
	uint32_t speed;
	uint32_t tolerance;
	uint32_t rg_strength;  // r通道强度（设置白平衡偏好r通道）
	uint32_t bg_strength;  // b通道强度（设置白平衡偏好b通道）
	hbn_isp_awb_gain_t gain;
	uint32_t temper;
} hbn_isp_awb_auto_attr_t;

typedef struct hbn_isp_awb_manual_attr_s {
	hbn_isp_awb_gain_t gain;
	uint32_t temper;	// 预留
} hbn_isp_awb_manual_attr_t;

typedef struct hbn_isp_awb_attr_s {
	hbn_isp_awb_version_e version;
	hbn_isp_mode_e mode;
	hbn_isp_awb_auto_attr_t auto_attr;
	hbn_isp_awb_manual_attr_t manual_attr;
} hbn_isp_awb_attr_t;

/* color process attribute */
typedef enum enum_isp_color_process_version {
	HBN_ISP_COLOR_PROCESS_V0 = 0,
	HBN_ISP_COLOR_PROCESS_V1,
} hbn_isp_color_process_version_e;

typedef struct hbn_isp_color_process_auto_attr_s {
	uint32_t total_size;
	float gain[20];
	float bright[20];
	float contrast[20];
	float saturation[20];
	float hue[20];
} hbn_isp_color_process_auto_attr_t;

typedef struct hbn_isp_color_process_manual_attr_s {
	uint32_t total_size;
	float bright;
	float contrast;
	float saturation;
	float hue;
} hbn_isp_color_process_manual_attr_t;

typedef struct hbn_isp_color_process_attr_s {
	hbn_isp_color_process_version_e version;
	hbn_isp_mode_e mode;
	hbn_isp_color_process_auto_attr_t auto_attr;
	hbn_isp_color_process_manual_attr_t manual_attr;
} hbn_isp_color_process_attr_t;

/* AE 1024-zone weight */
typedef enum enum_isp_ae_zone_weight_version {
	HBN_ISP_AE_ZONE_WEIGHT_A = 0,
	HBN_ISP_AE_ZONE_WEIGHT_B,
} hbn_isp_ae_zone_weight_version_e;

#define HBN_ISP_GRID_NUM 32
#define HBN_ISP_GRID_ITEMS (HBN_ISP_GRID_NUM * HBN_ISP_GRID_NUM)  /**< number of grid items */

typedef struct isp_ae_zone_weight_func_a_attr_s {
	uint32_t total_size;
	hbn_isp_zone_weight_t weight[HBN_ISP_GRID_ITEMS];
} isp_ae_zone_weight_func_a_attr_t;

typedef struct hbn_isp_ae_zone_weight_attr_s {
	hbn_isp_ae_zone_weight_version_e version;
	union {
		isp_ae_zone_weight_func_a_attr_t func_a_attr;
	} attr;
} hbn_isp_ae_zone_weight_attr_t;

/* AF 225-zone weight */
typedef enum enum_isp_af_zone_weight_version_e {
	HBN_ISP_AF_ZONE_WEIGHT_A = 0,
	HBN_ISP_AF_ZONE_WEIGHT_B,
} hbn_isp_af_zone_weight_version_e;

typedef struct isp_af_zone_weight_func_a_attr_s {
	uint32_t total_size;
	hbn_isp_zone_weight_t weight[15 * 15];
} isp_af_zone_weight_func_a_attr_t;

typedef struct hbn_isp_af_zone_weight_attr_s {
	hbn_isp_af_zone_weight_version_e version;
	union {
		isp_af_zone_weight_func_a_attr_t func_a_attr;
	} attr;
} hbn_isp_af_zone_weight_attr_t;

/* statistics */
#define HBN_ISP_PIXEL_CHANNEL 4   /**< number of pixel channel */
#define HBN_ISP_AFM_BLOCK_NUM 225

typedef enum enum_isp_exp_datatype_e {
	ISP_EXP_8BIT_DATA = 0,	/**< EXP 8 bit data */
	ISP_EXP_16BIT_DATA = 1,	/**< EXP 16 bit data */
	ISP_EXP_24BIT_DATA = 2,	/**< EXP 24 bit data */
	ISP_BIT_DATA_MAX
} hbn_isp_exp_datatype_t;

typedef struct hbn_isp_ae_statistics_s {
	uint32_t expStat[HBN_ISP_GRID_ITEMS * HBN_ISP_PIXEL_CHANNEL];
	uint32_t datatype;
	uint32_t frame_id;
	uint64_t timestamps;
} hbn_isp_ae_statistics_t;

typedef struct hbn_isp_awb_statistics_s {
	uint32_t awbStat[HBN_ISP_GRID_ITEMS * HBN_ISP_PIXEL_CHANNEL];
	uint32_t datatype;
	uint32_t frame_id;
	uint64_t timestamps;
} hbn_isp_awb_statistics_t;

typedef struct hbn_isp_af_statistics_s {
	uint32_t sharpnessLowPass[HBN_ISP_AFM_BLOCK_NUM];
	uint32_t sharpnessHighPass[HBN_ISP_AFM_BLOCK_NUM];
	uint32_t histLowData[HBN_ISP_AFM_BLOCK_NUM];
	uint32_t histHighData[HBN_ISP_AFM_BLOCK_NUM];
	uint32_t frame_id;	// 当前对应frame id(备用)
} hbn_isp_af_statistics_t;

/* exposure table */
#define CAMDEV_AE_EXP_TABLE_NUM	8

typedef struct hbn_isp_table_s {
	float exposure_time;	/**< AE exposure time */
	float again;		/**< AE simulated again */
	float dgain;		/**< AE digital gain */
	float isp_gain;		/**< AE isp gain */
} hbn_isp_table_t;

typedef struct hbn_isp_exposure_table_s {
	hbn_isp_table_t	exp_table[CAMDEV_AE_EXP_TABLE_NUM];	/**< Exposure table */
	uint8_t valid_num;	/**< The valid number of exposure table */
} hbn_isp_exposure_table_t;

extern int32_t hbn_isp_set_module_control(hbn_vnode_handle_t vnode_fd, hbn_isp_module_ctrl_t *p_ctrl);
extern int32_t hbn_isp_get_module_control(hbn_vnode_handle_t vnode_fd, hbn_isp_module_ctrl_t *p_ctrl);
extern int32_t hbn_isp_set_exposure_attr(hbn_vnode_handle_t vnode_fd, hbn_isp_exposure_attr_t *p_attr);
extern int32_t hbn_isp_get_exposure_attr(hbn_vnode_handle_t vnode_fd, hbn_isp_exposure_attr_t *p_attr);
extern int32_t hbn_isp_set_hdr_exposure_attr(hbn_vnode_handle_t vnode_fd, hbn_isp_hdr_exposure_attr_t *p_attr);
extern int32_t hbn_isp_get_hdr_exposure_attr(hbn_vnode_handle_t vnode_fd, hbn_isp_hdr_exposure_attr_t *p_attr);
extern int32_t hbn_isp_set_awb_attr(hbn_vnode_handle_t vnode_fd, hbn_isp_awb_attr_t *p_attr);
extern int32_t hbn_isp_get_awb_attr(hbn_vnode_handle_t vnode_fd, hbn_isp_awb_attr_t *p_attr);
extern int32_t hbn_isp_get_awb_gain_by_temper(hbn_vnode_handle_t vnode_fd, uint32_t temper, hbn_isp_awb_gain_t *p_awb_gain);
extern int32_t hbn_isp_set_color_process_attr(hbn_vnode_handle_t vnode_fd, hbn_isp_color_process_attr_t *p_attr);
extern int32_t hbn_isp_get_color_process_attr(hbn_vnode_handle_t vnode_fd, hbn_isp_color_process_attr_t *p_attr);
extern int32_t hbn_isp_set_ae_zone_weight_attr(hbn_vnode_handle_t vnode_fd, hbn_isp_ae_zone_weight_attr_t *p_attr);
extern int32_t hbn_isp_get_ae_zone_weight_attr(hbn_vnode_handle_t vnode_fd, hbn_isp_ae_zone_weight_attr_t *p_attr);
extern int32_t hbn_isp_set_af_zone_weight_attr(hbn_vnode_handle_t vnode_fd, hbn_isp_af_zone_weight_attr_t *p_attr);
extern int32_t hbn_isp_get_af_zone_weight_attr(hbn_vnode_handle_t vnode_fd, hbn_isp_af_zone_weight_attr_t *p_attr);
extern int32_t hbn_isp_get_ae_statistics(hbn_vnode_handle_t vnode_fd, hbn_isp_ae_statistics_t *p_data);
extern int32_t hbn_isp_get_awb_statistics(hbn_vnode_handle_t vnode_fd, hbn_isp_awb_statistics_t *p_data);
extern int32_t hbn_isp_get_af_statistics(hbn_vnode_handle_t vnode_fd, hbn_isp_af_statistics_t *p_data);
extern int32_t hbn_isp_set_exposure_table(hbn_vnode_handle_t vnode_fd, hbn_isp_exposure_table_t *p_attr);
extern int32_t hbn_isp_get_exposure_table(hbn_vnode_handle_t vnode_fd, hbn_isp_exposure_table_t *p_attr);
extern int32_t hbn_isp_get_lines_persecond(hbn_vnode_handle_t vnode_fd, uint32_t *lines);

#ifdef __cplusplus
}
#endif

#endif	//__HBN_ISP_API_H__
