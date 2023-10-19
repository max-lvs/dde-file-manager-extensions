// SPDX-FileCopyrightText: 2022 - 2023 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "demo2fileiterator.h"
#include "demo2fileinfo.h"

#include <dfm-base/base/schemefactory.h>

#include <dfm-io/denumerator.h>
#include <dfm-io/dfmio_utils.h>

#include <QSharedPointer>

DFMBASE_USE_NAMESPACE
USING_IO_NAMESPACE

using namespace dfmplugin_demo2;

Demo2FileIterator::Demo2FileIterator(const QUrl &url, const QStringList &nameFilters, QDir::Filters filters, QDirIterator::IteratorFlags flags)
    : AbstractDirIterator(/*ToLocalUrl(url),need to change to your url*/ url, nameFilters, filters, flags)
{
    QUrl localUrl = url;//oLocalUrl(url);
    dfmioDirIterator.reset(new DFMIO::DEnumerator(localUrl, nameFilters,
                                                  static_cast<DEnumerator::DirFilter>(static_cast<int32_t>(filters)),
                                                  static_cast<DEnumerator::IteratorFlag>(static_cast<uint8_t>(flags))));
    if (!dfmioDirIterator)
        qCritical("Vault: create DEnumerator failed!");
}

Demo2FileIterator::~Demo2FileIterator()
{
}

QUrl Demo2FileIterator::next()
{
    if (dfmioDirIterator)
        currentUrl = dfmioDirIterator->next().path();

    return currentUrl;
}

bool Demo2FileIterator::hasNext() const
{
    return (dfmioDirIterator ? dfmioDirIterator->hasNext() : false);
}

QString Demo2FileIterator::fileName() const
{
    return fileUrl().fileName();
}

QUrl Demo2FileIterator::fileUrl() const
{
    return currentUrl;
}

const FileInfoPointer Demo2FileIterator::fileInfo() const
{
    assert(QThread::currentThread() != qApp->thread());

    QUrl url = fileUrl(); //ToLocalUrl(fileUrl());
    FileInfoPointer fixedInfo(new Demo2FileInfo(url));
    return fixedInfo;
}

bool Demo2FileIterator::initIterator()
{
    if (dfmioDirIterator)
        return dfmioDirIterator->initEnumerator(oneByOne());
    return false;
}

QUrl Demo2FileIterator::url() const
{
    return QUrl("demo2"); // root url
}
