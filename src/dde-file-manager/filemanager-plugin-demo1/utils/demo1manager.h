// SPDX-FileCopyrightText: 2022 - 2023 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef DEMO1MANAGER_H
#define DEMO1MANAGER_H
#include "dfmplugin_demo1_global.h"

#include <QObject>
#include <QIcon>
#include <QUrl>

namespace dfmplugin_demo1 {
class Demo1Manager : public QObject
{
    Q_OBJECT

public:
    inline static QString scheme()
    {
        return "plugindemo1";
    }
    inline static QIcon icon()
    {
        return QIcon::fromTheme("iconplugindemo1");
    }
    inline static QUrl rootUrl()
    {
        QUrl u;
        u.setScheme(scheme());
        u.setPath("/");
        return u;
    }
    static Demo1Manager *instance();

    void infoRegister();

    void viewRegister();

    static void sidebarContenxtMenuHandle(quint64 windowId, const QUrl &url, const QPoint &globalPos);

    void bindWindows();

public slots:
    void onWindowOpened(quint64 winID);

    void sideBarRegister();

    void titleBarRegister();

    bool handleSepateTitlebarCrumb(const QUrl &url, QList<QVariantMap> *mapGroup);

private:
    void onSceneMenuSceneAdded(const QString &scene);

private:
    explicit Demo1Manager(QObject *parent = nullptr);
    ~Demo1Manager() override;
};
}
#endif
