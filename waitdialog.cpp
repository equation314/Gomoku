#include "waitdialog.h"
#include "ui_waitdialog.h"

#include <QCloseEvent>

WaitDialog::WaitDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::WaitDialog),
    m_allowed(false)
{
    ui->setupUi(this);
    this->setWindowFlags(Qt::Dialog | Qt::WindowCloseButtonHint);
    this->setFixedSize(240, 100);
}

WaitDialog::~WaitDialog()
{
    delete ui;
}

void WaitDialog::closeEvent(QCloseEvent* event)
{
    if (!ui->buttonBox->isEnabled()) event->ignore();
}



void WaitDialog::onBackAllowed()
{
    m_allowed = true;
    ui->label->setText(tr("Back request was allowed."));
    ui->buttonBox->setEnabled(true);
    this->setWindowTitle(tr("Allowed"));
    this->activateWindow();
    emit backAllowed();
}

void WaitDialog::onBackDisallowed()
{
    m_allowed = false;
    ui->label->setText(tr("Back request was not allowed."));
    ui->buttonBox->setEnabled(true);
    this->setWindowTitle(tr("Not Allowed"));
    this->activateWindow();
    emit backDisallowed();
}

