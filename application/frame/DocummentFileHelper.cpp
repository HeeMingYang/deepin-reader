#include "DocummentFileHelper.h"
#include <DApplication>
#include <QClipboard>
#include "subjectObserver/MsgHeader.h"
#include "controller/DataManager.h"
#include <DFileDialog>
#include "utils/utils.h"
#include <QDesktopServices>
#include "translator/Frame.h"
#include <DMessageBox>

DocummentFileHelper::DocummentFileHelper(QObject *parent) : QObject(parent)
{
    m_pDocummentProxy = DocummentProxy::instance();
    setObserverName("DocummentFileHelper");

    initConnections();

    m_pMsgSubject = MsgSubject::getInstance();
    if (m_pMsgSubject) {
        m_pMsgSubject->addObserver(this);
    }

    m_pNotifySubject = NotifySubject::getInstance();
    if (m_pNotifySubject) {
        m_pNotifySubject->addObserver(this);
    }
}

DocummentFileHelper::~DocummentFileHelper()
{
    if (m_pDocummentProxy && m_szFilePath != "") {
        m_pDocummentProxy->closeFile();
    }
}

//  ctrl s  保存文件
void DocummentFileHelper::onSaveFileCtrlS()
{
    m_pDocummentProxy->save(m_szFilePath, true);
}

//  保存
void DocummentFileHelper::onSaveFile()
{
    m_pDocummentProxy->save(m_szFilePath, true);
}

//  另存为
void DocummentFileHelper::onSaveAsFile()
{
    QString sFilter = "";
    if (m_nCurDocType == DocType_PDF) {
        sFilter = "Pdf File (*.pdf)";
    } else if (m_nCurDocType == DocType_TIFF) {
        sFilter = "Tiff files (*.tiff)";
    }
    if (sFilter != "") {
        DFileDialog dialog;
        dialog.selectFile(m_szFilePath);
        QString filePath = dialog.getSaveFileName(nullptr, Frame::sSaveFile,
                                                  m_szFilePath, sFilter);
        if (filePath != "") {
            if (m_nCurDocType == DocType_PDF) {
                if (!filePath.endsWith(".pdf")) {
                    filePath += ".pdf";
                }
            } else if (m_nCurDocType == DocType_TIFF) {
                if (!filePath.endsWith(".tiff")) {
                    filePath += ".tiff";
                }
            }

            m_pDocummentProxy->save(filePath, true);

            m_szFilePath = filePath;

            QFileInfo info(m_szFilePath);
            QString sTitle = "";
            m_pDocummentProxy->title(sTitle);
            if (sTitle == "") {
                sTitle = info.baseName();
            }
            sendMsg(MSG_OPERATION_OPEN_FILE_TITLE, sTitle);
        }
    }
}

//  打开　文件路径
void DocummentFileHelper::slotOpenFile(const QString &filePaths)
{
    //  已经打开了文件，　询问是否需要保存当前打开的文件
    if (m_szFilePath != "") {
        if (QMessageBox::Yes == DMessageBox::question(nullptr, Frame::sSaveFile, Frame::sSaveFileTitle)) {
            m_pDocummentProxy->save(m_szFilePath, true);
        }
        sendMsg(MSG_CLOSE_FILE);
        m_pDocummentProxy->closeFile();
    }

    QStringList fileList = filePaths.split("@#&wzx",  QString::SkipEmptyParts);
    int nSize = fileList.size();
    if (nSize > 0) {
        QString sPath = fileList.at(0);

        QFileInfo info(sPath);
        QString sCompleteSuffix = info.completeSuffix();
        if (sCompleteSuffix == "pdf") {
            m_nCurDocType = DocType_PDF;
        } else if (sCompleteSuffix == "tiff") {
            m_nCurDocType = DocType_TIFF;
        }
        bool rl = m_pDocummentProxy->openFile(m_nCurDocType, sPath);
        if (rl) {
            m_szFilePath = sPath;
            DataManager::instance()->setStrOnlyFilePath(sPath);
            //  通知 其他窗口， 打开文件成功了！！！
            NotifySubject::getInstance()->sendMsg(MSG_OPERATION_OPEN_FILE_OK);

            QString sTitle = "";
            m_pDocummentProxy->title(sTitle);
            if (sTitle == "") {
                sTitle = info.baseName();
            }
            sendMsg(MSG_OPERATION_OPEN_FILE_TITLE, sTitle);
        } else {
            sendMsg(MSG_OPERATION_OPEN_FILE_FAIL);
        }
    }
}

//  复制
void DocummentFileHelper::onCopySelectContent()
{
    QString sCopy = "";
    bool rl = m_pDocummentProxy->getSelectTextString(sCopy);
    if (rl && sCopy != "") {
        QClipboard *clipboard = DApplication::clipboard();   //获取系统剪贴板指针
        clipboard->setText(sCopy);
    }
}

//  放映
void DocummentFileHelper::onFileSlider()
{
    bool bSlideModel = m_pDocummentProxy->showSlideModel();    //  开启幻灯片
    if (bSlideModel) {
        DataManager::instance()->setCurShowState(FILE_SLIDE);
    }
}

//  文档　跳转页码　．　打开浏览器
void DocummentFileHelper::onClickPageLink(Page::Link *pLink)
{
    Page::LinkType_EM linkType = pLink->type;
    if (linkType == Page::LinkType_NULL) {

    } else if (linkType == Page::LinkType_Goto) {
        int page = pLink->page;
        m_pDocummentProxy->pageJump(page);
    } else if (linkType == Page::LinkType_GotoOtherFile) {

    } else if (linkType == Page::LinkType_Browse) {
        QString surlOrFileName = pLink->urlOrFileName;
        QDesktopServices::openUrl(QUrl(surlOrFileName, QUrl::TolerantMode));
    } else if (linkType == Page::LinkType_Execute) {

    }
}


void DocummentFileHelper::initConnections()
{
    connect(this, SIGNAL(sigOpenFile(const QString &)), this, SLOT(slotOpenFile(const QString &)));
}

int DocummentFileHelper::dealWithData(const int &msgType, const QString &msgContent)
{
    switch (msgType) {
    case MSG_OPEN_FILE_PATH:                //  打开文件
        emit sigOpenFile(msgContent);
        return ConstantMsg::g_effective_res;
    case MSG_OPERATION_SAVE_FILE :          //  保存文件
        onSaveFile();
        return ConstantMsg::g_effective_res;
    case MSG_OPERATION_SAVE_AS_FILE:        //  另存为文件
        onSaveAsFile();
        return ConstantMsg::g_effective_res;
    case MSG_OPERATION_TEXT_COPY:           //  复制
        onCopySelectContent();
        return ConstantMsg::g_effective_res;
    case MSG_OPERATION_SLIDE:               //  放映
        onFileSlider();
        return ConstantMsg::g_effective_res;
    case MSG_NOTIFY_KEY_MSG :
        if ("Ctrl+S" == msgContent) {
            onSaveFileCtrlS();
            return ConstantMsg::g_effective_res;
        }
        if ("Esc" == msgContent) {
            if (DataManager::instance()->CurShowState() == FILE_SLIDE) {    //  幻灯片模式，　需要退出
                m_pDocummentProxy->exitSlideModel();
                DataManager::instance()->setCurShowState(FILE_NORMAL);
            }
        }
    }
    return 0;
}

void DocummentFileHelper::sendMsg(const int &msgType, const QString &msgContent)
{
    m_pMsgSubject->sendMsg(this, msgType, msgContent);
}

void DocummentFileHelper::setObserverName(const QString &name)
{
    m_strObserverName = name;
}
