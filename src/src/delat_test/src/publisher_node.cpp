#include "rclcpp/rclcpp.hpp"
#include "std_msgs/msg/string.hpp"
#include <fstream>

class PublisherNode : public rclcpp::Node {
public:
    PublisherNode() : Node("publisher_node") {
        pub_ = create_publisher<std_msgs::msg::String>("ping_topic", 10);
        sub_ = create_subscription<std_msgs::msg::String>(
            "pong_topic", 10,
            [this](const std_msgs::msg::String::SharedPtr msg) {
                int64_t rtt = this->now().nanoseconds() - std::stoll(msg->data);
                RCLCPP_INFO(this->get_logger(), "DELAY: %.2f ms", rtt / 1e6);

                // Log to CSV
              static std::ofstream log("/home/nina/ros2_ws/delays.csv", std::ios::app);
                log << rtt << "\n";
            });

        timer_ = create_wall_timer(
            std::chrono::milliseconds(100),
            [this]() {
                auto msg = std_msgs::msg::String();
                msg.data = std::to_string(this->now().nanoseconds());
                pub_->publish(msg);
            });
    }

private:
    rclcpp::Publisher<std_msgs::msg::String>::SharedPtr pub_;
    rclcpp::Subscription<std_msgs::msg::String>::SharedPtr sub_;
    rclcpp::TimerBase::SharedPtr timer_;
};

int main(int argc, char * argv[]) {
    rclcpp::init(argc, argv);
    rclcpp::spin(std::make_shared<PublisherNode>());
    rclcpp::shutdown();
    return 0;
}

