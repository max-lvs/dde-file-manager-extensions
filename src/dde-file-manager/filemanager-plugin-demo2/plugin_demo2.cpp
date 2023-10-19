// SPDX-FileCopyrightText: 2023 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "plugin_demo2.h"
#include "utils/demo2manager.h"
#include "menu/demo2menuscene.h"

#include <DApplication>
#include <QDebug>

static constexpr char kComputerPluginName[] { "dfmplugin_computer" };
static constexpr char kMenuPluginName[] { "dfmplugin_menu" };

using namespace dfmplugin_demo2;

DWIDGET_USE_NAMESPACE

void PluginDemo2::initialize()
{
    qDebug() << "===============init the demo2 plugin";
    loadTranslator();
    Demo2Manager::instance()->infoRegister();
    qDebug() << "===============done to init the demo2 plugin";
}

bool PluginDemo2::start()
{
    Demo2Manager::instance()->initConection();
    return true;
}

void PluginDemo2::loadTranslator()
{
    QString &&applicationName = qApp->applicationName();
    qApp->setApplicationName("your app prjoect name");
    qApp->loadTranslator();
    qApp->setApplicationName(applicationName);
}
