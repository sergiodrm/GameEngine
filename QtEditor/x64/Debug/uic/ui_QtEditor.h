/********************************************************************************
** Form generated from reading UI file 'QtEditor.ui'
**
** Created by: Qt User Interface Compiler version 5.15.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_QTEDITOR_H
#define UI_QTEDITOR_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_QtEditorClass
{
public:

    void setupUi(QWidget *QtEditorClass)
    {
        if (QtEditorClass->objectName().isEmpty())
            QtEditorClass->setObjectName(QString::fromUtf8("QtEditorClass"));
        QtEditorClass->resize(600, 400);

        retranslateUi(QtEditorClass);

        QMetaObject::connectSlotsByName(QtEditorClass);
    } // setupUi

    void retranslateUi(QWidget *QtEditorClass)
    {
        QtEditorClass->setWindowTitle(QCoreApplication::translate("QtEditorClass", "QtEditor", nullptr));
    } // retranslateUi

};

namespace Ui {
    class QtEditorClass: public Ui_QtEditorClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_QTEDITOR_H
