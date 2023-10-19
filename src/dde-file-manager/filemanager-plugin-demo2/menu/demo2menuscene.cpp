// SPDX-FileCopyrightText: 2023 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "demo2menuscene.h"

#include <dfm-base/dfm_menu_defines.h>
#include <dfm-base/base/schemefactory.h>
#include <dfm-base/interfaces/fileinfo.h>
#include <dfm-framework/event/event.h>

#include "dfm-base/base/schemefactory.h"
#include "dfm-base/dfm_event_defines.h"

#include <DDialog>
#include <QDebug>
#include <QMenu>
#include <QProcess>
#include <QFile>
#include <QStringList>

DFMBASE_USE_NAMESPACE
DWIDGET_USE_NAMESPACE

using namespace dfmplugin_demo2;

void showPopDialog()
{
    DDialog dlg;
    dlg.setTitle("提示");
    dlg.setMessage("demo2 菜单展示，请自行添加业务");
    dlg.addButton("确认");
    dlg.exec();
}

Demo2MenuScene::Demo2MenuScene(QObject *parent)
    : AbstractMenuScene(parent)
{
}

dfmbase::AbstractMenuScene *Demo2MenuCreator::create()
{
    return new Demo2MenuScene();
}

QString Demo2MenuScene::name() const
{
    return Demo2MenuCreator::name();
}

bool Demo2MenuScene::initialize(const QVariantHash &params)
{
    qDebug() << "===============Demo2MenuScene::initialize:" << params;

    QList<QUrl> selectedItems = params.value(MenuParamKey::kSelectFiles).value<QList<QUrl>>();

    QList<AbstractMenuScene *> currentScene;

    const QVariant &scene = dpfSlotChannel->push("dfmplugin_menu", "slot_MenuScene_CreateScene", QString("WorkspaceMenu"));
    auto fileWorkScene = scene.value<DFMBASE_NAMESPACE::AbstractMenuScene *>();

    qDebug() << "========" << scene.isValid() << scene << fileWorkScene;

    if (fileWorkScene) {
        qDebug() << "===============fileWorkScene::initialize:" << params;
        currentScene.append(fileWorkScene);
    }

    // the scene added by binding must be initializeed after 'defalut scene'.
    currentScene.append(subScene);
    setSubscene(currentScene);

    // 初始化所有子场景
    bool ret = AbstractMenuScene::initialize(params);

    return true;
}

bool Demo2MenuScene::create(QMenu *parent)
{
    qDebug() << "===============Demo2MenuScene::create" ;
    Q_ASSERT(parent);

    // 创建子场景菜单
    AbstractMenuScene::create(parent);

    actEncrypt = parent->addAction("plugin-demo2-menu");
    return true;
}

bool Demo2MenuScene::triggered(QAction *action)
{
    qDebug() << "===============Demo2MenuScene::triggered" << action->text() ;

    if (action == actEncrypt) {
        showPopDialog();
        return true;
    }
    return AbstractMenuScene::triggered(action);
}

void Demo2MenuScene::updateState(QMenu *parent)
{
    qDebug() << "===============Demo2MenuScene::updateState" << parent->title() ;
    Q_ASSERT(parent);

    AbstractMenuScene::updateState(parent);

    QList<QAction *> acts = parent->actions();
    for (auto act : acts) {
        if (act == actEncrypt) {
            actEncrypt->setVisible(true);
            break;
        }
    }
}
