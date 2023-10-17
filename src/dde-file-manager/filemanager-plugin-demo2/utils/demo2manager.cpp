// SPDX-FileCopyrightText: 2022 - 2023 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "demo2manager.h"
#include "menu/demo2menuscene.h"
#include "view/Demo2View.h"

#include <dfm-framework/event/event.h>

#include "dfm-base/base/schemefactory.h"
#include "dfm-base/widgets/filemanagerwindowsmanager.h"
#include "dfm-base/dfm_global_defines.h"
#include "dfm-base/dfm_event_defines.h"

#include <QMenu>

DFMBASE_USE_NAMESPACE

using namespace dfmplugin_demo2;
using ContextMenuCallback = std::function<void(quint64 windowId, const QUrl &url, const QPoint &globalPos)>;
Q_DECLARE_METATYPE(ContextMenuCallback);
Q_DECLARE_METATYPE(QList<QVariantMap> *)

Demo2Manager *Demo2Manager::instance()
{
    static Demo2Manager instance;
    return &instance;
}

void Demo2Manager::infoRegister()
{
    UrlRoute::regScheme(Demo2Manager::scheme(), "/", Demo2Manager::icon(), true, tr("Plugin-demo2"));
    bindWindows();
}

void Demo2Manager::viewRegister()
{
    ViewFactory::regClass<Demo2View>(Demo2Manager::scheme());
    connect(&FMWindowsIns, &FileManagerWindowsManager::windowOpened, this, &Demo2Manager::onWindowOpened, Qt::DirectConnection);
}

void Demo2Manager::bindWindows()
{
    const auto &winIdList { FMWindowsIns.windowIdList() };
    std::for_each(winIdList.begin(), winIdList.end(), [this](quint64 id) {
        onWindowOpened(id);
    });
    connect(&FMWindowsIns, &FileManagerWindowsManager::windowOpened, this, &Demo2Manager::onWindowOpened, Qt::DirectConnection);
}

void Demo2Manager::onWindowOpened(quint64 winID)
{
    auto window = FMWindowsIns.findWindowById(winID);

    if (window->titleBar())
        titleBarRegister();
    else
        connect(window, &FileManagerWindow::titleBarInstallFinished, this, &Demo2Manager::titleBarRegister, Qt::DirectConnection);

    if (window->sideBar())
        sideBarRegister();
    else
        connect(window, &FileManagerWindow::sideBarInstallFinished, this, &Demo2Manager::sideBarRegister, Qt::DirectConnection);
}

void Demo2Manager::sideBarRegister()
{
    ContextMenuCallback contextMenuCb { Demo2Manager::sidebarContenxtMenuHandle };
    Qt::ItemFlags flags { Qt::ItemIsEnabled | Qt::ItemIsSelectable };
    QVariantMap map {
        { "Property_Key_Group", "Group_Network" },
        { "Property_Key_DisplayName", tr("Plugin demo1") },
        { "Property_Key_Icon", Demo2Manager::icon() },
        { "Property_Key_QtItemFlags", QVariant::fromValue(flags) },
        { "Property_Key_CallbackContextMenu", QVariant::fromValue(contextMenuCb) }
    };

    dpfSlotChannel->push("dfmplugin_sidebar", "slot_Item_Insert", 1, Demo2Manager::rootUrl(), map);
}

void Demo2Manager::titleBarRegister()
{
    QVariantMap property;
    property["Property_Key_HideIconViewBtn"] = true;
    property["Property_Key_HideListViewBtn"] = true;
    property["Property_Key_HideDetailSpaceBtn"] = true;
    dpfSlotChannel->push("dfmplugin_titlebar", "slot_Custom_Register", Demo2Manager::scheme(), property);
    dpfHookSequence->follow("dfmplugin_titlebar", "hook_Crumb_Seprate", Demo2Manager::instance(), &Demo2Manager::handleSepateTitlebarCrumb);
}

static constexpr char kBindMenuName[] { "FileOperatorMenu" };
static constexpr char kMenuPluginName[] { "dfmplugin_menu" };
static constexpr char kMenuSceneName[] { "demo1_sence_menu" };

bool hasSceneMenuRegisted()
{
    return dpfSlotChannel->push(kMenuPluginName, "slot_MenuScene_Contains", kMenuSceneName).toBool();
}

bool Demo2Manager::menuRegister()
{
    qDebug() << "===============Demo1Manager::menuRegister";
    dpfSlotChannel->push(kMenuPluginName, "slot_MenuScene_RegisterScene", Demo2MenuCreator::name(), new Demo2MenuCreator);

    dpfSlotChannel->push("dfmplugin_workspace", "slot_RegisterFileView", scheme());
    dpfSlotChannel->push("dfmplugin_workspace", "slot_RegisterMenuScene", scheme(), Demo2MenuCreator::name());


//    if (hasSceneMenuRegisted()) {
//        dpfSlotChannel->push(kMenuPluginName, "slot_MenuScene_Bind", Demo1MenuCreator::name(), kBindMenuName);
//    } else {
//        dpfSignalDispatcher->subscribe(kMenuPluginName, "signal_MenuScene_SceneAdded", this, &Demo1Manager::onSceneMenuSceneAdded);
//    }

    return true;
}

void Demo2Manager::onSceneMenuSceneAdded(const QString &scene)
{
    if (scene == kMenuSceneName) {
        dpfSlotChannel->push(kMenuPluginName, "slot_MenuScene_Bind", Demo2MenuCreator::name(), kMenuSceneName);
        dpfSignalDispatcher->unsubscribe(kMenuPluginName, "signal_MenuScene_SceneAdded", this, &Demo2Manager::onSceneMenuSceneAdded);
    }
}


bool Demo2Manager::handleSepateTitlebarCrumb(const QUrl &url, QList<QVariantMap> *mapGroup)
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

void Demo2Manager::sidebarContenxtMenuHandle(quint64 windowId, const QUrl &url, const QPoint &globalPos)
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

Demo2Manager::Demo2Manager(QObject *parent)
    : QObject(parent)
{
}

Demo2Manager::~Demo2Manager()
{
}
