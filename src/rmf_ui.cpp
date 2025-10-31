#include "rmf_ui.h"

#include <pluginlib/class_list_macros.hpp>
#include <QTimer>

using namespace std::chrono_literals;

namespace rviz_nmpc_plugin
{
    nmpc_panel::nmpc_panel(QWidget* parent)
    : rviz_common::Panel(parent)
    {
        auto* v = new QVBoxLayout;

        btn_flag     = new QPushButton("toggle SDF (-)");
        btn_yaw_mode = new QPushButton("toggle free yaw (-)");
        btn_hover    = new QPushButton("hover");
        btn_takeoff  = new QPushButton("takeoff");
        btn_goto     = new QPushButton("goto");
        btn_stop     = new QPushButton("STOP");

        v->addWidget(btn_flag);
        v->addWidget(btn_yaw_mode);
        v->addWidget(btn_hover);
        v->addWidget(btn_takeoff);
        v->addWidget(btn_goto);
        v->addWidget(btn_stop);
        setLayout(v);

        // Qt signals
        connect(btn_flag,     SIGNAL(clicked()), this, SLOT(onclick_flag()));
        connect(btn_yaw_mode, SIGNAL(clicked()), this, SLOT(onclick_yaw_mode()));
        connect(btn_hover,    SIGNAL(clicked()), this, SLOT(onclick_hover()));
        connect(btn_takeoff,  SIGNAL(clicked()), this, SLOT(onclick_takeoff()));
        connect(btn_goto,     SIGNAL(clicked()), this, SLOT(onclick_goto()));
        connect(btn_stop,     SIGNAL(clicked()), this, SLOT(onclick_stop()));
    }

    void nmpc_panel::onInitialize()
    {
        node_ = std::make_shared<rclcpp::Node>(
            "rviz_nmpc_panel",
            rclcpp::NodeOptions().automatically_declare_parameters_from_overrides(true)
        );

        //  service clients
        srvc_get_flag_     = node_->create_client<std_srvs::srv::Trigger>("get_flag");
        srvc_set_flag_     = node_->create_client<std_srvs::srv::SetBool>("set_flag");
        srvc_get_yaw_mode_ = node_->create_client<std_srvs::srv::Trigger>("get_yaw_mode");
        srvc_set_yaw_mode_ = node_->create_client<std_srvs::srv::SetBool>("set_yaw_mode");
        srvc_hover_        = node_->create_client<std_srvs::srv::Trigger>("hover");
        srvc_takeoff_      = node_->create_client<std_srvs::srv::Trigger>("takeoff");
        srvc_goto_         = node_->create_client<std_srvs::srv::Trigger>("goto");
        srvc_stop_         = node_->create_client<std_srvs::srv::Trigger>("stop");

        // periodic UI refresh
        qt_timer_ = new QTimer(this);
        qt_timer_->setInterval(1000);
        connect(qt_timer_, &QTimer::timeout, this, &nmpc_panel::onTimer);
        qt_timer_->start();

        // initial refresh
        onTimer();
    }

    // helper to wait for a service call without blocking UI hard
    template<typename FutureT>
    bool wait_for(rclcpp::Node::SharedPtr node, FutureT& fut, std::chrono::milliseconds timeout = 200ms)
    {
        return rclcpp::spin_until_future_complete(node, fut, timeout) == rclcpp::FutureReturnCode::SUCCESS;
    }

    // periodic refresher
    void nmpc_panel::onTimer()
    {
        (void)get_flag_status();
        (void)get_yaw_mode_status();
    }

    // status checker
    bool nmpc_panel::get_flag_status()
    {
        if (!srvc_get_flag_ || !srvc_get_flag_->service_is_ready()) {
            btn_flag->setText("toggle SDF (-)");
            return false;
        }
        auto req = std_srvs::srv::Trigger::Request::SharedPtr(new std_srvs::srv::Trigger::Request());
        auto fut = srvc_get_flag_->async_send_request(req);
        if (!wait_for(node_, fut)) {
            btn_flag->setText("toggle SDF (-)");
            return false;
        }
        const auto resp = fut.get();
        if (!resp) {
            btn_flag->setText("toggle SDF (-)");
            return false;
        }
        btn_flag->setText(resp->success ? "toggle SDF (ON)" : "toggle SDF (OFF)");
        return resp->success;
    }

    bool nmpc_panel::get_yaw_mode_status()
    {
        if (!srvc_get_yaw_mode_ || !srvc_get_yaw_mode_->service_is_ready()) {
            btn_yaw_mode->setText("toggle free yaw (-)");
            return false;
        }
        auto req = std_srvs::srv::Trigger::Request::SharedPtr(new std_srvs::srv::Trigger::Request());
        auto fut = srvc_get_yaw_mode_->async_send_request(req);
        if (!wait_for(node_, fut)) {
            btn_yaw_mode->setText("toggle free yaw (-)");
            return false;
        }
        const auto resp = fut.get();
        if (!resp) {
            btn_yaw_mode->setText("toggle free yaw (-)");
            return false;
        }
        btn_yaw_mode->setText(resp->success ? "toggle free yaw (ON)" : "toggle free yaw (OFF)");
        return resp->success;
    }

    // onclick events
    void nmpc_panel::onclick_flag()
    {
        if (!srvc_set_flag_) return;
        auto req = std_srvs::srv::SetBool::Request::SharedPtr(new std_srvs::srv::SetBool::Request());

        // request inverting current state
        req->data = !get_flag_status();
        auto fut = srvc_set_flag_->async_send_request(req);
        if (!wait_for(node_, fut)) {
            RCLCPP_ERROR(node_->get_logger(), "service call failed: %s", srvc_set_flag_->get_service_name());
        }
        onTimer();
    }

    void nmpc_panel::onclick_yaw_mode()
    {
        if (!srvc_set_yaw_mode_) return;
        auto req = std_srvs::srv::SetBool::Request::SharedPtr(new std_srvs::srv::SetBool::Request());

        // request inverting current state
        req->data = !get_yaw_mode_status();
        auto fut = srvc_set_yaw_mode_->async_send_request(req);
        if (!wait_for(node_, fut)) {
            RCLCPP_ERROR(node_->get_logger(), "service call failed: %s", srvc_set_yaw_mode_->get_service_name());
        }
        onTimer();
    }


    void nmpc_panel::onclick_hover()
    {
        if (!srvc_hover_) return;
        auto req = std_srvs::srv::Trigger::Request::SharedPtr(new std_srvs::srv::Trigger::Request());
        auto fut = srvc_hover_->async_send_request(req);
        if (!wait_for(node_, fut)) {
            RCLCPP_ERROR(node_->get_logger(), "service call failed: %s", srvc_hover_->get_service_name());
        }
    }

    void nmpc_panel::onclick_takeoff()
    {
        if (!srvc_takeoff_) return;
        auto req = std_srvs::srv::Trigger::Request::SharedPtr(new std_srvs::srv::Trigger::Request());
        auto fut = srvc_takeoff_->async_send_request(req);
        if (!wait_for(node_, fut)) {
            RCLCPP_ERROR(node_->get_logger(), "service call failed: %s", srvc_takeoff_->get_service_name());
        }
    }

    void nmpc_panel::onclick_goto()
    {
        if (!srvc_goto_) return;
        auto req = std_srvs::srv::Trigger::Request::SharedPtr(new std_srvs::srv::Trigger::Request());
        auto fut = srvc_goto_->async_send_request(req);
        if (!wait_for(node_, fut)) {
            RCLCPP_ERROR(node_->get_logger(), "service call failed: %s", srvc_goto_->get_service_name());
        }
    }

    void nmpc_panel::onclick_stop()
    {
        if (!srvc_stop_) return;
        auto req = std_srvs::srv::Trigger::Request::SharedPtr(new std_srvs::srv::Trigger::Request());
        auto fut = srvc_stop_->async_send_request(req);
        if (!wait_for(node_, fut)) {
            RCLCPP_ERROR(node_->get_logger(), "service call failed: %s", srvc_stop_->get_service_name());
        }

        // disable constraint flag, if enabled
        if (get_flag_status())
            onclick_flag();
    }

    // RViz config persistence
    void nmpc_panel::save(rviz_common::Config config) const
    {
        rviz_common::Panel::save(config);
    }

    void nmpc_panel::load(const rviz_common::Config& config)
    {
        rviz_common::Panel::load(config);
    }

} // namespace rviz_nmpc_plugin

PLUGINLIB_EXPORT_CLASS(rviz_nmpc_plugin::nmpc_panel, rviz_common::Panel)
