﻿#ifndef TITLEWIDGET_H
#define TITLEWIDGET_H

#include <DMenu>
#include <DPushButton>

#include "CustomControl/CustomWidget.h"
#include "pdfControl/font/FontMenu.h"

/**
 * @brief The TitleWidget class
 * @brief   标题栏的 按钮操作
 */

class TitleWidget : public CustomWidget
{
    Q_OBJECT
    Q_DISABLE_COPY(TitleWidget)

public:
    explicit TitleWidget(CustomWidget *parent = nullptr);
    ~TitleWidget() Q_DECL_OVERRIDE;

signals:
    void sigSetFindWidget(const int &);
    void sigOpenFileOk();
    void sigAppFullScreen();
    void sigMagnifierCancel();

    // IObserver interface
public:
    int dealWithData(const int &, const QString &) Q_DECL_OVERRIDE;

    // CustomWidget interface
protected:
    void initWidget() Q_DECL_OVERRIDE;

private slots:
    void slotSetFindWidget(const int &);
    void slotUpdateTheme();
    void slotOpenFileOk();
    void slotAppFullScreen();
    void slotMagnifierCancel();

    void on_thumbnailBtn_clicked();
    void on_settingBtn_clicked();
    void on_handleShapeBtn_clicked();
    void on_magnifyingBtn_clicked();

    void slotActionTrigger(QAction *);
    void slotDealWithShortKey(const QString &);

private:
    void initConnections();

    void initBtns();
    void initMenus();

    void setDefaultShape();
    void setHandleShape();

    DPushButton *createBtn(const QString &btnName, bool bCheckable = false);
    void sendMsgToSubject(const int &, const QString &msgCotent = "");
    void notifyMsgToSubject(const int &, const QString &msgCotent = "");

    void setMagnifierState();

private:
    QStringList shortKeyList;

    DMenu *m_pHandleMenu = nullptr;
    FontMenu *m_pFontMenu = nullptr;

    DPushButton *m_pThumbnailBtn = nullptr;
    DPushButton *m_pSettingBtn = nullptr;
    DPushButton *m_pHandleShapeBtn = nullptr;
    DPushButton *m_pMagnifierBtn = nullptr;

    int m_nCurHandleShape = -1;  //  当前的选择工具状态
};

#endif  // TITLEWIDGET_H