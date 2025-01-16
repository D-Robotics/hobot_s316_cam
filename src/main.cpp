// Copyright (c) 2024，D-Robotics.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#include "rclcpp/rclcpp.hpp"
#include "opencv2/opencv.hpp"
#include "MoLocalSdkInterface.h"
#include "sensor_msgs/msg/image.hpp"

class S316CamNode : public rclcpp::Node
{
public:
    S316CamNode() : Node("s316_cam_node")
    {
        // ======================================================================================================================================
        RCLCPP_INFO(this->get_logger(), "=> init s316_cam_node");
        // param
        need_gdc_ = this->declare_parameter("need_gdc", false);
        RCLCPP_INFO_STREAM(this->get_logger(), "\033[31m" << std::endl << "=> need_gdc: " << need_gdc_ << std::endl << "\033[0m");

        // ======================================================================================================================================
        // sub & pub
        stereo_msg_pub_ = this->create_publisher<sensor_msgs::msg::Image>("/image_combine_raw", 10);

        // ======================================================================================================================================
        int ret = initCam();
        if (ret != 0)
        {
            rclcpp::shutdown();
            return;
        }
        getImageData();
        deinitCam();
    }

private:
    // ======================================================================================================================================
    /**
     * @brief init cam
     */
    int initCam()
    {
        int Handle = 0;
        if (moOpenCamera(Handle) != 0)
        {
            RCLCPP_ERROR(this->get_logger(), "\033[31m=> open s316 camera error!\033[0m");
            return -1;
        }
        return 0;
    }

    /**
     * @brief get image data and pub
     */
    int getImageData()
    {
        int handle = 0;
        while (rclcpp::ok())
        {
            auto stereo_msg = std::make_shared<sensor_msgs::msg::Image>();
            stereo_msg->header.frame_id = "pcl_link";
            stereo_msg->is_bigendian = false;
            stereo_msg->encoding = "nv12";

            if (!need_gdc_)
            {
                MoFrame orgRgbFrame;
                if (MO_REV_OK == moGetOneOrgRGBFrame(handle, &orgRgbFrame))
                {
                    MoFrame leftRgbFrame, rightRgbFrame;
                    moDecodeFrame(handle, &orgRgbFrame, &leftRgbFrame, &rightRgbFrame);

                    // RCLCPP_INFO(this->get_logger(), "=> orgRgbFrame leftRgbFrame rightRgbFrame stamp: %lld, %lld, %lld", orgRgbFrame.TimeStamp, leftRgbFrame.TimeStamp, rightRgbFrame.TimeStamp);
                    // RCLCPP_INFO(this->get_logger(), "=> orgRgbFrame leftRgbFrame rightRgbFrame size: %d, %d, %d", orgRgbFrame.BytesUsed, leftRgbFrame.BytesUsed, rightRgbFrame.BytesUsed);

                    // output image pointer
                    void *output_image = nullptr;
                    output_image = std::malloc(orgRgbFrame.BytesUsed);

                    // concate image
                    concatenateNV12Images(leftRgbFrame.Data, rightRgbFrame.Data, 1280, 1088, output_image);

                    stereo_msg->header.stamp = rclcpp::Time(orgRgbFrame.TimeStamp);
                    stereo_msg->height = 2176;
                    stereo_msg->width = 1280;
                    stereo_msg->step = stereo_msg->width;
                    stereo_msg->data.resize(orgRgbFrame.BytesUsed);
                    std::memcpy(stereo_msg->data.data(), output_image, orgRgbFrame.BytesUsed);
                    stereo_msg_pub_->publish(*stereo_msg);

                    moReleaseFrame(&orgRgbFrame);
                    std::free(output_image);
                }
            }
            else
            {
                MoFrame rgbFrame;
                if (MO_REV_OK == moGetOneRGBFrame(handle, &rgbFrame))
                {
                    MoFrame leftRgbFrame, rightRgbFrame;
                    moDecodeFrame(handle, &rgbFrame, &leftRgbFrame, &rightRgbFrame);

                    // RCLCPP_INFO(this->get_logger(), "=> rgbFrame leftRgbFrame rightRgbFrame stamp: %lld, %lld, %lld", rgbFrame.TimeStamp, leftRgbFrame.TimeStamp, rightRgbFrame.TimeStamp);
                    // RCLCPP_INFO(this->get_logger(), "=> rgbFrame leftRgbFrame rightRgbFrame size: %d, %d, %d", rgbFrame.BytesUsed, leftRgbFrame.BytesUsed, rightRgbFrame.BytesUsed);

                    // output image pointer
                    void *output_image = nullptr;
                    output_image = std::malloc(rgbFrame.BytesUsed);

                    // concate image
                    concatenateNV12Images(leftRgbFrame.Data, rightRgbFrame.Data, 640, 544, output_image);

                    stereo_msg->header.stamp = rclcpp::Time(rgbFrame.TimeStamp);
                    stereo_msg->height = 1088;
                    stereo_msg->width = 640;
                    stereo_msg->step = stereo_msg->width;
                    stereo_msg->data.resize(rgbFrame.BytesUsed);
                    std::memcpy(stereo_msg->data.data(), output_image, rgbFrame.BytesUsed);
                    stereo_msg_pub_->publish(*stereo_msg);

                    std::free(output_image);
                    moReleaseFrame(&rgbFrame);
                }
            }
        }
        return 0;
    }

    /**
     * @brief deinit cam
     */
    int deinitCam()
    {
        int handle = 0;
        int iret = moCloseCamera(handle);
        if (MO_REV_OK != iret)
        {
            RCLCPP_INFO(this->get_logger(), "=> moLocalCloseCamera failed \n");
            return -1;
        }

        return 0;
    }

    // NV12 图像拼接函数
    void concatenateNV12Images(void *img1, void *img2, int width, int height, void *&output)
    {
        // NV12 格式的 Y 平面大小
        size_t y_plane_size = width * height;
        // NV12 格式的 UV 平面大小（每个像素对共享一个UV分量）
        size_t uv_plane_size = (width * height) / 2;

        // 获取输出图像的指针
        uint8_t *output_ptr = static_cast<uint8_t *>(output);

        // 拼接 Y 平面
        std::memcpy(output_ptr, img1, y_plane_size);                // 第一张图的 Y 平面
        std::memcpy(output_ptr + y_plane_size, img2, y_plane_size); // 第二张图的 Y 平面

        // 拼接 UV 平面
        std::memcpy(output_ptr + 2 * y_plane_size, static_cast<uint8_t *>(img1) + y_plane_size, uv_plane_size);                 // 第一张图的 UV 平面
        std::memcpy(output_ptr + 2 * y_plane_size + uv_plane_size, static_cast<uint8_t *>(img2) + y_plane_size, uv_plane_size); // 第二张图的 UV 平面
    }

    // ======================================================================================================================================
    // stereo image publisher
    rclcpp::Publisher<sensor_msgs::msg::Image>::SharedPtr stereo_msg_pub_;

    // image gdc rectify flag
    bool need_gdc_ = false;
};

int main(int argc, char *argv[])
{
    rclcpp::init(argc, argv);
    auto node = std::make_shared<S316CamNode>();
    rclcpp::spin(node);
    rclcpp::shutdown();
    return 0;
}