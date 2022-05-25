#include "Model.h"
#include "PDFModel.h"
#include "DjVuModel.h"
#include "dpdfannot.h"
#include "dpdfpage.h"
#include "dpdfdoc.h"

#include <QProcess>
#include <QFile>
#include <QDir>
#include <QTimer>

namespace deepin_reader {
deepin_reader::Document *deepin_reader::DocumentFactory::getDocument(const int &fileType,
                                                                     const QString &filePath,
                                                                     const QString &convertedFileDir,
                                                                     const QString &password,
                                                                     QProcess **pprocess,
                                                                     deepin_reader::Document::Error &error)
{
    deepin_reader::Document *document = nullptr;

    if (Dr::PDF == fileType) {
        document = deepin_reader::PDFDocument::loadDocument(filePath, password, error);
    } else if (Dr::DJVU == fileType) {
        document = deepin_reader::DjVuDocument::loadDocument(filePath, error);
    } else if (Dr::DOCX == fileType) {
        if (nullptr == pprocess) {
            error = deepin_reader::Document::ConvertFailed;
            return nullptr;
        }
        QString targetDoc = convertedFileDir + "/temp.docx";
        QString tmpHtmlFilePath = convertedFileDir + "/word/temp.html";
        QString realFilePath = convertedFileDir + "/temp.pdf";

        QFile file(filePath);
        if (!file.copy(targetDoc)) {
            qInfo() << QString("copy %1 failed.").arg(filePath);
            error = deepin_reader::Document::ConvertFailed;
            return nullptr;
        }

        //解压的目的是为了让资源文件可以被转换的时候使用到，防止转换后丢失图片等媒体信息
        QProcess decompressor;
        *pprocess = &decompressor;
        decompressor.setWorkingDirectory(convertedFileDir);
        decompressor.start("unzip " + targetDoc);
        if (!decompressor.waitForStarted()) {
            qInfo() << "start unzip failed";
            error = deepin_reader::Document::ConvertFailed;
            *pprocess = nullptr;
            return nullptr;
        }
        if (!decompressor.waitForFinished()) {
            qInfo() << "unzip failed";
            error = deepin_reader::Document::ConvertFailed;
            *pprocess = nullptr;
            return nullptr;
        }
        if (!QDir(convertedFileDir + "/word").exists()) {
            qInfo() << "unzip failed";
            error = deepin_reader::Document::ConvertFailed;
            if (!(QProcess::CrashExit == decompressor.exitStatus() && 9 == decompressor.exitCode())) {
                *pprocess = nullptr;
            }
            return nullptr;
        }

        // docx -> html
        QProcess converter;
        *pprocess = &converter;
        converter.setWorkingDirectory(convertedFileDir + "/word");
        converter.start("pandoc " +  targetDoc + " -o " + tmpHtmlFilePath);
        if (!converter.waitForStarted()) {
            qInfo() << "start pandoc failed";
            error = deepin_reader::Document::ConvertFailed;
            *pprocess = nullptr;
            return nullptr;
        }
        if (!converter.waitForFinished()) {
            qInfo() << "pandoc failed";
            error = deepin_reader::Document::ConvertFailed;
            *pprocess = nullptr;
            return nullptr;
        }
           QFile tmpHtmlFile(tmpHtmlFilePath);
           if (!tmpHtmlFile.exists()) {
            qInfo() << "temp.html doesn't exist";
            error = deepin_reader::Document::ConvertFailed;
            // 转换过程中关闭应用，docsheet被释放，对应的*pprocess已不存在
            if (!(QProcess::CrashExit == converter.exitStatus() && 9 == converter.exitCode())) {
                *pprocess = nullptr;
            }
            return nullptr;
        }

        // html -> pdf
        QProcess converter2;
        *pprocess = &converter2;
        converter2.setWorkingDirectory(convertedFileDir + "/word");
        converter2.start("/usr/lib/deepin-reader/htmltopdf " +  tmpHtmlFilePath + " " + realFilePath);
        if (!converter2.waitForStarted()) {
            qInfo() << "start htmltopdf failed";
            error = deepin_reader::Document::ConvertFailed;
            *pprocess = nullptr;
            return nullptr;
        }
        if (!converter2.waitForFinished()) {
            qInfo() << "htmltopdf failed";
            error = deepin_reader::Document::ConvertFailed;
            *pprocess = nullptr;
            return nullptr;
        }

        QFile realFile(realFilePath);
        if (!realFile.exists()) {
            qInfo() << "temp.pdf doesn't exist";
            error = deepin_reader::Document::ConvertFailed;
            if (!(QProcess::CrashExit == converter.exitStatus() && 9 == converter.exitCode())) {
                *pprocess = nullptr;
            }
            return nullptr;
        }

        *pprocess = nullptr;
        document = deepin_reader::PDFDocument::loadDocument(realFilePath, password, error);
    }

    return document;
}

bool SearchResult::setctionsFillText(std::function<QString (int, QRectF)> getText, qreal xp1, qreal yp1, qreal xp2, qreal yp2)
{
    bool ret = false;
    for(auto &section : sections) {
        for(auto &line : section) {
            //这里的page比index大1
            int index = page - 1;
            QString text = getText(index, line.rect.adjusted(xp1, yp1, xp2, yp2));
            if(!text.isEmpty()) {
                line.text = text;
                ret = true;
            }
        }
    }
    return ret;
}

QRectF SearchResult::sectionBoundingRect(const PageSection &section)
{
    QRectF ret;
    for(const PageLine &line : section) {
        ret = ret.united(line.rect);
    }
    return ret;
}

}
