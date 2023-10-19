// SPDX-FileCopyrightText: 2022 - 2023 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef DEMO2FILEINFO_H
#define DEMO2FILEINFO_H

#include "dfmplugin_demo2_global.h"
#include <dfm-base/interfaces/proxyfileinfo.h>

namespace dfmplugin_demo2 {
class VaultFileInfoPrivate;
class Demo2FileInfo : public DFMBASE_NAMESPACE::ProxyFileInfo
{
    friend class VaultFileInfoPrivate;
    VaultFileInfoPrivate *d;

public:
    explicit Demo2FileInfo(const QUrl &url);
    explicit Demo2FileInfo(const QUrl &url, const FileInfoPointer &proxy);
    virtual ~Demo2FileInfo() override;

    virtual Demo2FileInfo &operator=(const Demo2FileInfo &fileinfo);
    virtual bool operator==(const Demo2FileInfo &fileinfo) const;
    virtual bool operator!=(const Demo2FileInfo &fileinfo) const;

    virtual QString pathOf(const FilePathInfoType type) const override;
    virtual QUrl urlOf(const FileUrlInfoType type) const override;
    virtual bool exists() const override;
    virtual void refresh() override;
    virtual bool isAttributes(const FileIsType type) const override;
    virtual bool canAttributes(const FileCanType type) const override;

    virtual qint64 size() const override;
    virtual int countChildFile() const override;
    virtual QVariant extendAttributes(const FileExtendedInfoType type) const override;
    virtual QVariantHash extraProperties() const override;
    virtual QUrl getUrlByType(const FileUrlInfoType type, const QString &fileName) const override;

    virtual QIcon fileIcon() override;

    virtual QString nameOf(const FileNameInfoType type) const override;

    virtual QString displayOf(const DisplayInfoType type) const override;
    virtual QString viewOfTip(const ViewType type) const override;
};
}
#endif   //! DEMO2FILEINFO_H
