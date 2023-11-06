// SPDX-FileCopyrightText: 2023 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "plugin_diskencryptentry.h"
#include "menu/diskencryptmenuscene.h"
#include "gui/encryptprocessdialog.h"

#include <QApplication>
#include <QDebug>
#include <QDBusConnection>
#include <QTranslator>

static constexpr char kComputerPluginName[] { "dfmplugin_computer" };
static constexpr char kMenuPluginName[] { "dfmplugin_menu" };

using namespace dfmplugin_diskenc;

bool hasComputerMenuRegisted()
{
    return dpfSlotChannel->push(kMenuPluginName, "slot_MenuScene_Contains", "ComputerMenu").toBool();
}

void DiskEncryptEntry::initialize()
{
    auto i18n = new QTranslator(this);
    qDebug() << ">>>>>> load translations"
             << i18n->load(QLocale(), "disk-encrypt", "_", "/usr/share/dde-file-manager/translations");
    qDebug() << ">>>>>> install translations" << i18n->isEmpty()
             << QCoreApplication::installTranslator(i18n);
}

bool DiskEncryptEntry::start()
{
    dpfSlotChannel->push(kMenuPluginName, "slot_MenuScene_RegisterScene",
                         DiskEncryptMenuCreator::name(), new DiskEncryptMenuCreator);

    if (hasComputerMenuRegisted()) {
        dpfSlotChannel->push("dfmplugin_menu", "slot_MenuScene_Bind",
                             DiskEncryptMenuCreator::name(), kComputerPluginName);
    } else {
        dpfSignalDispatcher->subscribe(kMenuPluginName, "signal_MenuScene_SceneAdded",
                                       this, &DiskEncryptEntry::onComputerMenuSceneAdded);
    }

    connectDaemonSignals();

    return true;
}

void DiskEncryptEntry::connectDaemonSignals()
{
    QDBusConnection::systemBus().connect(kDaemonBusName,
                                         kDaemonBusPath,
                                         kDaemonBusIface,
                                         "EncryptDiskPrepareResult",
                                         this,
                                         SLOT(onPreencryptResult(const QString &, const QString &, int)));

    QDBusConnection::systemBus().connect(kDaemonBusName,
                                         kDaemonBusPath,
                                         kDaemonBusIface,
                                         "EncryptDiskResult",
                                         this,
                                         SLOT(onEncryptResult(const QString &, int)));

    QDBusConnection::systemBus().connect(kDaemonBusName,
                                         kDaemonBusPath,
                                         kDaemonBusIface,
                                         "EncryptProgress",
                                         this,
                                         SLOT(onEncryptProgress(const QString &, double)));

    QDBusConnection::systemBus().connect(kDaemonBusName,
                                         kDaemonBusPath,
                                         kDaemonBusIface,
                                         "DecryptDiskResult",
                                         this,
                                         SLOT(onDecryptResult(const QString &, const QString &, int)));

    QDBusConnection::systemBus().connect(kDaemonBusName,
                                         kDaemonBusPath,
                                         kDaemonBusIface,
                                         "DecryptProgress",
                                         this,
                                         SLOT(onDecryptProgress(const QString &, double)));
}

void DiskEncryptEntry::onPreencryptResult(const QString &dev, const QString &, int code)
{
    QApplication::restoreOverrideCursor();

    QString title = tr("Preencrypt done");
    QString msg = tr("Device %1 has been preencrypt, please reboot to finish encryption.").arg(dev);
    if (code != 0) {
        title = tr("Preencrypt failed");
        msg = tr("Device %1 preencrypt failed, please see log for more information.(%2)")
                      .arg(dev)
                      .arg(code);
    }

    DDialog *dlg = new DDialog;
    dlg->setAttribute(Qt::WA_DeleteOnClose);
    dlg->setTitle(title);
    dlg->setMessage(msg);
    dlg->addButton(tr("Confirm"));
    dlg->show();
}

void DiskEncryptEntry::onEncryptResult(const QString &dev, int code)
{
    if (encryptDialogs.contains(dev)) {
        delete encryptDialogs.value(dev);
        encryptDialogs.remove(dev);
    }

    QString title = tr("Encrypt done");
    QString msg = tr("Device %1 has been encrypted").arg(dev);
    if (code != 0) {
        title = tr("Encrypt failed");
        msg = tr("Device %1 encrypt failed, please see log for more information.(%2)")
                      .arg(dev)
                      .arg(code);
    }

    DDialog *dlg = new DDialog;
    dlg->setAttribute(Qt::WA_DeleteOnClose);
    dlg->setTitle(title);
    dlg->setMessage(msg);
    dlg->addButton(tr("Confirm"));
    dlg->show();
}

void DiskEncryptEntry::onEncryptProgress(const QString &dev, double progress)
{
    if (!encryptDialogs.contains(dev))
        encryptDialogs.insert(dev, new EncryptProcessDialog(tr("Encrypting...%1").arg(dev)));
    auto dlg = encryptDialogs.value(dev);
    dlg->updateProgress(progress);
    dlg->show();
}

void DiskEncryptEntry::onDecryptResult(const QString &dev, const QString &job, int code)
{
    if (decryptDialogs.contains(dev)) {
        decryptDialogs.value(dev)->deleteLater();
        decryptDialogs.remove(dev);
    }

    QString title = tr("Decrypt done");
    QString msg = tr("Device %1 has been decrypted").arg(dev);
    if (code != 0) {
        title = tr("Decrypt failed");
        msg = tr("Device %1 Decrypt failed, please see log for more information.(%2)")
                      .arg(dev)
                      .arg(code);
    }

    DDialog *dlg = new DDialog;
    dlg->setAttribute(Qt::WA_DeleteOnClose);
    dlg->setTitle(title);
    dlg->setMessage(msg);
    dlg->addButton(tr("Confirm"));
    dlg->show();
}

void DiskEncryptEntry::onDecryptProgress(const QString &dev, double progress)
{
    if (!decryptDialogs.contains(dev))
        decryptDialogs.insert(dev, new EncryptProcessDialog(tr("Decrypting...%1").arg(dev)));
    auto dlg = decryptDialogs.value(dev);
    dlg->updateProgress(progress);
    dlg->show();
}

void DiskEncryptEntry::onComputerMenuSceneAdded(const QString &scene)
{
    if (scene == "ComputerMenu") {
        dpfSlotChannel->push(kMenuPluginName, "slot_MenuScene_Bind", DiskEncryptMenuCreator::name(), "ComputerMenu");
        dpfSignalDispatcher->unsubscribe("dfmplugin_menu", "signal_MenuScene_SceneAdded", this, &DiskEncryptEntry::onComputerMenuSceneAdded);
    }
}