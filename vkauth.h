#ifndef VKAUTH_H
#define VKAUTH_H

#include <QWebView>

class vkAuth : public QWebView
{
    Q_OBJECT

protected:

public:
    explicit vkAuth(QWidget *parent = 0);
    QString token;
    QString uid;
private:
    int expires;
signals:
    void tokenSet(QString,QString);
public slots:
    void slotUrlChanged(QUrl);
};

#endif // VKAUTH_H
