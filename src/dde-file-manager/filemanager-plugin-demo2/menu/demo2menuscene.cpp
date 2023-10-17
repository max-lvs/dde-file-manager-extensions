// SPDX-FileCopyrightText: 2023 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "demo2menuscene.h"

#include <dfm-base/dfm_menu_defines.h>
#include <dfm-base/base/schemefactory.h>
#include <dfm-base/interfaces/fileinfo.h>

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
    return true;

    QList<QUrl> selectedItems = params.value(MenuParamKey::kSelectFiles).value<QList<QUrl>>();
    if (selectedItems.isEmpty())
        return false;

    selectedItem = selectedItems.first();
    if (!selectedItem.path().endsWith("your path or other thing"))
        return false;

    QSharedPointer<FileInfo> info = InfoFactory::create<FileInfo>(selectedItem);
    if (!info)
        return false;

    return true;
}

bool Demo2MenuScene::create(QMenu *parent)
{
    qDebug() << "===============Demo2MenuScene::create" ;
    Q_ASSERT(parent);
    actEncrypt = parent->addAction("plugin-demo1-menu");
    return true;
}

bool Demo2MenuScene::triggered(QAction *action)
{
    qDebug() << "===============Demo2MenuScene::create" << action->text() ;
    if (action == actEncrypt) {
        showPopDialog();
        return true;
    }
    return false;
}

void Demo2MenuScene::updateState(QMenu *parent)
{
    qDebug() << "===============Demo2MenuScene::create" << parent->title() ;
    Q_ASSERT(parent);
    QList<QAction *> acts = parent->actions();
    for (auto act : acts) {
        if (act == actEncrypt) {
            actEncrypt->setVisible(true);
            break;
        }
    }
}
