#ifndef LOGONLOGWIDGET_H
#define LOGONLOGWIDGET_H

#include "ui_LogonLogWidget.h"

class LogonLogWidget : public QWidget, public Ui::LogonLogWidget
{
	Q_OBJECT
public:
	LogonLogWidget( QWidget *parent = NULL );
	~LogonLogWidget();
};

#endif

