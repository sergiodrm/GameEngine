#pragma once

#include <QtWidgets/QWidget>
#include "ui_QtEditor.h"

class QtEditor : public QWidget
{
    Q_OBJECT

public:
    QtEditor(QWidget *parent = Q_NULLPTR);

private:
    Ui::QtEditorClass ui;
};
