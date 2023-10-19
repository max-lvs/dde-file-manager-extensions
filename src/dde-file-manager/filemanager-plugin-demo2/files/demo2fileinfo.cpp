// SPDX-FileCopyrightText: 2022 - 2023 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "demo2fileinfo.h"

#include <dfm-base/base/schemefactory.h>

#include <dfm-io/dfmio_utils.h>

#include <DFileIconProvider>

#include <QDateTime>
#include <QDir>
#include <QDebug>
#include <qplatformdefs.h>

#include <sys/stat.h>

DWIDGET_USE_NAMESPACE
DFMBASE_USE_NAMESPACE
using namespace dfmplugin_demo2;


Demo2FileInfo::Demo2FileInfo(const QUrl &url)
    : ProxyFileInfo(url)
{
    QUrl localUrl = url;//ToLocalUrl(url);
    setProxy(InfoFactory::create<FileInfo>(localUrl,  Global::CreateFileInfoType::kCreateFileInfoAsyncAndCache));
}

Demo2FileInfo::Demo2FileInfo(const QUrl &url, const FileInfoPointer &proxy)
    : ProxyFileInfo(url)
{
    setProxy(proxy);
}

Demo2FileInfo::~Demo2FileInfo()
{
}

Demo2FileInfo &Demo2FileInfo::operator=(const Demo2FileInfo &fileinfo)
{
    ProxyFileInfo::operator=(fileinfo);
    if (!proxy)
        setProxy(fileinfo.proxy);
    else {
        url = fileinfo.url;
        proxy = fileinfo.proxy;
    }
    return *this;
}

bool Demo2FileInfo::operator==(const Demo2FileInfo &fileinfo) const
{
    return proxy == fileinfo.proxy && url == fileinfo.url;
}

bool Demo2FileInfo::operator!=(const Demo2FileInfo &fileinfo) const
{
    return !(operator==(fileinfo));
}

QString Demo2FileInfo::pathOf(const PathInfoType type) const
{
    switch (type) {
    case FilePathInfoType::kAbsolutePath:
        if (!proxy)
            return "";
        return proxy->pathOf(type);
    default:
        return ProxyFileInfo::pathOf(type);
    }
}

QUrl Demo2FileInfo::urlOf(const UrlInfoType type) const
{
    switch (type) {
    case FileUrlInfoType::kUrl:
        return url;
    case FileUrlInfoType::kRedirectedFileUrl:
        return url;//return d->localUrl;
    default:
        return ProxyFileInfo::urlOf(type);
    }
}
bool Demo2FileInfo::exists() const
{
    qDebug() << " =================Demo2FileInfo::exists() ";
    if (urlOf(UrlInfoType::kUrl).isEmpty())
        return false;

    return proxy && proxy->exists();
   // return true;
}

void Demo2FileInfo::refresh()
{
    if (!proxy) {
        return;
    }

    proxy->refresh();
}

bool Demo2FileInfo::isAttributes(const OptInfoType type) const
{
    switch (type) {
    case FileIsType::kIsFile:
        [[fallthrough]];
    case FileIsType::kIsDir:
        [[fallthrough]];
    case FileIsType::kIsReadable:
        [[fallthrough]];
    case FileIsType::kIsWritable:
        [[fallthrough]];
    case FileIsType::kIsExecutable:
        [[fallthrough]];
    case FileIsType::kIsSymLink:
        [[fallthrough]];
    case FileIsType::kIsHidden:
        return !proxy || proxy->isAttributes(type);
    default:
        return ProxyFileInfo::isAttributes(type);
    }
}

bool Demo2FileInfo::canAttributes(const CanableInfoType type) const
{
    switch (type) {
    case FileCanType::kCanDrop:
        return !proxy || proxy->canAttributes(type);
    case FileCanType::kCanRedirectionFileUrl:
        return proxy;
    default:
        return ProxyFileInfo::canAttributes(type);
    }
}

QVariantHash Demo2FileInfo::extraProperties() const
{
    if (!proxy)
        return ProxyFileInfo::extraProperties();
    return proxy->extraProperties();
}

QUrl Demo2FileInfo::getUrlByType(const UrlInfoType type, const QString &fileName) const
{
    switch (type) {
    case FileUrlInfoType::kGetUrlByNewFileName:

    default:
        return ProxyFileInfo::getUrlByType(type, fileName);
    }
}

QIcon Demo2FileInfo::fileIcon()
{
//    if (d->isRoot) {
//        QString iconName = "window-new";   // 如果是根目录，用自定义图标
//        return QIcon::fromTheme(iconName);
//    }

    if (!proxy)
        return ProxyFileInfo::fileIcon();
    return proxy->fileIcon();
}

QString Demo2FileInfo::viewOfTip(const FileInfo::ViewType type) const
{
    return ProxyFileInfo::viewOfTip(type);
}

qint64 Demo2FileInfo::size() const
{
    if (!proxy)
        return ProxyFileInfo::size();
    return proxy->size();
}

int Demo2FileInfo::countChildFile() const
{
    if (isAttributes(OptInfoType::kIsDir)) {
        QDir dir(pathOf(PathInfoType::kAbsoluteFilePath));
        QStringList entryList = dir.entryList(QDir::AllEntries | QDir::System
                                              | QDir::NoDotAndDotDot | QDir::Hidden);
        return entryList.size();
    }

    return -1;
}

QVariant Demo2FileInfo::extendAttributes(const ExtInfoType type) const
{
    switch (type) {
    case FileExtendedInfoType::kSizeFormat:
        if (!proxy)
            return ProxyFileInfo::extendAttributes(type);
        return proxy->extendAttributes(type);
    default:
        return ProxyFileInfo::extendAttributes(type);
    }
}

QString Demo2FileInfo::nameOf(const NameInfoType type) const
{
    switch (type) {
    case NameInfoType::kFileCopyName:
        return displayOf(DisPlayInfoType::kFileDisplayName);

    default:
        return ProxyFileInfo::nameOf(type);
    }
}

QString Demo2FileInfo::displayOf(const DisPlayInfoType type) const
{
    switch (type) {
    case DisPlayInfoType::kFileDisplayName:

        if (proxy)
            return proxy->displayOf(DisPlayInfoType::kFileDisplayName);
        return ProxyFileInfo::displayOf(type);

    default:
        return ProxyFileInfo::displayOf(type);
    }
}
