/*
 * Software License Agreement (BSD License)
 *
 * Copyright (c) 2017, Poznan University of Technology
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 *     * Redistributions of source code must retain the above copyright
 *       notice, this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above copyright
 *       notice, this list of conditions and the following disclaimer in the
 *       documentation and/or other materials provided with the distribution.
 *     * Neither the name of the Poznan University of Technology nor the names
 *       of its contributors may be used to endorse or promote products
 *       derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

/*
 * Author: Mateusz Przybyla
 */

#pragma once

#include <rclcpp/rclcpp.hpp>
#include <tf2_ros/transform_listener.h>
#include <tf2_ros/buffer.h>
#include <std_srvs/srv/empty.hpp>
#include <sensor_msgs/msg/laser_scan.hpp>
#include <sensor_msgs/msg/point_cloud2.hpp>
// #include <obstacle_detector/Obstacles.h>
#include "visualization_msgs/msg/marker.hpp"


#include <obstacle_msgs/msg/obstacles.hpp>

#include "obstacle_detector/utilities/point.h"
#include "obstacle_detector/utilities/segment.h"
#include "obstacle_detector/utilities/circle.h"
#include "obstacle_detector/utilities/point_set.h"

namespace obstacle_detector
{

class ObstacleExtractor : public rclcpp::Node
{
public:
  ObstacleExtractor(rclcpp::NodeOptions options);
  ~ObstacleExtractor();

private:
  // bool updateParams(std_srvs::Empty::Request& req, std_srvs::Empty::Response& res);
  // void scanCallback(const sensor_msgs::LaserScan::ConstPtr scan_msg);
  // void pclCallback(const sensor_msgs::PointCloud2::ConstPtr pcl_msg);
  bool updateParams(const std::shared_ptr<std_srvs::srv::Empty::Request> request, std::shared_ptr<std_srvs::srv::Empty::Response> response);
  void scanCallback(const sensor_msgs::msg::LaserScan::SharedPtr scan_msg);
  void pclCallback(const sensor_msgs::msg::PointCloud2::SharedPtr pcl_msg);

  // void initialize() { std_srvs::srv::Empty empt; updateParams(empt.request, empt.response); }
  void initialize() { std_srvs::srv::Empty::Request::SharedPtr empt; std_srvs::srv::Empty::Response::SharedPtr empt2; updateParams(empt, empt2); }

  void processPoints();
  void groupPoints();
  void publishObstacles();

  void detectSegments(const PointSet& point_set);
  void mergeSegments();
  bool compareSegments(const Segment& s1, const Segment& s2, Segment& merged_segment);
  bool checkSegmentsProximity(const Segment& s1, const Segment& s2);
  bool checkSegmentsCollinearity(const Segment& segment, const Segment& s1, const Segment& s2);

  void detectCircles();
  void mergeCircles();
  bool compareCircles(const Circle& c1, const Circle& c2, Circle& merged_circle);

  // ros::NodeHandle nh_;
  // ros::NodeHandle nh_local_;

  // ros::Subscriber scan_sub_;
  // ros::Subscriber pcl_sub_;
  // ros::Publisher obstacles_pub_;
  // ros::ServiceServer params_srv_;

  rclcpp::Subscription<sensor_msgs::msg::LaserScan>::SharedPtr scan_sub_;
  rclcpp::Subscription<sensor_msgs::msg::PointCloud2>::SharedPtr pcl_sub_;
  rclcpp::Publisher<obstacle_msgs::msg::Obstacles>::SharedPtr obstacles_pub_;
  rclcpp::Publisher<visualization_msgs::msg::Marker>::SharedPtr marker_pub_;
  rclcpp::Publisher<visualization_msgs::msg::Marker>::SharedPtr marker_line_pub_;
  rclcpp::Service<std_srvs::srv::Empty>::SharedPtr params_srv_;

  // ros::Time stamp_;
  rclcpp::Time stamp_;
  std::string base_frame_id_;
  // tf::TransformListener tf_listener_;
  // tf2_ros::Buffer tf_buffer_;
  std::unique_ptr<tf2_ros::Buffer> tf_buffer_;
  std::shared_ptr<tf2_ros::TransformListener> tf_listener_;

  std::list<Point> input_points_;
  std::list<Segment> segments_;
  std::list<Circle> circles_;

  Point point;

  // Parameters
  bool p_active_;
  bool p_use_scan_;
  bool p_use_pcl_;

  bool p_use_split_and_merge_;
  bool p_circles_from_visibles_;
  bool p_discard_converted_segments_;
  bool p_transform_coordinates_;

  int p_min_group_points_;

  double p_distance_proportion_;
  double p_max_group_distance_;
  double p_max_split_distance_;
  double p_max_merge_separation_;
  double p_max_merge_spread_;
  double p_max_circle_radius_;
  double p_radius_enlargement_;

  double p_min_x_limit_;
  double p_max_x_limit_;
  double p_min_y_limit_;
  double p_max_y_limit_;

  std::string p_frame_id_;
};

} // namespace obstacle_detector
