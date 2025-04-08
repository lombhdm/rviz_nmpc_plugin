#ifndef RMF_UI_H
#define RMF_UI_H

#include <ros/ros.h>
#include <std_msgs/ColorRGBA.h>
#include <stdio.h>
#include <std_srvs/Trigger.h>
#include <std_srvs/SetBool.h>

#ifndef Q_MOC_RUN
#include <QPainter>
#include <QLineEdit>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QLabel>
#include <QTimer>
#include <rviz/panel.h>
#endif

class QLineEdit;
class QPushButton;

namespace rviz_nmpc_plugin
{
    class rmf_panel : public rviz::Panel
    {
        Q_OBJECT
        public:
            rmf_panel(QWidget *parent = 0);
            virtual void load(const rviz::Config &config);
            virtual void save(rviz::Config config) const;

        public Q_SLOTS:
            void onclick_flag();
            void onclick_hover();
            void onclick_takeoff();
            void onclick_goto();
            void onclick_stop();

        protected Q_SLOTS:

        protected:
            QPushButton *btn_flag;
            ros::ServiceClient srvc_get_flag;
            ros::ServiceClient srvc_set_flag;
            bool get_flag_status();

            QPushButton *btn_hover;
            ros::ServiceClient srvc_hover;

            QPushButton *btn_takeoff;
            ros::ServiceClient srvc_takeoff;

            QPushButton *btn_goto;
            ros::ServiceClient srvc_goto;

            QPushButton *btn_stop;
            ros::ServiceClient srvc_stop;

            ros::NodeHandle nh;
    };

} // namespace rmf_ui

#endif // RMF_UI_H
