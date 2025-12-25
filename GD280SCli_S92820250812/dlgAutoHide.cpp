#include "dlgAutoHide.h"
#include "ui_dlgAutoHide.h"


  TipsDlg::TipsDlg(QWidget *parent) :
      QWidget(parent),
      ui(new Ui::TipsDlg)
{
      ui->setupUi(this);
      this->setWindowFlag(Qt::FramelessWindowHint);

    m_pTimer = new QTimer(this);
    m_pTimer->setSingleShot(true);
    connect(m_pTimer, &QTimer::timeout, this, [=](){this->close();});
}

TipsDlg::~TipsDlg()
{
    if(m_pTimer != Q_NULLPTR)
        m_pTimer->deleteLater();

    delete ui;
    this->close();
}

void TipsDlg::startTimer()
{
    m_pTimer->start(3000);
}

void TipsDlg::initFrame(const QString &msg)
{

    ui->tipsLabel->setText(msg);

}
