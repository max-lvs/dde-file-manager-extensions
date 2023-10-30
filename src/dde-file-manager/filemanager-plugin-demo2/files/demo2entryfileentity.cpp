// SPDX-FileCopyrightText: 2022 - 2023 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "demo2entryfileentity.h"
#include "utils/demo2manager.h"

#include <QApplication>

DFMBASE_USE_NAMESPACE
using namespace dfmplugin_demo2;

Demo2EntryFileEntity::Demo2EntryFileEntity(const QUrl &url)
    : AbstractEntryFileEntity(url)
{

}

Demo2EntryFileEntity::~Demo2EntryFileEntity()
{

}

QString Demo2EntryFileEntity::displayName() const
{
    return tr("My Demo2");
}

QIcon Demo2EntryFileEntity::icon() const
{
    return QIcon::fromTheme("window-new");
}

bool Demo2EntryFileEntity::exists() const
{
    qDebug() << " =================Demo2EntryFileEntity::exists() ";
    return true;
}

bool Demo2EntryFileEntity::showProgress() const
{
    return false;
}

bool Demo2EntryFileEntity::showTotalSize() const
{
    return false;
}

bool Demo2EntryFileEntity::showUsageSize() const
{
    return false;
}

AbstractEntryFileEntity::EntryOrder Demo2EntryFileEntity::order() const
{
//    EntryFileInfo::EntryOrder order = EntryFileInfo::EntryOrder(static_cast<int>(EntryFileInfo::EntryOrder::kOrderCustom) + 1);
//    return order;
    return AbstractEntryFileEntity::kOrderApps;
}

void Demo2EntryFileEntity::refresh()
{
}

quint64 Demo2EntryFileEntity::sizeTotal() const
{
    if (vaultTotal > 0)
        return vaultTotal;
    else
        return totalchange;
}

QUrl Demo2EntryFileEntity::targetUrl() const
{
    return Demo2Manager::instance()->rootUrl();
}

void Demo2EntryFileEntity::slotFileDirSizeChange(qint64 size, int filesCount, int directoryCount)
{
    Q_UNUSED(filesCount)
    Q_UNUSED(directoryCount)
    if (showSizeState) {
        totalchange = size;
        if (vaultTotal > 0 && totalchange > vaultTotal)
            vaultTotal = totalchange;
    }
}

void Demo2EntryFileEntity::slotFinishedThread()
{
    showSizeState = false;
    vaultTotal = totalchange;
}
