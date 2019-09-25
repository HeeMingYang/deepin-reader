#ifndef THUMBNAILWIDGET_H
#define THUMBNAILWIDGET_H

#include <DListWidget>
#include <DLabel>
#include <DPushButton>

#include <QPalette>
#include <QDebug>
#include <QListWidgetItem>
#include <QVBoxLayout>
#include <QImage>
#include <QThread>
#include <QTimer>

#include "pdfControl/ThumbnailItemWidget.h"
#include "subjectObserver/CustomWidget.h"
#include "PagingWidget.h"
#include "docview/docummentproxy.h"

//const int FIRST_LOAD_PAGES = 20;

class ThumbnailWidget;

class ThreadLoadImage : public QThread
{
public:
    ThreadLoadImage();

public:
    void stopThreadRun();

    inline void setPages(const int pages)
    {
        m_pages = pages;
    }

    inline int endPage()
    {
        return m_nEndPage;
    }

    inline void setIsLoaded(const bool &load)
    {
        m_isLoaded = load;
    }

    inline bool isLoaded()
    {
        return m_isLoaded;
    }

    inline void setThumbnail(ThumbnailWidget *thumbnail)
    {
        m_pThumbnailWidget = thumbnail;
    }

    inline void setStartAndEndIndex()
    {
        m_nStartPage = 0;  // 加载图片起始页码
        m_nEndPage = 19;   // 加载图片结束页码
    }

protected:
    void run() override;

private:
    int m_pages = 0; // 文件总页数
    bool m_isLoaded = false;// 是都加载完毕
    ThumbnailWidget *m_pThumbnailWidget = nullptr;
    int m_nStartPage = 0;  // 加载图片起始页码
    int m_nEndPage = 19;   // 加载图片结束页码
};

/*
*缩略图列表页面
*/
class ThumbnailWidget : public CustomWidget
{
    Q_OBJECT

public:
    ThumbnailWidget(CustomWidget *parent = nullptr);
    ~ThumbnailWidget() override;

signals:
    void sigOpenFileOk();
    void sigSelectIndexPage(const int &);
    void sigJumpIndexPage(int);
    void sigCloseFile();

public:
    // IObserver interface
    int dealWithData(const int &, const QString &) override;
    bool fillContantToList();

    void loadImage(const int &, QImage &);

    inline bool isLoading()
    {
        return m_isLoading;
    }

protected:
    void initWidget() override;

private:
    void setSelectItemBackColor(QListWidgetItem *);
    void setCurrentRow(const int &);
    void addThumbnailItem(const int &);


    inline int preRowVal() const
    {
        return m_preRow;
    }

    inline void setPreRowVal(const int &val)
    {
        m_preRow = val;
    }

    inline int totalPages() const
    {
        return m_totalPages;
    }

    inline void setTotalPages(const int &pages)
    {
        m_totalPages = pages;
    }


private slots:
    void slotShowSelectItem(QListWidgetItem *);
    void slotOpenFileOk();
    void slotJumpIndexPage(int);
    void slotLoadThumbnailImage();
    void slotFileViewToListPage(int);
    void slotCloseFile();

private:
    DListWidget *m_pThumbnailListWidget = nullptr;
    QVBoxLayout *m_pvBoxLayout = nullptr;
    DLabel *m_pThumbnailPageLabel = nullptr;
    PagingWidget *m_pPageWidget = nullptr;

    DLabel *m_pSonWidgetPageLabel = nullptr;
    DLabel *m_pSonWidgetContantLabel = nullptr;
    ThumbnailItemWidget *m_pThumbnailItemWidget = nullptr;

    int m_totalPages = -1; // 总页码数
    int m_preRow     = -1; // 前一次页码数
    ThreadLoadImage m_ThreadLoadImage;
    bool m_isLoading = false;
//    QTimer m_loadImageTimer;
};

#endif // THUMBNAILWIDGET_H
