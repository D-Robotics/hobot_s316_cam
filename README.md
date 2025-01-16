# hobot_s316_cam

## 描述

启动s316相机，并发布双目图像

## 编译

依赖：

1. 安装依赖的库文件，在RDK X5板端执行

```shell
cp -rv hobot_s316_cam/lib/* /usr/
```

2. 交叉编译，在PC端执行

```shell
bash ./robot_dev_config/build.sh -p X5 -s hobot_s316_cam
```

## 运行

(0) 运行s316相机之前，需要将配置文件拷贝到运行目录

```shell
cp -rv hobot_s316_cam/config/cfg ./
```

(1) 发布双目图像

```bash
ros2 launch hobot_s316_cam pub_stereo_imgs.launch.py
```

在浏览器输入[http://ip:8000](http://ip:8000)即可查看s316输出的双目图像

![](./doc/s316-stereo-result.png)

(2) 发布双目图像+地瓜双目算法

```bash
ros2 launch hobot_s316_cam test_stereo_custom_rectify.launch.py \
stereonet_model_file_path:=./x5baseplus_alldata_woIsaac_yuv444.bin postprocess:=v2 \
stereo_calib_path:=./stereo_8.yaml
```

在浏览器输入[http://ip:8000](http://ip:8000)即可查看双目算法的结果

![](./doc/s316-stereonet-result.png)

## 功能包参数

| 名称 | 默认值 | 说明 |
| ---- | ------ | ---- |
|      |        |      |
