#ifndef __MO_TYPES_H
#define __MO_TYPES_H

#include <stdint.h>
#include <stdio.h>

#include "MoCommon.h"

#define MO_RGB_IMAGE_TYPE_YUV422_YUYV 0x01
#define MO_RGB_IMAGE_TYPE_YUV422_YVYU 0x02

#define MO_RGB_IMAGE_TYPE_RGB888 0x03

#define MO_RGB_IMAGE_TYPE_YUV422_UYVY 0x04

#define MO_RGB_IMAGE_TYPE_YUV420_NV12 0x10
#define MO_RGB_IMAGE_TYPE_YUV420_I420 0x20

typedef struct __MoFrame
{
    unsigned long long TimeStamp; // time (ms) when this frame generate
    unsigned long long FrameId;   // id of this frame

    unsigned int StreamType;         // image type of this frame
    unsigned int SensorId;           // the id of the sensor that the outpout this frame
    unsigned short ResolutionWidth;  // resolution - width of this frame
    unsigned short ResolutionHeight; // resolution - height of this frame
    unsigned int BitsPerPixel;       // bytes per pixel

    unsigned int BytesUsed; // frame size in bytes

    int FillLightStatus;     // fill light status when this frame generate                  1 on, 0 off, 2(other) unknown
    int FillLightBrightness; // fill light bright (1%~100%) when this frame generate

    int RGBImageType;

    uint8_t Reserved[64]; // reserved

    void *Data; // image data
} MoFrame;
// stream frame

typedef struct _mo_mono_camera_params
{
    float fx;
    float fy;
    float cx;
    float cy;
    //  相机畸变参数
    float radial_distortion_k[6];
    float tangential_distortion_p[2];
} MoMonoCameraParams;

#endif
