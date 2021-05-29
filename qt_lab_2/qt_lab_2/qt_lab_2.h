#pragma once

#include <QtWidgets/QWidget>
#include "ui_qt_lab_2.h"

class qt_lab_2 : public QWidget
{
    Q_OBJECT

public:
    qt_lab_2(QWidget *parent = Q_NULLPTR);

private:
    Ui::qt_lab_2Class ui;
};
