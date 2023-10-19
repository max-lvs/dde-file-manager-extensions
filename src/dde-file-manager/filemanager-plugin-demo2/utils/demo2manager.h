// SPDX-FileCopyrightText: 2022 - 2023 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef DEMO2MANAGER_H
#define DEMO2MANAGER_H
#include "dfmplugin_demo2_global.h"

#include <QObject>
#include <QIcon>
#include <QUrl>

namespace dfmplugin_demo2 {
class Demo2Manager : public QObject
{
    Q_OBJECT

public:
    inline static QString scheme()
    {
        return "plugindemo2";
    }
    inline static QIcon icon()
    {
        return QIcon::fromTheme("iconplugindemo2");
    }
    inline static QUrl rootUrl()
    {
        QUrl u;
        u.setScheme(scheme());
        u.setPath("/");
        return u;
    }
    static Demo2Manager *instance();

    void infoRegister();

    void viewRegister();

    void initConection();

    static void sidebarContenxtMenuHandle(quint64 windowId, const QUrl &url, const QPoint &globalPos);

    void bindWindows();

public slots:
    void onWindowOpened(quint64 winID);

    void sideBarRegister();

    void titleBarRegister();

    void addComputer();

    void computerOpenItem(quint64 winId, const QUrl &url);

    bool handleSepateTitlebarCrumb(const QUrl &url, QList<QVariantMap> *mapGroup);

private:
    explicit Demo2Manager(QObject *parent = nullptr);
    ~Demo2Manager() override;
};
}
#endif
