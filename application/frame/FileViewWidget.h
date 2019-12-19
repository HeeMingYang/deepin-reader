#ifndef FILEVIEWWIDGET_H
#define FILEVIEWWIDGET_H

#include <QMouseEvent>

#include "CustomControl/CustomWidget.h"
#include "DocummentFileHelper.h"
#include "NoteTipWidget.h"
#include "mainShow/TextOperationMenu.h"

//  当前鼠标状态
enum Handel_Enum {
    Default_State,
    Handel_State,
    Magnifier_State
};

//  窗口自适应状态
enum ADAPTE_Enum {
    NO_ADAPTE_State,
    WIDGET_State,
    HEIGHT_State
};

/**
 * @brief The FileViewWidget class
 * @brief   文档显示区域
 */
class FileViewWidget : public CustomWidget
{
    Q_OBJECT
    Q_DISABLE_COPY(FileViewWidget)

public:
    explicit FileViewWidget(CustomWidget *parent = nullptr);
    ~FileViewWidget() Q_DECL_OVERRIDE;

signals:
    void sigDealWithData(const int &, const QString &);

    void sigPrintFile();
    void sigSaveFile();
    void sigCopySelectContent(const QString &);
    void sigFileSlider(const int &);

//    void sigSetHandShape(const QString &);
//    void sigMagnifying(const QString &);

//    void sigWidgetAdapt();

//    void sigFileAddAnnotation(const QString &);
    void sigFileAddAnnotation();
//    void sigFileUpdateAnnotation(const QString &);
//    void sigFileRemoveAnnotation(const QString &);

//    void sigFileAddNote(const QString &);
    void sigFileAddNote();

    void sigFileCtrlContent();
    void sigShowPlayCtrl(bool bshow);

protected:
    void mouseMoveEvent(QMouseEvent *event) Q_DECL_OVERRIDE;
    void mousePressEvent(QMouseEvent *event) Q_DECL_OVERRIDE;
    void mouseReleaseEvent(QMouseEvent *event) Q_DECL_OVERRIDE;
    void leaveEvent(QEvent *event) Q_DECL_OVERRIDE;
    void resizeEvent(QResizeEvent *event) Q_DECL_OVERRIDE;

private slots:
    void slotDealWithData(const int &, const QString &);

    void slotCustomContextMenuRequested(const QPoint &);

    void slotPrintFile();

    void slotFileAddAnnotation();

    void slotFileAddNote();

    void slotBookMarkStateChange(int, bool);
    void slotDocFilePageChanged(int);

    void slotFileCtrlContent();

private:
    void initConnections();
    void onClickPageLink(Page::Link *pLink);
    void onShowNoteTipWidget(const QPoint &docPos);

    void onMagnifying(const QString &);
    void onSetHandShape(const QString &);
    void onSetWidgetAdapt();

    void onFileAddAnnotation(const QString &);
    void onFileUpdateAnnotation(const QString &);
    void onFileRemoveAnnotation(const QString &);

    void onFileAddNote(const QString &);

    int dealWithFileMenuRequest(const int &msgType, const QString &msgContent);
    int dealWithNotifyMsg(const QString &msgContent);

private:
    TextOperationMenu       *m_operatemenu = nullptr;
    DocummentFileHelper     *m_pDocummentFileHelper = nullptr;
    NoteTipWidget           *m_pNoteTipWidget = nullptr;

private:
    int         m_nCurrentHandelState = Default_State;  //  当前鼠标状态
    int         m_nAdapteState = NO_ADAPTE_State;       //  当前自适应状态
    bool        m_bSelectOrMove = false;                //  是否可以选中文字、移动
    bool        m_bIsHandleSelect = false;              //  是否可以选中

    QPoint      m_pStartPoint;
    QPoint      m_pEndSelectPoint;
    QPoint      m_pHandleMoveStartPoint;

    // CustomWidget interface
protected:
    void initWidget() Q_DECL_OVERRIDE;

    // IObserver interface
public:
    int dealWithData(const int &, const QString &) Q_DECL_OVERRIDE;
};


#endif // FILEVIEWWIDGET_H
