#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "vkauth.h"
#include <QApplication>
#include "qdebug.h"
#include <QByteArray>
#include <QFile>
#include <QtXml>
#include <QXmlQuery>
#include <QtXmlPatterns>
#include <iostream>
#include <QAction>
#include <QMenu>
#include "playingcontrol.h"
#include <phonon>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent),ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->showTime->hide();
    ui->lineEdit->setPlaceholderText("Search here");
    //ui->lineEdit->hide();
    ui->loopButton->hide();
    setWindowIcon(QIcon(QPixmap(":/dark/icons/qvk.ico")));

    //HOTKEYS


    //\HOTKEYS

    ////////////////////////////////////////////////Creating actions
    QAction *login = new QAction(tr("Login"),this);
    QAction *About = new QAction(tr("About"),this);
    QAction *refrsh = new QAction(tr("Refresh"),this);
    connect(refrsh,SIGNAL(triggered()),this,SLOT(getAudioList()));
    connect(login, SIGNAL(triggered()), this, SLOT(loginSlot()));
    connect(About, SIGNAL(triggered()), this, SLOT(about()));

    ////////////////////////////////////////////////Creating menu
    QMenu *eyemenu = new QMenu("Options", ui->toolButton);
    eyemenu->addAction(refrsh);
    eyemenu->addAction(login);
    eyemenu->addSeparator();
    eyemenu->addAction(About);
    ///////////////////////////////////////////////making menu button
    ui->toolButton->setMenu(eyemenu);
    ui->musicWidget->setColumnCount(3);

    //table setting
    QStringList header;
    //write table header
    header <<"Artist"<<"Title"<<"Duration"/*<< "link"*/;
    //set table header
    ui->musicWidget->setHorizontalHeaderLabels(header);
    ui->musicWidget->setShowGrid(false);
    //ui->musicWidget->horizontalHeader()->setResizeMode(QHeaderView::ResizeToContents);
    ui->musicWidget->horizontalHeader()->setResizeMode(QHeaderView::Stretch);
    ui->musicWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->musicWidget->setSelectionBehavior(QAbstractItemView::SelectRows);

    //some connections
    playingControl *playControl = new playingControl;
    connect(playControl,SIGNAL(sentAudioOutput(Phonon::AudioOutput *)),SLOT(connectVolumeSlider(Phonon::AudioOutput *)));
    connect(playControl,SIGNAL(sentMediaObject(Phonon::MediaObject *)),SLOT(connectSeekSlider(Phonon::MediaObject *)));
    connect(ui->musicWidget,SIGNAL(cellDoubleClicked(int,int)),SLOT(playThatSong(int,int)));
    connect(ui->musicWidget,SIGNAL(cellClicked(int,int)),SLOT(selectThatSong(int,int)));
    connect(this,SIGNAL(playUrl(int)),playControl,SLOT(playSignalRecived(int)));
    connect(ui->tooglePlayingButton,SIGNAL(clicked()),playControl,SLOT(chState()));
    connect(playControl,SIGNAL(playing()),SLOT(isPlaying()));
    connect(playControl,SIGNAL(paused()),SLOT(isPaused()));
    connect(this,SIGNAL(setPlayingOrder(QList<QUrl>)),playControl,SLOT(setOrder(QList<QUrl>)));
    connect(playControl,SIGNAL(setRowBold(int,int)),SLOT(setUiToSong(int,int)));
    connect(ui->nextButton,SIGNAL(clicked()),playControl,SLOT(playNextSong()));
    connect(ui->prevButton,SIGNAL(clicked()),playControl,SLOT(playPrevSong()));
    connect(ui->shuffButton,SIGNAL(toggled(bool)),playControl,SLOT(setShuffle(bool)));
    connect(ui->lineEdit,SIGNAL(textChanged(QString)),SLOT(currentSearch(QString)));
    //some shitcode :-[
    //Sending Phonon objects to our class
    playControl->start();
    //checking file
    QFile file("token");
    if(!file.open(QIODevice::ReadOnly))
    {
        loginSlot();
    }
    else
    {
        QTextStream in(&file);
        token = in.readLine();
        file.close();
        getAudioList();
    }

}

void MainWindow::currentSearch(QString text)
{
    qDebug()<<"USER IS SEARCHING======================================";
    ui->lineEdit->setStyleSheet("QLineEdit{background: #FFFFFF;}");    //white for search line
    qDebug()<<text;
    QList<QTableWidgetItem *> foundList;
    foundList = ui->musicWidget->findItems(text,Qt::MatchContains);
    if(!foundList.isEmpty())
    {
        qDebug()<<"Found at row: " ;
        qDebug()<<foundList[0]->row()+1;
        ui->musicWidget->selectRow(foundList[0]->row());
    }
    else
    {
        ui->lineEdit->setStyleSheet("QLineEdit{background: #FF6666;}");   //error for the searchline
    }
}

void MainWindow::about()
{
    QMessageBox::information(this, tr("About QVkPlayer"),
                             tr("It is a Player for playing music from Your Vkontakte playlist.\n"
                                "More features will be avalible later.\n"
                                "\tfirst released version  'alpha 0.1'\n"
                                " credits:\n"
                                "\tMe: kazak1377(Maxim Kozachenko)\n"
                                "Thanks to:\n"
                                "\tQt team\n"
                                "\tmembers of c_plus_plus jabber.ru conference"));
}

void MainWindow::setUiToSong(int row, int rowPrev)
{
    QFont fontBold;
    QFont fontRegular;
    fontRegular.setBold(false);
    fontBold.setBold(true);
    ui->musicWidget->item(rowPrev,0)->setFont(fontRegular);
    ui->musicWidget->item(rowPrev,1)->setFont(fontRegular);
    ui->musicWidget->item(rowPrev,2)->setFont(fontRegular);
    ui->musicWidget->setCurrentCell(row,0);
    ui->musicWidget->item(row,0)->setFont(fontBold);
    ui->musicWidget->item(row,1)->setFont(fontBold);
    ui->musicWidget->item(row,2)->setFont(fontBold);
    ui->musicWidget->scrollToItem(ui->musicWidget->item(row,0));
}

void MainWindow::isPlaying()
{
    ui->tooglePlayingButton->setIcon(QIcon(QPixmap(":/dark/icons/dark/gtk-media-pause.png")));
}

void MainWindow::isPaused()
{
    ui->tooglePlayingButton->setIcon(QIcon(QPixmap(":/dark/icons/dark/gtk-media-play-ltr.png")));
}

void MainWindow::selectThatSong(int row,int /*coulumn*/)
{
    ui->musicWidget->item(row,2)->setSelected(true);
}

void MainWindow::playThatSong(int row,int coulumn)
{
    emit playUrl(row);
}

void MainWindow::connectVolumeSlider(Phonon::AudioOutput *audioOutput)
{
    ui->volumeSlider->setAudioOutput(audioOutput);
}

void MainWindow::connectSeekSlider(Phonon::MediaObject *mediaObject)
{
    ui->seekSlider->setMediaObject(mediaObject);
}

void MainWindow::setTableLine(QStringList line)
{
    int lastRow = ui->musicWidget->rowCount();
    ui->musicWidget->insertRow(lastRow);
    for(int i=0;i<=3;i++)
    {
        QTableWidgetItem *item=new QTableWidgetItem (line[i]);
        ui->musicWidget->setItem(lastRow,i,item);
    }
}

QString MainWindow :: durationToHuman(int d)
{
    int minutes = d/60;
    int seconds = d%60;
    QString out;
    if(seconds<10)
    {
        out = QString::number(minutes)+":0"+QString::number(seconds);
    }
    else
        out = QString::number(minutes)+":"+QString::number(seconds);
    return out;
}

void MainWindow::setToken(QString value,QString value2)
{
    token = value;
    userId = value2;
    this->getAudioList();
}

void MainWindow::loginSlot()
{
    vkAuth *loginWindow = new vkAuth;
    QObject::connect(loginWindow,SIGNAL(tokenSet(QString,QString)),SLOT(setToken(QString,QString)));
    loginWindow->show();
}

void MainWindow::replyFinished(QNetworkReply *reply)
{
    if(reply->error() == QNetworkReply::NoError)
    {
        qDebug()<<"Audio list get";
        QXmlStreamReader xml(reply);
        while(!xml.atEnd() && !xml.hasError())
        {
            /* Read next element.*/
            QXmlStreamReader::TokenType token = xml.readNext();
            /* If token is just StartDocument, we'll go to next.*/
            if(token == QXmlStreamReader::StartDocument)
            {
                continue;
            }
            /* If token is StartElement, we'll see if we can read it.*/
            if(token == QXmlStreamReader::StartElement)
            {
                /* If it's named persons, we'll go to the next.*/
                if(xml.name() == "audio")
                {
                    tableLine.clear();
                    continue;
                }
                /* If it's named person, we'll dig the information from there.*/
                if(xml.name() == "artist")
                {
                    tableLine.append(xml.readElementText());
                }
                if(xml.name() == "title")
                {
                    tableLine.append(xml.readElementText());
                }
                if(xml.name() == "duration")
                {
                    QString duration;
                    duration = durationToHuman(xml.readElementText().toInt());
                    tableLine.append(duration);
                }
                if(xml.name() == "url")
                {
                    QString line = xml.readElementText();
                    tableLine.append(line);
                    linkList.append(QUrl(line));
                    setTableLine(tableLine);
                }

            }
        }
        /* Error handling. */
        if(xml.hasError())
        {
            QMessageBox::critical(this,
                                  "QXSRExample::parseXML",
                                  xml.errorString(),
                                  QMessageBox::Ok);
        }
        /* Removes any device() or data from the reader
            * and resets its internal state to the initial state. */
        xml.clear();
    }
    else
    {
        qDebug()<<reply->errorString();
    }
    emit setPlayingOrder(linkList);
}

void MainWindow::getAudioList()    //it is our request function
{
    linkList.clear();
    QUrl rAudioUrl("https://api.vk.com/method/audio.get.xml?");
    ui->musicWidget->clear();
    ui->musicWidget->setRowCount(0);
    QStringList header;
    header <<"Artist"<<"Title"<<"Duration"<< "link";
    ui->musicWidget->setHorizontalHeaderLabels(header);
    rAudioUrl.addQueryItem("uid", userId);
    rAudioUrl.addQueryItem("access_token", token);
    qDebug()<<rAudioUrl;
    QNetworkAccessManager *manager = new QNetworkAccessManager(this);
    connect(manager, SIGNAL(finished(QNetworkReply*)),this, SLOT(replyFinished(QNetworkReply*)));
    manager->get(QNetworkRequest(rAudioUrl));
}

MainWindow::~MainWindow()
{
    delete ui;
}
