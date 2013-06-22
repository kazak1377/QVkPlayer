#include "vkauth.h"
#include "qdebug.h"
#include "mainwindow.h"
#include "QtGui"
#include <QFile>
#include <QTextStream>
#include <QtXml>

vkAuth::vkAuth(QWidget *parent) : QWebView(parent)
{
    connect(this, SIGNAL(urlChanged(QUrl)), SLOT(slotUrlChanged(QUrl)));
    QUrl url("https://oauth.vk.com/authorize?");
    url.addQueryItem("client_id", "3652252");  //HERE second param is my app_id, which get from vk.com
    url.addQueryItem("display", "popup");
    url.addQueryItem("type", "browser");
    url.addQueryItem("scope", "audio,offline");
    url.addQueryItem("redirect_uri", "https://oauth.vk.com/blank.html");
    url.addQueryItem("response_type", "token");
    this->load(url);
    qDebug()<<url;
}

void vkAuth::slotUrlChanged(QUrl url)
{
    qDebug()<<url;
    url = url.toString().replace("#","?");
    if (url.hasQueryItem("error"))
    {// in case of unsuccessfull authorisation
        qDebug()<<"vk api returned error statment";
        return ;
    }
    if (!url.hasQueryItem("access_token"))
    {
        qDebug()<<"Accsess denided";
        return;
    }
    //creating cookie file
    QFile file("token");
    file.open(QIODevice::ReadWrite | QIODevice::Text);
    QTextStream out(&file);
    //writing token and uid to vodiables
    token = url.queryItemValue("access_token");
    expires = url.queryItemValue("expires_in").toInt();
    uid = url.queryItemValue("user_id");
    //... and to file
    out<<token;
    file.close();
    //now we should have an token file
    qDebug()<<"TOKEN:";
    qDebug() << token;
    qDebug()<<"USER_ID:";
    qDebug()<<uid;
    this->hide();
    QMessageBox::information(
                this,
                tr("Login successful"),
                tr("You have successfuly log in.") );
    emit tokenSet(token,uid);
}
