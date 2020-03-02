/*
 * Copyright (C) 2019 ~ 2019 Deepin Technology Co., Ltd.
 *
 * Author:     wangzhxiaun
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
#include "PrintManager.h"

#include <QPrinter>
#include <QPainter>
#include <QPrintPreviewDialog>

#include "docview/docummentproxy.h"

#include "widgets/main/MainTabWidgetEx.h"

PrintManager::PrintManager(const QString &sPath, QObject *parent)
    : QObject(parent)
{
    setPrintPath(sPath);
}

void PrintManager::showPrintDialog(DWidget *widget)
{
    QPrinter printer;

    QPrintPreviewDialog preview(&printer, widget);
    connect(&preview, SIGNAL(paintRequested(QPrinter *)), SLOT(slotPrintPreview(QPrinter *)));
    preview.exec();
}

void PrintManager::slotPrintPreview(QPrinter *printer)
{
    MainTabWidgetEx *pMtwe = MainTabWidgetEx::Instance();
    if (pMtwe) {
        DocummentProxy *_proxy =  pMtwe->getCurFileAndProxy(m_strPrintPath);
        if (_proxy) {
            //  文档实际大小
            stFileInfo fileInfo;
            _proxy->docBasicInfo(fileInfo);

            int nPageSize = _proxy->getPageSNum();  //  pdf 页数

            printer->setDocName(m_strPrintName);

            QPainter painter(printer);
            painter.setRenderHints(QPainter::HighQualityAntialiasing | QPainter::SmoothPixmapTransform);
            painter.begin(printer);

            QRect rect = painter.viewport();

            for (int iIndex = 0; iIndex < nPageSize; iIndex++) {
                QImage image;

                bool rl = _proxy->getImage(iIndex, image, rect.width(), rect.height());
                if (rl) {
                    painter.drawPixmap(0, 0, QPixmap::fromImage(image));
                    if (iIndex < nPageSize - 1)
                        printer->newPage();
                }
            }
            painter.end();
        }
    }
}

void PrintManager::setPrintPath(const QString &strPrintPath)
{
    m_strPrintPath = strPrintPath;
    QString sPath = strPrintPath;
    int nLastPos = sPath.lastIndexOf('/');
    nLastPos++;

    m_strPrintName = sPath.mid(nLastPos);
}
