#pragma once

// #include <memory>
#include <chrono>

#include <rclcpp/rclcpp.hpp>
#include <std_srvs/srv/trigger.hpp>
#include <std_srvs/srv/set_bool.hpp>

#ifndef Q_MOC_RUN
    #include <QPainter>
    #include <QLineEdit>
    #include <QVBoxLayout>
    #include <QHBoxLayout>
    #include <QPushButton>
    #include <QLabel>
#endif

#include <rviz_common/panel.hpp>

namespace rviz_nmpc_plugin
{
    class nmpc_panel : public rviz_common::Panel
    {
        Q_OBJECT
        public:
            nmpc_panel(QWidget *parent = 0);
            void onInitialize() override;
            void load(const rviz_common::Config& config) override;
            void save(rviz_common::Config config) const override;
            // virtual void load(const rviz::Config &config);
            // virtual void save(rviz::Config config) const;

        public Q_SLOTS:
            void onclick_flag();
            void onclick_yaw_mode();
            void onclick_hover();
            void onclick_takeoff();
            void onclick_goto();
            void onclick_stop();

        protected Q_SLOTS:

        protected:
            // periodic status polling
            void onTimer();
            QTimer* qt_timer_{nullptr};

            // helpers
            bool get_flag_status();
            bool get_yaw_mode_status();

            // Qt widgets
            QPushButton* btn_flag{nullptr};
            QPushButton* btn_yaw_mode{nullptr};
            QPushButton* btn_hover{nullptr};
            QPushButton* btn_takeoff{nullptr};
            QPushButton* btn_goto{nullptr};
            QPushButton* btn_stop{nullptr};

            // ROS 2 bits
            rclcpp::Node::SharedPtr node_;
            rclcpp::Client<std_srvs::srv::Trigger>::SharedPtr srvc_get_flag_;
            rclcpp::Client<std_srvs::srv::SetBool>::SharedPtr  srvc_set_flag_;
            rclcpp::Client<std_srvs::srv::Trigger>::SharedPtr srvc_get_yaw_mode_;
            rclcpp::Client<std_srvs::srv::SetBool>::SharedPtr  srvc_set_yaw_mode_;
            rclcpp::Client<std_srvs::srv::Trigger>::SharedPtr srvc_hover_;
            rclcpp::Client<std_srvs::srv::Trigger>::SharedPtr srvc_takeoff_;
            rclcpp::Client<std_srvs::srv::Trigger>::SharedPtr srvc_goto_;
            rclcpp::Client<std_srvs::srv::Trigger>::SharedPtr srvc_stop_;
    };
} // namespace rviz_nmpc_plugin
