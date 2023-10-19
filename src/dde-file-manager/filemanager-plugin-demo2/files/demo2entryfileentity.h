// SPDX-FileCopyrightText: 2022 - 2023 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef DEMO2ENTRYFILEENTITY_H
#define DEMO2ENTRYFILEENTITY_H

#include "dfmplugin_demo2_global.h"

#include <dfm-base/interfaces/abstractentryfileentity.h>

#include <QObject>

namespace dfmplugin_demo2 {
class Demo2EntryFileEntity : public DFMBASE_NAMESPACE::AbstractEntryFileEntity
{
    Q_OBJECT
public:
    explicit Demo2EntryFileEntity(const QUrl &url);
    virtual ~Demo2EntryFileEntity() override;

    virtual QString displayName() const override;
    virtual QIcon icon() const override;
    virtual bool exists() const override;

    virtual bool showProgress() const override;
    virtual bool showTotalSize() const override;
    virtual bool showUsageSize() const override;
    virtual int order() const override;

    virtual void refresh() override;
    virtual quint64 sizeTotal() const override;
    virtual QUrl targetUrl() const override;

public slots:
    void slotFileDirSizeChange(qint64 size, int filesCount, int directoryCount);

    void slotFinishedThread();

private:
    qint64 vaultTotal { 0 };
    qint64 totalchange { 0 };

    mutable bool showSizeState { false };
};
}
#endif   // DEMO2ENTRYFILEENTITY_H
