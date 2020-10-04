#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "mpvadapter.h"
#include "iconfactory.h"
#include "widgets/definitionwidget.h"

#include <QFileDialog>
#include <QMimeData>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), m_ui(new Ui::MainWindow)
{
    m_ui->setupUi(this);

    m_actionGroupAudio = new QActionGroup(this);
    m_ui->m_actionAudioNone->setActionGroup(m_actionGroupAudio);
    m_actionGroupVideo = new QActionGroup(this);
    m_ui->m_actionVideoNone->setActionGroup(m_actionGroupVideo);
    m_actionGroupSubtitle = new QActionGroup(this);
    m_ui->m_actionSubtitleNone->setActionGroup(m_actionGroupSubtitle);

    m_player = new MpvAdapter(m_ui->m_mpv, this);

    m_ui->m_controls->setVolumeLimit(m_player->getMaxVolume());

    // Toolbar Actions
    connect(m_ui->m_actionOpen, &QAction::triggered, this, &MainWindow::open);

    // Buttons
    connect(m_ui->m_controls, &PlayerControls::play, m_player, &PlayerAdapter::play);
    connect(m_ui->m_controls, &PlayerControls::pause, m_player, &PlayerAdapter::pause);
    connect(m_ui->m_controls, &PlayerControls::stop, m_player, &PlayerAdapter::stop);
    connect(m_ui->m_controls, &PlayerControls::skipForward, m_player, &PlayerAdapter::skipForward);
    connect(m_ui->m_controls, &PlayerControls::skipBackward, m_player, &PlayerAdapter::skipBackward);
    connect(m_ui->m_controls, &PlayerControls::seekForward, m_player, &PlayerAdapter::seekForward);
    connect(m_ui->m_controls, &PlayerControls::seekBackward, m_player, &PlayerAdapter::seekBackward);
    connect(m_ui->m_controls, &PlayerControls::fullscreenChanged, m_player, &PlayerAdapter::setFullscreen);

    // Slider
    connect(m_ui->m_controls, &PlayerControls::volumeSliderMoved, m_player, &PlayerAdapter::setVolume);
    connect(m_ui->m_controls, &PlayerControls::sliderMoved, m_player, &PlayerAdapter::seek);
    connect(m_player, &PlayerAdapter::durationChanged, m_ui->m_controls, &PlayerControls::setDuration);
    connect(m_player, &PlayerAdapter::positionChanged, m_ui->m_controls, &PlayerControls::setPosition);

    // State changes
    connect(m_player, &PlayerAdapter::subtitleChanged, m_ui->m_controls, &PlayerControls::setSubtitle);
    connect(m_player, &PlayerAdapter::stateChanged, m_ui->m_controls, &PlayerControls::setPaused);
    connect(m_player, &PlayerAdapter::fullscreenChanged, this, &MainWindow::setFullscreen);
    connect(m_player, &PlayerAdapter::fullscreenChanged, m_ui->m_controls, &PlayerControls::setFullscreen);
    connect(m_player, &PlayerAdapter::volumeChanged, m_ui->m_controls, &PlayerControls::setVolume);
    connect(m_player, &PlayerAdapter::hideCursor, [=] { if(isFullScreen() && !m_ui->m_controls->underMouse()) m_ui->m_controls->hide(); });
    connect(m_player, &PlayerAdapter::close, this, &QApplication::quit);

    // Key/Mouse presses
    connect(this, &MainWindow::keyPressed, m_player, &PlayerAdapter::keyPressed);
    connect(this, &MainWindow::wheelMoved, m_player, &PlayerAdapter::mouseWheelMoved);

    // Track changes
    connect(m_player, &PlayerAdapter::tracksChanged, this, &MainWindow::setTracks);
    connect(m_player, &PlayerAdapter::audioTrackChanged, 
            [=] (const int id) { if(!m_audioTracks.isEmpty()) m_audioTracks[id - 1].first->setChecked(true); } );
    connect(m_player, &PlayerAdapter::videoTrackChanged,
            [=] (const int id) { if(!m_videoTracks.isEmpty()) m_videoTracks[id - 1].first->setChecked(true); } );
    connect(m_player, &PlayerAdapter::subtitleTrackChanged,
            [=] (const int id) { if(!m_subtitleTracks.isEmpty()) m_subtitleTracks[id - 1].first->setChecked(true); } );
    connect(m_player, &PlayerAdapter::audioDisabled, [=] { m_ui->m_actionAudioNone->setChecked(true); } );
    connect(m_player, &PlayerAdapter::videoDisabled, [=] { m_ui->m_actionVideoNone->setChecked(true); } );
    connect(m_player, &PlayerAdapter::subtitleDisabled, [=] { m_ui->m_actionSubtitleNone->setChecked(true); } );
    connect(m_ui->m_actionAudioNone, &QAction::triggered, m_player, &PlayerAdapter::disableAudio);
    connect(m_ui->m_actionVideoNone, &QAction::triggered, m_player, &PlayerAdapter::disableVideo);
    connect(m_ui->m_actionSubtitleNone, &QAction::triggered, m_player, &PlayerAdapter::disableSubtitles);

    m_defintion = 0;
}


MainWindow::~MainWindow()
{
    clearTracks();
    delete m_ui;
    delete m_player;
    delete m_actionGroupAudio;
    delete m_actionGroupVideo;
    delete m_actionGroupSubtitle;
    delete m_defintion;
}

void MainWindow::keyPressEvent(QKeyEvent *event)
{
    switch (event->key())
    {
    default:
    {
        Q_EMIT keyPressed(event);
    };
    }
}

void MainWindow::wheelEvent(QWheelEvent *event)
{
    Q_EMIT wheelMoved(event);
}

void MainWindow::dragEnterEvent(QDragEnterEvent *event)
{
    if (event->mimeData()->hasFormat("text/uri-list"))
        event->acceptProposedAction();
}

void MainWindow::dropEvent(QDropEvent *event)
{
    if (event->mimeData()->hasUrls())
        m_player->open(event->mimeData()->urls());
}

void MainWindow::setFullscreen(bool value)
{
    if (value)
    {
        showFullScreen();
        m_ui->m_menubar->hide();
        QApplication::processEvents();
        m_ui->m_controls->hide();
        m_ui->m_centralwidget->layout()->removeWidget(m_ui->m_controls);
        m_ui->m_controls->raise();
    }
    else
    {
        showNormal();
        m_ui->m_menubar->show();
        m_ui->m_controls->show();
        m_ui->m_centralwidget->layout()->addWidget(m_ui->m_controls);
    }
}

void MainWindow::mouseMoveEvent(QMouseEvent *event)
{
    if (isFullScreen() && m_ui->m_controls->isHidden())
    {
        m_ui->m_controls->show();
    }
    //int offset = isFullScreen() ? m_ui->m_controls->height() : 0;
    //m_defintion->move(event->x() - (m_defintion->width() / 2), m_ui->m_mpv->height() - m_defintion->height() - offset);
}

void MainWindow::setTracks(QVector<const PlayerAdapter::Track *> tracks)
{
    clearTracks();

    for (auto it = tracks.begin(); it != tracks.end(); ++it)
    {
        const PlayerAdapter::Track *track = *it;
        QString actionText = "Track " + QString::number(track->id);
        if (!track->lang.isEmpty())
            actionText += " [" + track->lang + "]";
        if (!track->title.isEmpty())
            actionText += " - " + track->title;

        QAction *action = new QAction(this);
        action->setText(actionText);
        action->setCheckable(true);
        QVector<QPair<QAction *, const PlayerAdapter::Track *>> *trackList;
        switch (track->type)
        {
        case PlayerAdapter::Track::track_type::audio:
            m_ui->m_menuAudio->addAction(action);
            action->setActionGroup(m_actionGroupAudio);
            connect(action, &QAction::triggered, [=] (const bool checked) { if (checked) m_player->setAudioTrack(track->id); } );
            trackList = &m_audioTracks;
            break;
        case PlayerAdapter::Track::track_type::video:
            m_ui->m_menuVideo->addAction(action);
            action->setActionGroup(m_actionGroupVideo);
            connect(action, &QAction::triggered, [=] (const bool checked) { if (checked) m_player->setVideoTrack(track->id); } );
            trackList = &m_videoTracks;
            break;
        case PlayerAdapter::Track::track_type::subtitle:
            m_ui->m_menuSubtitle->addAction(action);
            action->setActionGroup(m_actionGroupSubtitle);
            connect(action, &QAction::triggered, [=] (const bool checked) { if (checked) m_player->setSubtitleTrack(track->id); } );
            trackList = &m_subtitleTracks;
            break;
        }
        action->setChecked(track->selected);
        trackList->push_back(QPair<QAction *, const PlayerAdapter::Track *>(action, track));
    }
}

void MainWindow::clearTracks()
{
    clearTrack(m_audioTracks, m_ui->m_menuAudio, m_actionGroupAudio, m_ui->m_actionAudioNone);
    clearTrack(m_videoTracks, m_ui->m_menuVideo, m_actionGroupVideo, m_ui->m_actionVideoNone);
    clearTrack(m_subtitleTracks, m_ui->m_menuSubtitle, m_actionGroupSubtitle, m_ui->m_actionSubtitleNone);
}

void MainWindow::clearTrack(QVector<QPair<QAction *, const PlayerAdapter::Track *>> &tracks, QMenu *menu, QActionGroup *actionGroup, QAction *actionDisable)
{
    for (auto it = tracks.begin(); it != tracks.end(); ++it)
    {
        menu->removeAction((*it).first);
        actionGroup->removeAction((*it).first);
        delete (*it).first;
        delete (*it).second;
    }
    tracks.clear();

    actionDisable->setChecked(true);
}

void MainWindow::open()
{
    QList<QUrl> files = QFileDialog::getOpenFileUrls(0, "Open a video");
    m_player->open(files);
}