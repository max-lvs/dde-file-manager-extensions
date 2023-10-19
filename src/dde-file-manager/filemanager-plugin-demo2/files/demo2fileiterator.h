// SPDX-FileCopyrightText: 2022 - 2023 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef DEMO2FILEDIRITERATOR_H
#define DEMO2FILEDIRITERATOR_H

#include "dfmplugin_demo2_global.h"
#include <dfm-base/interfaces/abstractdiriterator.h>
#include <dfm-io/denumerator.h>

namespace dfmplugin_demo2 {
class Demo2FileIterator : public DFMBASE_NAMESPACE::AbstractDirIterator
{
    Q_OBJECT
    friend class VaultFileIteratorPrivate;

public:
    explicit Demo2FileIterator(const QUrl &url,
                               const QStringList &nameFilters = QStringList(),
                               QDir::Filters filters = QDir::NoFilter,
                               QDirIterator::IteratorFlags flags = QDirIterator::NoIteratorFlags);

    virtual ~Demo2FileIterator() override;

    virtual QUrl next() override;

    virtual bool hasNext() const override;

    virtual QString fileName() const override;

    virtual QUrl fileUrl() const override;

    virtual const FileInfoPointer fileInfo() const override;

    virtual QUrl url() const override;

    bool initIterator() override;

private:
    QSharedPointer<dfmio::DEnumerator> dfmioDirIterator { Q_NULLPTR };
    QUrl currentUrl;
};
}
#endif   // DEMO2FILEDIRITERATOR_H
