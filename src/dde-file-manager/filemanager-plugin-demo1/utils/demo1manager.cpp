// SPDX-FileCopyrightText: 2022 - 2023 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "demo1manager.h"

#include <dfm-framework/event/event.h>

#include "dfm-base/base/schemefactory.h"
#include "dfm-base/widgets/filemanagerwindowsmanager.h"
#include "dfm-base/dfm_global_defines.h"
#include "dfm-base/dfm_event_defines.h"

#include "view/Demo1View.h"

#include <QMenu>

DFMBASE_USE_NAMESPACE

using namespace dfmplugin_demo1;
using ContextMenuCallback = std::function<void(quint64 windowId, const QUrl &url, const QPoint &globalPos)>;
Q_DECLARE_METATYPE(ContextMenuCallback);
Q_DECLARE_METATYPE(QList<QVariantMap> *)

Demo1Manager *Demo1Manager::instance()
{
    static Demo1Manager instance;
    return &instance;
}

void Demo1Manager::infoRegister()
{
    UrlRoute::regScheme(Demo1Manager::scheme(), "/", Demo1Manager::icon(), true, tr("Plugin-demo1"));
    bindWindows();
}

void Demo1Manager::viewRegister()
{
    ViewFactory::regClass<Demo1View>(Demo1Manager::scheme());
    connect(&FMWindowsIns, &FileManagerWindowsManager::windowOpened, this, &Demo1Manager::onWindowOpened, Qt::DirectConnection);
}

void Demo1Manager::bindWindows()
{
    const auto &winIdList { FMWindowsIns.windowIdList() };
    std::for_each(winIdList.begin(), winIdList.end(), [this](quint64 id) {
        onWindowOpened(id);
    });
    connect(&FMWindowsIns, &FileManagerWindowsManager::windowOpened, this, &Demo1Manager::onWindowOpened, Qt::DirectConnection);
}

void Demo1Manager::onWindowOpened(quint64 winID)
{
    auto window = FMWindowsIns.findWindowById(winID);

    if (window->titleBar())
        titleBarRegister();
    else
        connect(window, &FileManagerWindow::titleBarInstallFinished, this, &Demo1Manager::titleBarRegister, Qt::DirectConnection);

    if (window->sideBar())
        sideBarRegister();
    else
        connect(window, &FileManagerWindow::sideBarInstallFinished, this, &Demo1Manager::sideBarRegister, Qt::DirectConnection);
}

void Demo1Manager::sideBarRegister()
{
    ContextMenuCallback contextMenuCb { Demo1Manager::sidebarContenxtMenuHandle };
    Qt::ItemFlags flags { Qt::ItemIsEnabled | Qt::ItemIsSelectable };
    QVariantMap map {
        { "Property_Key_Group", "Group_Network" },
        { "Property_Key_DisplayName", tr("Plugin demo1") },
        { "Property_Key_Icon", Demo1Manager::icon() },
        { "Property_Key_QtItemFlags", QVariant::fromValue(flags) },
        { "Property_Key_CallbackContextMenu", QVariant::fromValue(contextMenuCb) }
    };

    dpfSlotChannel->push("dfmplugin_sidebar", "slot_Item_Insert", 1, Demo1Manager::rootUrl(), map);
}

void Demo1Manager::titleBarRegister()
{
    QVariantMap property;
    property["Property_Key_HideIconViewBtn"] = true;
    property["Property_Key_HideListViewBtn"] = true;
    property["Property_Key_HideDetailSpaceBtn"] = true;
    dpfSlotChannel->push("dfmplugin_titlebar", "slot_Custom_Register", Demo1Manager::scheme(), property);
    dpfHookSequence->follow("dfmplugin_titlebar", "hook_Crumb_Seprate", Demo1Manager::instance(), &Demo1Manager::handleSepateTitlebarCrumb);
}


bool Demo1Manager::handleSepateTitlebarCrumb(const QUrl &url, QList<QVariantMap> *mapGroup)
{
    if (url.scheme() == scheme()) {
        QList<QUrl> urls;
        urls.push_back(url);
        UrlRoute::urlParentList(url, &urls);

        for (int count = urls.size() - 1; count >= 0; count--) {
            QVariantMap map;
            QUrl curUrl { urls.at(count) };
            QStringList pathList { curUrl.path().split("/") };

            curUrl.setHost("phone");
            map["CrumbData_Key_Url"] = curUrl;
            map["CrumbData_Key_DisplayText"] = pathList.isEmpty() ? "" : pathList.last();
            if (curUrl == rootUrl())
                map["CrumbData_Key_IconName"] = UrlRoute::icon(scheme()).name();
            mapGroup->push_back(map);
        }
        return true;
    }
    return false;
}

void Demo1Manager::sidebarContenxtMenuHandle(quint64 windowId, const QUrl &url, const QPoint &globalPos)
{
    QFileInfo info(url.path());
    bool bEnabled = info.exists();

    QMenu *menu = new QMenu;

    auto newWindowAct = menu->addAction(QObject::tr("Open in new window"), [url]() {
        dpfSignalDispatcher->publish(GlobalEventType::kOpenNewWindow, url);
    });
    newWindowAct->setEnabled(bEnabled);

    auto newTabAct = menu->addAction(QObject::tr("Open in new tab"), [windowId, url]() {
        dpfSignalDispatcher->publish(GlobalEventType::kOpenNewTab, windowId, url);
    });

    newTabAct->setEnabled(bEnabled);

    menu->exec(globalPos);

    delete menu;
}

Demo1Manager::Demo1Manager(QObject *parent)
    : QObject(parent)
{
}

Demo1Manager::~Demo1Manager()
{
}
