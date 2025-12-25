#ifndef DLGAUTOHIDE_H
#define DLGAUTOHIDE_H

#include <QWidget>
#include <QTimer>

enum TipsType
{
    TipsType_Unknown =          0,
    TipsType_Warnning =         1,//警告
    TipsType_Ok =               2//成功
};

namespace Ui {
class TipsDlg;
}
class TipsDlg : public QWidget
{
    Q_OBJECT

public:
    explicit TipsDlg(QWidget *parent = nullptr);
    ~TipsDlg();

    void startTimer();
    /**
     * @brief initFrame 初始化对话框
     * @param msg       提示信息
     */
    void initFrame(const QString &msg);

private:


private:
    Ui::TipsDlg *ui;
    TipsType m_type;
    QTimer *m_pTimer;
};
#endif // DLGAUTOHIDE_H
