// SPDX-FileCopyrightText: 2023 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef PluginDemo2_H
#define PluginDemo2_H

#include "dfmplugin_demo2_global.h"

#include <dfm-framework/dpf.h>

namespace dfmplugin_demo2 {

class DFMPLUGIN_DEMO2_EXPORT PluginDemo2 : public dpf::Plugin
{
    Q_OBJECT

    Q_PLUGIN_METADATA(IID "org.deepin.plugin.filemanager" FILE "filemanager-plugin-demo2.json")

    // Plugin interface
public:
    virtual void initialize() override;
    virtual bool start() override;

private:
    void loadTranslator();
};
}

#endif   // PluginDemo2_H
