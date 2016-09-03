#include "actionbutton.h"

ActionButton::ActionButton(QWidget* parent) :
    QPushButton(parent)
{

}

void ActionButton::SetAction(QAction* action)
{
    m_action = action;
    connect(m_action, &QAction::triggered, this, &QPushButton::clicked);
}
