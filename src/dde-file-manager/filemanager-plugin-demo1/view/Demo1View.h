// SPDX-FileCopyrightText: 2022 - 2023 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef DEMO1VIEW_H
#define DEMO1VIEW_H


#include <QWidget>
#include <QMap>

#include "dfm-base/interfaces/abstractbaseview.h"

#define DUrl QUrl

DFMBASE_USE_NAMESPACE

class Demo1View : public QWidget, public AbstractBaseView
{
    Q_OBJECT

public:
    Demo1View();
    Demo1View(const QUrl &url, QWidget *parent = nullptr);
    virtual ~Demo1View() override;

    virtual QWidget *widget() const override;
    virtual DUrl rootUrl() const override;
    virtual ViewState viewState() const override;
    virtual bool setRootUrl(const DUrl &url) override;
    virtual QList<QAction *> toolBarActionList() const override;
    virtual void refresh() override;

private:
    void initUi();
    void initConnect();

private Q_SLOTS:
    void onMenuRequest(const QPoint &pos);

private:
    //view的url，记录每个view的url
    DUrl m_durl;
    //返回的view界面
    QWidget *m_widget = nullptr;
};

#endif   // DEMO1VIEW_H
