// SPDX-FileCopyrightText: 2022 - 2023 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef DEMO2FILEWATCHER_H
#define DEMO2FILEWATCHER_H

#include "dfmplugin_demo2_global.h"
#include <dfm-base/interfaces/abstractfilewatcher.h>

namespace dfmplugin_demo2 {
class Demo2FileWatcher : public DFMBASE_NAMESPACE::AbstractFileWatcher
{
    Q_OBJECT
public:
    explicit Demo2FileWatcher() = delete;
    explicit Demo2FileWatcher(const QUrl &url, QObject *parent = nullptr);
    virtual ~Demo2FileWatcher() override;

private slots:
    void onFileDeleted(const QUrl &url);
    void onFileAttributeChanged(const QUrl &url);
    void onFileRename(const QUrl &fromUrl, const QUrl &toUrl);
    void onSubfileCreated(const QUrl &url);

};
}
#endif   // DEMO2FILEWATCHER_H
