#include "rclcpp/rclcpp.hpp"
#include "std_msgs/msg/string.hpp"

class SubscriberNode : public rclcpp::Node {
public:
    SubscriberNode() : Node("subscriber_node") {
        sub_ = create_subscription<std_msgs::msg::String>(
            "ping_topic", 10,
            [this](const std_msgs::msg::String::SharedPtr msg) {
                auto echo = std_msgs::msg::String();
                echo.data = msg->data;
                pub_->publish(echo);
            });

        pub_ = create_publisher<std_msgs::msg::String>("pong_topic", 10);
    }

private:
    rclcpp::Publisher<std_msgs::msg::String>::SharedPtr pub_;
    rclcpp::Subscription<std_msgs::msg::String>::SharedPtr sub_;
};

int main(int argc, char * argv[]) {
    rclcpp::init(argc, argv);
    rclcpp::spin(std::make_shared<SubscriberNode>());
    rclcpp::shutdown();
    return 0;
}

