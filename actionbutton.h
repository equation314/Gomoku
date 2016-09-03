#ifndef ACTIONBUTTON_H
#define ACTIONBUTTON_H

#include <QAction>
#include <QPushButton>

class ActionButton : public QPushButton
{
    Q_OBJECT

public:
    ActionButton(QWidget* parent);

    void SetAction(QAction* action);
    void SetEnabled(bool enable)
    {
        this->setEnabled(enable);
        m_action->setEnabled(enable);
    }
    void SetText(const QString& text)
    {
        this->setText(text);
        m_action->setText(text);
    }

private:
    QAction* m_action;
};

#endif // ACTIONBUTTON_H
