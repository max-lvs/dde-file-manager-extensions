// SPDX-FileCopyrightText: 2023 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef PluginDemo1_H
#define PluginDemo1_H

#include "dfmplugin_demo1_global.h"

#include <dfm-framework/dpf.h>

namespace dfmplugin_demo1 {

class DFMPLUGIN_DEMO1_EXPORT PluginDemo1 : public dpf::Plugin
{
    Q_OBJECT

    Q_PLUGIN_METADATA(IID "org.deepin.plugin.filemanager" FILE "filemanager-plugin-demo1.json")

    // Plugin interface
public:
    virtual void initialize() override;
    virtual bool start() override;

private:
    void loadTranslator();
};
}

#endif   // PluginDemo1_H
