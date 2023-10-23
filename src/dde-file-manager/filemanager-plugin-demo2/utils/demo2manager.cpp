// SPDX-FileCopyrightText: 2022 - 2023 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "demo2manager.h"
#include "menu/demo2menuscene.h"
#include "files/demo2entryfileentity.h"
#include "files/demo2fileinfo.h"
#include "files/demo2fileiterator.h"
#include "files/demo2filewatcher.h"

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

static constexpr char kMenuSceneName[] { "demo2_sence_menu" };

void Demo2Manager::infoRegister()
{
    // 注册本插件的scheme
    UrlRoute::regScheme(Demo2Manager::scheme(), "/", Demo2Manager::icon(), true, tr("Plugin-demo2"));

    //注册本插件的fileinfo信息。特殊业务，需要自行编写files里面的Demo2FileInfo、Demo2FileWatcher、Demo2FileIterator，并进行注册。demo2只演示了适配文管file的业务，file相关的fileinfo已经适配，因此不需要自己写
//    InfoFactory::regClass<Demo2FileInfo>(Demo2Manager::scheme());
//    WatcherFactory::regClass<Demo2FileWatcher>(Demo2Manager::scheme(), WatcherFactory::kNoCache);
//    DirIteratorFactory::regClass<Demo2FileIterator>(Demo2Manager::scheme());

    // 订阅计算机页面事件
    EntryEntityFactor::registCreator<Demo2EntryFileEntity>("demo2"); //  订阅计算机业务的entryfile，demo2 是suffix，用于标识当前Demo2EntryFileEntity

    dpfSignalDispatcher->subscribe("dfmplugin_computer", "signal_Operation_OpenItem", this, &Demo2Manager::computerOpenItem);

    dpfSignalDispatcher->subscribe("dfmplugin_computer", "signal_View_Refreshed",
                                   this, &Demo2Manager::addComputer);

    // bind workspace 窗口信息
    bindWindows();
}

void Demo2Manager::bindWindows()
{
    const auto &winIdList { FMWindowsIns.windowIdList() };
    std::for_each(winIdList.begin(), winIdList.end(), [this](quint64 id) {
        onWindowOpened(id);
    });
    connect(&FMWindowsIns, &FileManagerWindowsManager::windowOpened, this, &Demo2Manager::onWindowOpened, Qt::DirectConnection);
}


void Demo2Manager::initConection()
{
    // 发布工作区本插件scheme、鼠标右键场景信息,方便工作区文件信息、鼠标右键识别
    dpfSlotChannel->push("dfmplugin_workspace", "slot_RegisterFileView", Demo2Manager::scheme());
    dpfSlotChannel->push("dfmplugin_workspace", "slot_RegisterMenuScene", scheme(), Demo2MenuCreator::name());


    // 发布向右键注入本插件鼠标信息
    dpfSlotChannel->push("dfmplugin_menu", "slot_MenuScene_RegisterScene", Demo2MenuCreator::name(), new Demo2MenuCreator);
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

    if (window->workSpace())
        addComputer();
    else
        connect(window, &FileManagerWindow::workspaceInstallFinished, this, &Demo2Manager::addComputer, Qt::DirectConnection);
}

void Demo2Manager::sideBarRegister()
{
    ContextMenuCallback contextMenuCb { Demo2Manager::sidebarContenxtMenuHandle };
    Qt::ItemFlags flags { Qt::ItemIsEnabled | Qt::ItemIsSelectable };
    QVariantMap map {
        { "Property_Key_Group", "Group_Network" },
        { "Property_Key_DisplayName", tr("Plugin demo2") },
        { "Property_Key_Icon", Demo2Manager::icon() },
        { "Property_Key_QtItemFlags", QVariant::fromValue(flags) },
        { "Property_Key_CallbackContextMenu", QVariant::fromValue(contextMenuCb) }
    };

    dpfSlotChannel->push("dfmplugin_sidebar", "slot_Item_Insert", 1, Demo2Manager::rootUrl(), map);
}

void Demo2Manager::addComputer()
{
    // entry:/// 计算机界面标识，demodev:设备名,“虚拟设备名，不能为空，任意的”,demo2: suffix,用于标识自定义 Demo2EntryFileEntity, item标识
     dpfSlotChannel->push("dfmplugin_computer", "slot_Item_Add", tr("Your dev label"), QUrl("entry:///demodev.demo2"), 0, false);
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


void Demo2Manager::computerOpenItem(quint64 winId, const QUrl &url)
{
    if (url.path().contains(scheme())) {
        // your business and open the window
        dpfSignalDispatcher->publish(GlobalEventType::kChangeCurrentUrl, winId, url);
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
