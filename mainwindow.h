#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkReply>
#include <QMessageBox>
#include <QByteArray>
#include "vkauth.h"
#include <QXmlStreamReader>
#include <QAction>
#include <QMenu>
#include <phonon>
#include <QFile>


namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = 0);

    ~MainWindow();

private:
    void setTableLine(QStringList line);
    QString durationToHuman(int d);
    Ui::MainWindow *ui;
    QString token;
    QString userId;
    QStringList tableLine;
    QList<QUrl> linkList;
    QAction *login;
    QMenu *eyemenu;
    void *customMessageHandler(QtMsgType type, const char *msg);


public slots:
    void loginSlot();
    void getAudioList();
    void setUiToSong(int,int);
private slots:
    void playThatSong(int row,int coulumn);
    void replyFinished(QNetworkReply *reply);
    void setToken(QString,QString);
    void connectVolumeSlider(Phonon::AudioOutput *audioOutput);
    void connectSeekSlider(Phonon::MediaObject *mediaObject);
    void isPlaying();
    void isPaused();
    void about();
    void selectThatSong(int,int);
    void currentSearch(QString text);
signals:
    void playUrl(int);
    void setPlayingOrder(QList<QUrl>);

};

#endif // MAINWINDOW_H
