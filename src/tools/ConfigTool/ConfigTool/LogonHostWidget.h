#ifndef LOGONHOSTWIDGET_H
#define LOGONHOSTWIDGET_H

#include "ui_LogonHostWidget.h"

class LogonHostWidget : public QWidget, public Ui::LogonHostWidget
{
	Q_OBJECT

public:
	LogonHostWidget( QWidget *parent = NULL );
	~LogonHostWidget();

};

#endif

