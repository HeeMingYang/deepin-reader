#include "NotesWidget.h"

NotesWidget::NotesWidget(DWidget *parent) :
    DWidget(parent)
{
    m_pThemeSubject = ThemeSubject::getInstace();
    if(m_pThemeSubject)
    {
        m_pThemeSubject->addObserver(this);
    }
}

NotesWidget::~NotesWidget()
{
    if(m_pThemeSubject)
    {
        m_pThemeSubject->removeObserver(this);
    }
}

int NotesWidget::update(const QString &)
{
    return 0;
}
