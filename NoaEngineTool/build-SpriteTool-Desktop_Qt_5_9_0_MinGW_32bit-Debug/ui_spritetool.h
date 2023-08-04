/********************************************************************************
** Form generated from reading UI file 'spritetool.ui'
**
** Created by: Qt User Interface Compiler version 5.9.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SPRITETOOL_H
#define UI_SPRITETOOL_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_SpriteTool
{
public:

    void setupUi(QWidget *SpriteTool)
    {
        if (SpriteTool->objectName().isEmpty())
            SpriteTool->setObjectName(QStringLiteral("SpriteTool"));
        SpriteTool->resize(400, 300);

        retranslateUi(SpriteTool);

        QMetaObject::connectSlotsByName(SpriteTool);
    } // setupUi

    void retranslateUi(QWidget *SpriteTool)
    {
        SpriteTool->setWindowTitle(QApplication::translate("SpriteTool", "SpriteTool", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class SpriteTool: public Ui_SpriteTool {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SPRITETOOL_H
