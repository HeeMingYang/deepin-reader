#ifndef FILEATTRWIDGET_H
#define FILEATTRWIDGET_H

#include <DLabel>
#include <DTextEdit>
#include <DWidget>
#include <DWidgetUtil>
#include <QGridLayout>
#include <QVBoxLayout>

#include <DDialog>

#include "application.h"

#include "utils/utils.h"
#include "CustomControl/ImageLabel.h"


DWIDGET_USE_NAMESPACE
/**
 * @brief The FileAttrWidget class
 * @brief   文件属性框
 */

class FileAttrWidget : public DAbstractDialog, public IObserver
{
    Q_OBJECT
    Q_DISABLE_COPY(FileAttrWidget)

public:
    explicit FileAttrWidget(DWidget *parent = nullptr);
    ~FileAttrWidget() override;
    void showScreenCenter();

public:
    int dealWithData(const int &, const QString &) override;
    void sendMsg(const int &, const QString &msgContent = "") override;
    void notifyMsg(const int &, const QString &msgContent = "") override;

private:
    void initWidget();
    void setFileAttr();
    void initCloseBtn();
    void initImageLabel();
    void addTitleFrame(const QString &);

private slots:
    void slotBtnCloseClicked();

private:
    QVBoxLayout     *m_pVBoxLayout = nullptr;
    ImageLabel *frameImage = nullptr;
};

#endif  // FILEATTRWIDGET_H