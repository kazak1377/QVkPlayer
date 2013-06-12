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
    {// обработка неудачной авторизации
        qDebug()<<"vk api returned error statment";
        return ;
    }
    if (!url.hasQueryItem("access_token"))
    {
        qDebug()<<"Accsess denided";
        return;
    }
    //Открыть файл для записи token
    QFile file("token");
    file.open(QIODevice::ReadWrite | QIODevice::Text);
    QTextStream out(&file);
    //дальше записываем данные юзера в переменные
    token = url.queryItemValue("access_token");
    expires = url.queryItemValue("expires_in").toInt();
    uid = url.queryItemValue("user_id");
    //так же и в файл
    out<<token;
    file.close();
    //теперь должны получить файл с нашим Token
    qDebug()<<"TOKEN:";
    qDebug() << token;
    qDebug()<<"USER_ID:";
    qDebug()<<uid;
    this->hide();
    QMessageBox::information(
                this,
                tr("Login successful"),
                tr("You heve successfuly log in.") );
    emit tokenSet(token,uid);
}
