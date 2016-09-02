#include "dialpaddialog.h"
#include "ui_dialpaddialog.h"

#include <QPushButton>
#include <QSignalMapper>

DialPadDialog::DialPadDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialPadDialog)
{
    ui->setupUi(this);
    this->setWindowFlags(Qt::Tool);

    QSignalMapper *signalMap = new QSignalMapper(this);
    for (int i = 0; i <= 9; i++)
    {
        m_num[i] = new QPushButton(QString::number(i), this);
        m_num[i]->setFixedSize(48, 48);
        m_num[i]->setShortcut(QString::number(i));
        m_num[i]->setAutoDefault(false);
        if (!i)
            ui->gridLayout->addWidget(m_num[i], 3, 1);
        else
            ui->gridLayout->addWidget(m_num[i], (i - 1) / 3, (i - 1) % 3);

        signalMap->setMapping(m_num[i], i);
        connect(m_num[i], SIGNAL(clicked()), signalMap, SLOT(map()));
    }

    m_backspace = new QPushButton("←", this);
    m_backspace->setFixedSize(48, 48);
    m_backspace->setShortcut(Qt::Key_Backspace);
    m_backspace->setAutoDefault(false);
    ui->gridLayout->addWidget(m_backspace, 3, 0);
    signalMap->setMapping(m_backspace, -1);
    connect(m_backspace, SIGNAL(clicked()), signalMap, SLOT(map()));

    m_clear = new QPushButton(tr("Clear"), this);
    m_clear->setFixedSize(48, 48);
    m_clear->setShortcut(Qt::Key_C);
    m_clear->setAutoDefault(false);
    ui->gridLayout->addWidget(m_clear, 3, 2);
    signalMap->setMapping(m_clear, -2);
    connect(m_clear, SIGNAL(clicked()), signalMap, SLOT(map()));

    connect(signalMap, SIGNAL(mapped(int)), this, SIGNAL(numClicked(int)));

    this->setFixedSize(this->sizeHint());
}

DialPadDialog::~DialPadDialog()
{
    delete ui;
}
