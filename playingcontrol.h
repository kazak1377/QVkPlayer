#ifndef PLAYINGCONTROL_H
#define PLAYINGCONTROL_H

#include <QThread>
#include <phonon>
#include <QStack>
#include "mainwindow.h"
#include <QList>


class playingControl : public QThread
{
//objects
    Q_OBJECT
    void run();
    bool flag;
    QStack<int> history;
    bool prev;
    bool shuffle;
    int currentSong;
    int nextSong;
    int prevSong;
    QList<QUrl> mediaList;
    Phonon::MediaObject *mediaObject;
    Phonon::AudioOutput *audioOutput;
//methods
    void changeState();
    int generateNextSong(int currrent);
    int randSong(int low,int high);

public:
    explicit playingControl(QObject *parent = 0);

signals:
    void sentAudioOutput(Phonon::AudioOutput*);
    void sentMediaObject(Phonon::MediaObject*);
    void paused();
    void playing();
    void setRowBold(int,int);
    
public slots:
    void playSignalRecived(int i);
    void chState();
    void setOrder(QList<QUrl>);
    void playNextSong();
    void playPrevSong();
    void setShuffle(bool);
private slots:
    void setNext();
    void playNext();   
};

#endif // PLAYINGCONTROL_H
