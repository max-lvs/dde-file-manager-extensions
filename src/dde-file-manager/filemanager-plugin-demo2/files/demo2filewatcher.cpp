// SPDX-FileCopyrightText: 2022 - 2023 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "demo2filewatcher.h"
#include <dfm-base/base/schemefactory.h>

DFMBASE_USE_NAMESPACE
using namespace dfmplugin_demo2;

Demo2FileWatcher::Demo2FileWatcher(const QUrl &url, QObject *parent)
    : AbstractFileWatcher(nullptr, parent)
{

}

Demo2FileWatcher::~Demo2FileWatcher()
{
}

void Demo2FileWatcher::onFileDeleted(const QUrl &url)
{
    QUrl furl = QUrl(url);//need to change the url base on your bussiness;
    emit fileDeleted(furl);
}

void Demo2FileWatcher::onFileAttributeChanged(const QUrl &url)
{
    QUrl furl = QUrl(url);//need to change the url base on your bussiness;
    emit fileAttributeChanged(furl);
}

void Demo2FileWatcher::onFileRename(const QUrl &fromUrl, const QUrl &toUrl)
{
    QUrl furl = QUrl(fromUrl);//need to change the url base on your bussiness;
    QUrl turl = QUrl(toUrl);//need to change the url base on your bussiness;
    emit fileRename(furl, turl);
}

void Demo2FileWatcher::onSubfileCreated(const QUrl &url)
{
    QUrl furl = QUrl(url);//need to change the url base on your bussiness;
    qDebug() << url;
    emit subfileCreated(furl);
}
