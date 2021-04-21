/*
* Copyright (C) 2019 ~ 2019 Deepin Technology Co., Ltd.
*
* Author:     leiyu <leiyu@live.com>
* Maintainer: leiyu <leiyu@deepin.com>
* This program is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* any later version.
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
* GNU General Public License for more details.
* You should have received a copy of the GNU General Public License
* along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/
#include "SlidePlayWidget.h"
#include "utils/utils.h"
#include "ModuleHeader.h"

#include <DPlatformWindowHandle>
#include <DWidget>

#include <QHBoxLayout>
#include <QMouseEvent>

SlidePlayWidget::SlidePlayWidget(QWidget *parent) : DFloatingWidget(parent)
{
    initControl();
}

void SlidePlayWidget::initControl()
{
    setBlurBackgroundEnabled(true);
    setFramRadius(18);

    m_timer.setInterval(3000);
    m_timer.start();
    connect(&m_timer, &QTimer::timeout, this, &SlidePlayWidget::onTimerout);

    QHBoxLayout *playout = new QHBoxLayout;
    playout->setContentsMargins(10, 10, 10, 10);
    playout->setSpacing(10);

    DIconButton *pbtnpre  = createBtn(QString("previous_normal"));
    DIconButton *pbtnplay = createBtn(QString("suspend_normal"));
    DIconButton *pbtnnext = createBtn(QString("next_normal"));
    DIconButton *pbtnexit = createBtn(QString("exit_normal"));
    m_playBtn = pbtnplay;

    playout->addWidget(pbtnpre);
    playout->addWidget(pbtnplay);
    playout->addWidget(pbtnnext);
    playout->addWidget(pbtnexit);

    DWidget *pwidget = new DWidget;
    pwidget->setLayout(playout);
    this->setWidget(pwidget);

    connect(pbtnpre,  &DIconButton::clicked, this, &SlidePlayWidget::onPreClicked);
    connect(pbtnplay, &DIconButton::clicked, this, &SlidePlayWidget::onPlayClicked);
    connect(pbtnnext, &DIconButton::clicked, this, &SlidePlayWidget::onNextClicked);
    connect(pbtnexit, &DIconButton::clicked, this, &SlidePlayWidget::onExitClicked);
}

void SlidePlayWidget::showControl()
{
    m_timer.start();
    this->show();
}

void SlidePlayWidget::onTimerout()
{
    this->hide();
}

void SlidePlayWidget::enterEvent(QEvent *event)
{
    m_timer.stop();
    DFloatingWidget::enterEvent(event);
}

void SlidePlayWidget::leaveEvent(QEvent *event)
{
    m_timer.start();
    DFloatingWidget::leaveEvent(event);
}

DIconButton *SlidePlayWidget::createBtn(const QString &strname)
{
    DIconButton *btn = new  DIconButton(this);
    btn->setObjectName(strname);
    btn->setFixedSize(50, 50);
    btn->setIcon(/*PF::getIcon*/QIcon::fromTheme(Pri::g_module + strname));
    btn->setIconSize(QSize(36, 36));
    return  btn;
}

void SlidePlayWidget::setPlayStatus(bool play)
{
    m_autoPlay = play;
    playStatusChanged();
}

bool SlidePlayWidget::getPlayStatus()
{
    return m_autoPlay;
}

void SlidePlayWidget::onPreClicked()
{
    emit signalPreBtnClicked();
}

void SlidePlayWidget::onPlayClicked()
{
    m_autoPlay = !m_autoPlay;
    playStatusChanged();
}

void SlidePlayWidget::onNextClicked()
{
    emit signalNextBtnClicked();
}

void SlidePlayWidget::onExitClicked()
{
    emit signalExitBtnClicked();
}

void SlidePlayWidget::playStatusChanged()
{
    if (m_autoPlay) {
        m_playBtn->setIcon(/*PF::getIcon*/QIcon::fromTheme(Pri::g_module + "suspend_normal"));
    } else {
        m_playBtn->setIcon(/*PF::getIcon*/QIcon::fromTheme(Pri::g_module + "play_normal"));
    }
    emit signalPlayBtnClicked();
}

void SlidePlayWidget::mousePressEvent(QMouseEvent *)
{
    //Nottodo
}
