#include "playingcontrol.h"
#include <phonon>
#include "QThread"
#include <QtGlobal>
#include <QTime>
#include "mainwindow.h"

playingControl::playingControl(QObject *parent) : QThread(parent)
{
    currentSong = 0;
    prevSong = 0;
    nextSong = 0;
    shuffle=false;
    QTime time = QTime::currentTime();
    qsrand((uint)time.msec());
    mediaObject = new Phonon::MediaObject(this);
    audioOutput =new Phonon::AudioOutput(Phonon::MusicCategory, this);
    Phonon::Path path = Phonon::createPath(mediaObject, audioOutput);
    connect(mediaObject,SIGNAL(aboutToFinish()),SLOT(setNext()));
    connect(mediaObject,SIGNAL(finished()),SLOT(playNext()));
    connect(mediaObject,SIGNAL(bufferStatus(int)),SLOT(cachState(int)));
}

void playingControl::run()
{
    emit sentAudioOutput(audioOutput);
    emit sentMediaObject(mediaObject);
}

void playingControl::cachState(int val)
{
    qDebug()<<val;
}

int playingControl::randSong(int low,int high)
{
    return qrand() % ((high + 1) - low) + low;
}

void playingControl::setOrder(QList<QUrl> queue)
{
    mediaList = queue;
}

void playingControl::playSignalRecived(int i)
{
    qDebug()<<mediaList[i];
    mediaObject->stop();
    prevSong=currentSong;
    currentSong = i;
    nextSong = generateNextSong(i);
    if(prev==false)
    {
        history.push(prevSong);
    }
    qDebug()<< "prev: "<<prevSong<<"   current: "<<currentSong<<"   next: "<<nextSong;
    emit setRowBold(currentSong,prevSong);
    mediaObject->setCurrentSource(mediaList[i]);
    mediaObject->play();
    emit playing();
    prev=false;
}

void playingControl::setNext()
{
    prevSong = currentSong;
    currentSong = nextSong;
    nextSong = currentSong+1;
    flag = true;
}

void playingControl::playNext()
{
    emit setRowBold(currentSong,prevSong);
    playSignalRecived(currentSong);
}

void playingControl::playNextSong()
{
    emit setRowBold(currentSong,prevSong);
    playSignalRecived(nextSong);
}

void playingControl::setShuffle(bool s)
{
    qDebug()<<s;
    shuffle = s;
    if(s==true)
        nextSong = randSong(0,mediaList.size());
}

int playingControl::generateNextSong(int current)
{
    qDebug()<<mediaList.size();
    qDebug()<<shuffle;
    if(shuffle == true)
    {
        return randSong(0,mediaList.size());
    }
    else
    {
        return current+1;
    }
}

void playingControl::playPrevSong()
{
    emit setRowBold(currentSong,prevSong);
    prev = true;
    if(history.size()>1)
        playSignalRecived(history.pop());
    else
        playSignalRecived(currentSong);
}

void playingControl::chState()
{
    Phonon::State state = mediaObject->state();
    switch(state)
    {
    case Phonon::PlayingState:
        qDebug()<<"PAUSING";
        mediaObject->pause();
        emit paused();
        break;
    case Phonon::PausedState:
        qDebug()<<"PLAYING";
        mediaObject->play();
        emit playing();
        break;
    }
}
