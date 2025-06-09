#include "rmf_ui.h"

namespace rviz_nmpc_plugin
{
    rmf_panel::rmf_panel(QWidget *parent) : rviz::Panel(parent)
    {
        srvc_get_flag = nh.serviceClient<std_srvs::Trigger>("/nmpc/get_flag");
        srvc_set_flag = nh.serviceClient<std_srvs::SetBool>("/nmpc/set_flag");
        srvc_get_yaw_mode = nh.serviceClient<std_srvs::Trigger>("/nmpc/get_yaw_mode");
        srvc_set_yaw_mode = nh.serviceClient<std_srvs::SetBool>("/nmpc/set_yaw_mode");
        srvc_hover = nh.serviceClient<std_srvs::Trigger>("/nmpc/hover");
        srvc_takeoff = nh.serviceClient<std_srvs::Trigger>("/nmpc/takeoff");
        srvc_goto = nh.serviceClient<std_srvs::Trigger>("/nmpc/goto");
        srvc_stop = nh.serviceClient<std_srvs::Trigger>("/nmpc/stop");

        QVBoxLayout *v_box_layout = new QVBoxLayout;

        btn_flag = new QPushButton;
        btn_yaw_mode = new QPushButton;
        btn_hover = new QPushButton;
        btn_takeoff = new QPushButton;
        btn_goto = new QPushButton;
        btn_stop = new QPushButton;


        (void)get_flag_status();
        (void)get_yaw_status();
        btn_hover->setText("hover in place");
        btn_takeoff->setText("takeoff");
        btn_goto->setText("goto wp");
        btn_stop->setText("stop");

        v_box_layout->addWidget(btn_flag);
        v_box_layout->addWidget(btn_yaw_mode);
        v_box_layout->addWidget(btn_hover);
        v_box_layout->addWidget(btn_takeoff);
        v_box_layout->addWidget(btn_goto);
        v_box_layout->addWidget(btn_stop);

        QVBoxLayout *global_vbox_layout = new QVBoxLayout;
        QHBoxLayout *global_hbox_layout = new QHBoxLayout;

        global_vbox_layout->addLayout(global_hbox_layout);
        v_box_layout->addLayout(global_vbox_layout);

        setLayout(v_box_layout);

        connect(btn_flag, SIGNAL(clicked()), this, SLOT(onclick_flag()));
        connect(btn_yaw_mode, SIGNAL(clicked()), this, SLOT(onclick_yaw_mode()));
        connect(btn_hover, SIGNAL(clicked()), this, SLOT(onclick_hover()));
        connect(btn_takeoff, SIGNAL(clicked()), this, SLOT(onclick_takeoff()));
        connect(btn_goto, SIGNAL(clicked()), this, SLOT(onclick_goto()));
        connect(btn_stop, SIGNAL(clicked()), this, SLOT(onclick_stop()));
    }

    // status checker
    bool rmf_panel::get_flag_status()
    {
        std_srvs::Trigger srv;
        bool ret = false;
        if (!srvc_get_flag.call(srv))
            btn_flag->setText("toggle SDF (-)");
        else if (!srv.response.success)
            btn_flag->setText("toggle SDF (OFF)");
        else
        {
            btn_flag->setText("toggle SDF (ON)");
            ret = true;
        }
        return ret;
    }

    bool rmf_panel::get_yaw_status()
    {
        std_srvs::Trigger srv;
        bool ret = false;
        if (!srvc_get_yaw_mode.call(srv))
            btn_yaw_mode->setText("toggle free yaw (-)");
        else if (!srv.response.success)
            btn_yaw_mode->setText("toggle free yaw (OFF)");
        else
        {
            btn_yaw_mode->setText("toggle free yaw (ON)");
            ret = true;
        }
        return ret;
    }

    // onclick events
    void rmf_panel::onclick_flag()
    {
        bool status = get_flag_status();

        std_srvs::SetBool srv;
        srv.request.data = !status;
        if (!srvc_set_flag.call(srv))
        {
            ROS_ERROR("[RMF-UI] Service call failed: %s", srvc_set_flag.getService().c_str());
            btn_flag->setText("toggle SDF (-)");
        }
        else
        {
            if (srv.response.success)
                btn_flag->setText("toggle SDF (ON)");
            else
                btn_flag->setText("toggle SDF (OFF)");
        }
    }

    void rmf_panel::onclick_yaw_mode()
    {
        bool status = get_yaw_status();

        std_srvs::SetBool srv;
        srv.request.data = !status;
        if (!srvc_set_yaw_mode.call(srv))
        {
            ROS_ERROR("[RMF-UI] Service call failed: %s", srvc_set_yaw_mode.getService().c_str());
            btn_yaw_mode->setText("toggle free yaw (-)");
        }
        else
        {
            if (srv.response.success)
                btn_yaw_mode->setText("toggle free yaw (ON)");
            else
                btn_yaw_mode->setText("toggle free yaw (OFF)");
        }
    }

    void rmf_panel::onclick_hover()
    {
        std_srvs::Trigger srv;
        if (!srvc_hover.call(srv))
            ROS_ERROR("[RMF-UI] Service call failed: %s", srvc_hover.getService().c_str());
    }

    void rmf_panel::onclick_takeoff()
    {
        std_srvs::Trigger srv;
        if (!srvc_takeoff.call(srv))
            ROS_ERROR("[RMF-UI] Service call failed: %s", srvc_takeoff.getService().c_str());
    }

    void rmf_panel::onclick_goto()
    {
        std_srvs::Trigger srv;
        if (!srvc_goto.call(srv))
            ROS_ERROR("[RMF-UI] Service call failed: %s", srvc_goto.getService().c_str());
    }

    void rmf_panel::onclick_stop()
    {
        // hover
        std_srvs::Trigger srv;
        if (!srvc_stop.call(srv))
            ROS_ERROR("[RMF-UI] Service call failed: %s", srvc_stop.getService().c_str());
        // disable constraint flag, if enabled
        if (get_flag_status())
        {
            std_srvs::SetBool srv;
            srv.request.data = false;
            if (!srvc_set_flag.call(srv))
                ROS_ERROR("[RMF-UI] Service call failed: %s", srvc_set_flag.getService().c_str());
            else
                btn_flag->setText("toggle SDF (OFF)");
        }
    }


    // save/load
    void rmf_panel::save(rviz::Config config) const
    {
        rviz::Panel::save(config);
    }
    void rmf_panel::load(const rviz::Config &config)
    {
        rviz::Panel::load(config);
    }

} // namespace rmf_ui

#include <pluginlib/class_list_macros.h>
PLUGINLIB_EXPORT_CLASS(rviz_nmpc_plugin::rmf_panel, rviz::Panel)
