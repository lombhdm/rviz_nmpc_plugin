#ifndef RMF_UI_H
#define RMF_UI_H

#include <ros/ros.h>
#include <std_msgs/ColorRGBA.h>
#include <stdio.h>
#include <std_srvs/Empty.h>
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
            void on_start_click();
            void on_sdf_click();
            void on_goto_click();
            void on_reset_click();
            void on_stop_click();

        protected Q_SLOTS:

        protected:
            QPushButton *btn_start;
            ros::ServiceClient srvc_start;

            QPushButton *btn_sdf;
            ros::ServiceClient srvc_sdf;
            bool sdf_on = false;

            QPushButton *btn_goto;
            ros::ServiceClient srvc_goto;
            bool goto_on = false;

            QPushButton *btn_reset;
            ros::ServiceClient srvc_reset;

            QPushButton *btn_stop;
            ros::ServiceClient srvc_stop;

            ros::NodeHandle nh;
    };

} // namespace rmf_ui

#endif // RMF_UI_H
