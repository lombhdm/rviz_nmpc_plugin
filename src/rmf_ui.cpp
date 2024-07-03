#include "rmf_ui.h"

namespace rviz_nmpc_plugin
{
    rmf_panel::rmf_panel(QWidget *parent) : rviz::Panel(parent)
    {
        srvc_start = nh.serviceClient<std_srvs::SetBool>(
            "/nmpc/start");
        srvc_sdf = nh.serviceClient<std_srvs::Trigger>(
            "/nmpc/df_flag");
        srvc_goto = nh.serviceClient<std_srvs::Trigger>(
            "/nmpc/goto");

        QVBoxLayout *v_box_layout = new QVBoxLayout;

        btn_start = new QPushButton;
        btn_sdf = new QPushButton;
        btn_goto = new QPushButton;
        btn_reset = new QPushButton;
        btn_stop = new QPushButton;

        btn_start->setText("start NMPC");
        btn_sdf->setText("start SDF constraint");
        btn_goto->setText("start goto wp");
        btn_reset->setText("emerg stop");
        btn_stop->setText("stop NMPC");

        v_box_layout->addWidget(btn_start);
        v_box_layout->addWidget(btn_sdf);
        v_box_layout->addWidget(btn_goto);
        v_box_layout->addWidget(btn_reset);
        v_box_layout->addWidget(btn_stop);

        QVBoxLayout *global_vbox_layout = new QVBoxLayout;
        QHBoxLayout *global_hbox_layout = new QHBoxLayout;

        global_vbox_layout->addLayout(global_hbox_layout);
        v_box_layout->addLayout(global_vbox_layout);

        setLayout(v_box_layout);

        connect(btn_start, SIGNAL(clicked()), this, SLOT(on_start_click()));
        connect(btn_sdf, SIGNAL(clicked()), this, SLOT(on_sdf_click()));
        connect(btn_goto, SIGNAL(clicked()), this, SLOT(on_goto_click()));
        connect(btn_reset, SIGNAL(clicked()), this, SLOT(on_reset_click()));
        connect(btn_stop, SIGNAL(clicked()), this, SLOT(on_stop_click()));
    }

    // on_click events
    void rmf_panel::on_start_click()
    {
        std_srvs::SetBool srv;
        srv.request.data = true;
        if (!srvc_start.call(srv))
        {
            ROS_ERROR("[RMF-UI] Service call failed: %s",
                    srvc_start.getService().c_str());
        }
    }

    void rmf_panel::on_sdf_click()
    {
        std_srvs::Trigger srv;
        if (!srvc_sdf.call(srv))
        {
            ROS_ERROR("[RMF-UI] Service call failed: %s",
                    srvc_sdf.getService().c_str());
            return;
        }
        if (sdf_on)
            btn_sdf->setText("start SDF constraint");
        else
            btn_sdf->setText("stop SDF constraint");
        sdf_on = !sdf_on;
    }

    void rmf_panel::on_goto_click()
    {
        std_srvs::Trigger srv;
        if (!srvc_goto.call(srv))
        {
            ROS_ERROR("[RMF-UI] Service call failed: %s",
                    srvc_goto.getService().c_str());
            return;
        }
        if (goto_on)
            btn_goto->setText("start goto wp");
        else
            btn_goto->setText("stop goto wp");
        goto_on = !goto_on;
    }

    void rmf_panel::on_reset_click()
    {
        std_srvs::Trigger srv;
        if (!sdf_on || !srvc_sdf.call(srv))
        {
            ROS_ERROR("[RMF-UI] Service call failed: %s",
                    srvc_goto.getService().c_str());
        }
        if (!goto_on || !srvc_goto.call(srv))
        {
            ROS_ERROR("[RMF-UI] Service call failed: %s",
                    srvc_goto.getService().c_str());
        }

        sdf_on = false;
        goto_on = false;
        btn_sdf->setText("start SDF constraint");
        btn_goto->setText("start goto wp");
    }

    void rmf_panel::on_stop_click()
    {
        std_srvs::SetBool srv;
        srv.request.data = false;
        if (!srvc_start.call(srv))
        {
            ROS_ERROR("[RMF-UI] Service call failed: %s",
                    srvc_start.getService().c_str());
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
