/********************************************************************************
** Form generated from reading UI file 'dlgAutoHide.ui'
**
** Created by: Qt User Interface Compiler version 5.9.9
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_DLGAUTOHIDE_H
#define UI_DLGAUTOHIDE_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_TipsDlg
{
public:
    QLabel *tipsLabel;

    void setupUi(QWidget *TipsDlg)
    {
        if (TipsDlg->objectName().isEmpty())
            TipsDlg->setObjectName(QStringLiteral("TipsDlg"));
        TipsDlg->setWindowModality(Qt::NonModal);
        TipsDlg->resize(603, 108);
        QFont font;
        font.setFamily(QString::fromUtf8("\346\226\260\345\256\213\344\275\223"));
        font.setPointSize(16);
        font.setBold(true);
        font.setWeight(75);
        TipsDlg->setFont(font);
        TipsDlg->setAutoFillBackground(false);
        TipsDlg->setStyleSheet(QStringLiteral("background-color: rgba(255, 255, 255, 200);"));
        tipsLabel = new QLabel(TipsDlg);
        tipsLabel->setObjectName(QStringLiteral("tipsLabel"));
        tipsLabel->setGeometry(QRect(10, 19, 571, 51));
        QFont font1;
        font1.setPointSize(18);
        font1.setBold(true);
        font1.setWeight(75);
        tipsLabel->setFont(font1);
        tipsLabel->setStyleSheet(QStringLiteral("background-color: rgba(255, 255, 255, 0);"));
        tipsLabel->setAlignment(Qt::AlignCenter);

        retranslateUi(TipsDlg);

        QMetaObject::connectSlotsByName(TipsDlg);
    } // setupUi

    void retranslateUi(QWidget *TipsDlg)
    {
        TipsDlg->setWindowTitle(QApplication::translate("TipsDlg", "Tips", Q_NULLPTR));
        tipsLabel->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class TipsDlg: public Ui_TipsDlg {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_DLGAUTOHIDE_H
