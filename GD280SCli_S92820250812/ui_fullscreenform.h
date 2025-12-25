/********************************************************************************
** Form generated from reading UI file 'fullscreenform.ui'
**
** Created by: Qt User Interface Compiler version 5.9.9
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_FULLSCREENFORM_H
#define UI_FULLSCREENFORM_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_FullScreenForm
{
public:
    QLabel *lblfullPic;

    void setupUi(QWidget *FullScreenForm)
    {
        if (FullScreenForm->objectName().isEmpty())
            FullScreenForm->setObjectName(QStringLiteral("FullScreenForm"));
        FullScreenForm->resize(1920, 1082);
        QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(FullScreenForm->sizePolicy().hasHeightForWidth());
        FullScreenForm->setSizePolicy(sizePolicy);
        FullScreenForm->setMinimumSize(QSize(1920, 1082));
        FullScreenForm->setMaximumSize(QSize(3840, 2160));
        lblfullPic = new QLabel(FullScreenForm);
        lblfullPic->setObjectName(QStringLiteral("lblfullPic"));
        lblfullPic->setGeometry(QRect(0, 0, 1920, 1080));
        QSizePolicy sizePolicy1(QSizePolicy::Maximum, QSizePolicy::Maximum);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(lblfullPic->sizePolicy().hasHeightForWidth());
        lblfullPic->setSizePolicy(sizePolicy1);
        lblfullPic->setStyleSheet(QStringLiteral("background-color: rgb(0, 0, 0);"));
        lblfullPic->setAlignment(Qt::AlignCenter);

        retranslateUi(FullScreenForm);

        QMetaObject::connectSlotsByName(FullScreenForm);
    } // setupUi

    void retranslateUi(QWidget *FullScreenForm)
    {
        FullScreenForm->setWindowTitle(QApplication::translate("FullScreenForm", "\345\205\250\345\261\217", Q_NULLPTR));
        lblfullPic->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class FullScreenForm: public Ui_FullScreenForm {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_FULLSCREENFORM_H
