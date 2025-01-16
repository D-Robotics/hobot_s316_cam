#ifndef __MO_LOCAL_SDK_INTERFACE_H
#define __MO_LOCAL_SDK_INTERFACE_H

#include "MoCommon.h"
#include "MoTypes.h"

#include <stddef.h>
#include <vector>
#ifdef __cplusplus

#ifndef IN
#define IN
#endif // IN

#ifndef OUT
#define OUT
#endif // OUT

extern "C"
{
#define DLL_API_C_DECL extern "C" __attribute__((visibility("default")))
#endif

    // 获取双目相机的基线值和BxF（基线乘以焦距的积）
    DLL_API_C_DECL int moGetBaseAndBxf(IN const int Handle, OUT double *const pBase, OUT double *const pBxF);

    // 获取相机参数
    DLL_API_C_DECL int moGetCameraParams(IN const int nHandle, OUT MoMonoCameraParams *pParams, OUT double *pBxf, OUT double *pBaseline,
                                         OUT int *pResolutionWidth, OUT int *pResolutionHeight, OUT int *pDspMaxValue);

    DLL_API_C_DECL char *moGetErrorString(IN const int ErrorCode);

    // 打开相机。
    DLL_API_C_DECL int moOpenCamera(IN const int nHandle);

    // 关闭相机。
    DLL_API_C_DECL int moCloseCamera(IN const int nHandle);

    // 获取一帧原始图像数据。左图 + 右图 组合数据,1280x1088 nv12 yuv图像数据。调用moDecodeFrame解析成分别的左图和右图
    DLL_API_C_DECL int moGetOneOrgRGBFrame(IN const int nHandle, OUT MoFrame *const pFrame);

    // 获取一帧矫正图像数据。左图 + 右图 组合数据, 640x544 nv12 yuv图像数据。调用moDecodeFrame解析成分别的左图和右图
    DLL_API_C_DECL int moGetOneRGBFrame(IN const int nHandle, OUT MoFrame *const pFrame);

    // 将一帧组合数据解析成左图，右图。
    DLL_API_C_DECL int moDecodeFrame(IN const int nHandle, IN const MoFrame *const pComposeFrm,
                                     OUT MoFrame *const pLeftFrm, OUT MoFrame *const pRightFrm);

    // 释放数据帧的内存。
    DLL_API_C_DECL int moReleaseFrame(IN MoFrame *const pFrame);

#ifdef __cplusplus
}
#endif

#endif
