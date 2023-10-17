// SPDX-FileCopyrightText: 2023 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "plugin_demo1.h"
#include "utils/demo1manager.h"

#include <DApplication>
#include <QDebug>

static constexpr char kComputerPluginName[] { "dfmplugin_computer" };
static constexpr char kMenuPluginName[] { "dfmplugin_menu" };

using namespace dfmplugin_demo1;

DWIDGET_USE_NAMESPACE

void PluginDemo1::initialize()
{
    qDebug() << "===============init the demo1 plugin";
    loadTranslator();
    Demo1Manager::instance()->infoRegister();
    qDebug() << "===============done to init the demo1 plugin";
}

bool PluginDemo1::start()
{
    Demo1Manager::instance()->viewRegister();
    return true;
}

void PluginDemo1::loadTranslator()
{
    QString &&applicationName = qApp->applicationName();
    qApp->setApplicationName("your app prjoect name");
    qApp->loadTranslator();
    qApp->setApplicationName(applicationName);
}
