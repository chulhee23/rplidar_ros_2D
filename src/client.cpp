
#include <ros/ros.h>
#include <sensor_msgs/LaserScan.h>
#include <laser_geometry/laser_geometry.h> 
#include <pcl_conversions/pcl_conversions.h> 
#include <pcl/point_types.h>

//----------------- 전역변수 수정 ----------------- 
ros::Publisher pub;
void scanCallback(const sensor_msgs::LaserScan::ConstPtr& scan) {
    //----- Type 변환 : sensor_msgs::LaserScan -> sensor_msgs::PointCloud ----- 
    sensor_msgs::PointCloud msgCloud;
    laser_geometry::LaserProjection projector_;
    projector_.projectLaser(*scan, msgCloud);
    //----- Type 변환 : sensor_msgs::PointCloud to pcl::PointCloud ------------ 
    pcl::PointCloud<pcl::PointXYZ> pclCloud;
    pclCloud.width = msgCloud.points.size();
    pclCloud.height = 1;
    pclCloud.points.resize(pclCloud.width * pclCloud.height);
    for(int i = 0; i < msgCloud.points.size(); i++){ 
        pclCloud.points[i].x = msgCloud.points[i].x; 
        pclCloud.points[i].y = msgCloud.points[i].y; 
        pclCloud.points[i].z = 0;
    }
    //----- Type 변환 : pcl::PointCloud to sensor_msgs::PointCloud2 ---------- 
    sensor_msgs::PointCloud2 output;
    pcl::toROSMsg(pclCloud, output);
    std_msgs::Header h = scan->header;
    output.header.frame_id = "/map";
    output.header.stamp = h.stamp;
    //------------------ Data publish ------------------
    pub.publish(output);
}

int main(int argc, char **argv)
{
    ros::init(argc, argv, "rplidar_node_client"); ros::NodeHandle nh;
    ros::Subscriber sub = nh.subscribe<sensor_msgs::LaserScan>("/scan", 1000, scanCallback); 
    pub = nh.advertise<sensor_msgs::PointCloud2>("/Laser2PointCloud", 1);
    ros::spin();
    return 0; 
}
