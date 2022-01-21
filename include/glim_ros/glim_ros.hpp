#pragma once

#include <any>
#include <deque>
#include <memory>
#include <rclcpp/rclcpp.hpp>

#include <cv_bridge/cv_bridge.h>
#include <sensor_msgs/msg/imu.hpp>
#include <sensor_msgs/msg/image.hpp>
#include <sensor_msgs/msg/point_cloud2.hpp>

#ifdef BUILD_WITH_LIVOX
#include <livox_interfaces/msg/custom_msg.hpp>
#endif

namespace glim {
class CloudPreprocessor;
class AsyncOdometryEstimation;
class AsyncSubMapping;
class AsyncGlobalMapping;
class StandardViewer;
}  // namespace glim

namespace glim_ros {

struct RawPoints;

class GlimROS : public rclcpp::Node {
public:
  GlimROS(const rclcpp::NodeOptions& options);
  ~GlimROS();

  void timer_callback();

  void imu_callback(const sensor_msgs::msg::Imu::ConstSharedPtr msg);
  void image_callback(const sensor_msgs::msg::Image::ConstSharedPtr msg);
  void points_callback(const sensor_msgs::msg::PointCloud2::ConstSharedPtr msg);

#ifdef BUILD_WITH_LIVOX
  void livox_points_callback(const livox_interfaces::msg::CustomMsg::ConstSharedPtr msg);
#endif

  bool ok() const;
  void wait();

  void save(const std::string& path);

private:
private:
  std::any timer;
  std::any imu_sub;
  std::any image_sub;
  std::any points_sub;

  double latest_imu_stamp;
  std::deque<std::shared_ptr<const glim_ros::RawPoints>> frame_queue;

  std::unique_ptr<glim::CloudPreprocessor> preprocessor;
  std::unique_ptr<glim::AsyncOdometryEstimation> odometry_estimation;
  std::unique_ptr<glim::AsyncSubMapping> sub_mapping;
  std::unique_ptr<glim::AsyncGlobalMapping> global_mapping;

  std::unique_ptr<glim::StandardViewer> standard_viewer;
};

}  // namespace glim_ros
