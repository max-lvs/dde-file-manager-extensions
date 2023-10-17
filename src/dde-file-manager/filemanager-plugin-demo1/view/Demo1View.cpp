// SPDX-FileCopyrightText: 2022 - 2023 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "Demo1View.h"

#include <QPushButton>
#include <QDebug>
#include <DMenu>
#include <QContextMenuEvent>

DWIDGET_USE_NAMESPACE

Demo1View::Demo1View()
    : QWidget(), AbstractBaseView ()
{
    initUi();
}

Demo1View::Demo1View(const QUrl &url, QWidget *parent)
    : QWidget(parent), AbstractBaseView ()
{
    Q_UNUSED(url);

    initUi();
    initConnect();
}

Demo1View::~Demo1View()
{
}

QWidget *Demo1View::widget() const
{
    qWarning() << "Demo1View::widget()Demo1View::widget()";
    return m_widget;
}

DUrl Demo1View::rootUrl() const
{
    return m_durl;
}

AbstractBaseView::ViewState Demo1View::viewState() const
{
    return AbstractBaseView::ViewState::kViewIdle;
}

bool Demo1View::setRootUrl(const DUrl &url)
{
    Q_UNUSED(url);
    bool ret = false;
    ret = (url.scheme() == "plugindemo1");
    if (ret && m_durl != url) {
        m_durl = url;
        qDebug() << "===============setRootUrl："<<url;
        //handleYourBussiness(url);
    }
    return ret;
}

QList<QAction *> Demo1View::toolBarActionList() const
{
    return QList<QAction *>();
}

void Demo1View::refresh()
{
}


void Demo1View::initUi()
{
    m_widget = new QWidget(this);
    m_widget->setContextMenuPolicy(Qt::CustomContextMenu);

    QPushButton *button = new QPushButton(m_widget);
    button->setText(QObject::tr("new view", "button"));
    button->setFixedWidth(82);
    button->setEnabled(true);
    button->move(100,100);
    QObject::connect(button, &QPushButton::clicked, [=]() {
       button->setText("....") ;
    });
}

void Demo1View::initConnect()
{
    if(m_widget){
        connect(m_widget, &QWidget::customContextMenuRequested, this, &Demo1View::onMenuRequest);
    }
}

void Demo1View::onMenuRequest(const QPoint &pos)
{
    DMenu* menu = new DMenu();
    menu->addAction(QObject::tr("your self view menu"));

    auto act = menu->exec(QCursor::pos());

    delete menu;
}
