#include "simplegui/media_player.h"
#include <QFrame>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QSlider>
#include <QPointer>

namespace simplegui {

struct MediaPlayer::Impl {
    QPointer<QFrame> container;
    QPointer<QLabel> title_lbl;
    QPointer<QLabel> artist_lbl;
    QPointer<QLabel> time_lbl;
    QPointer<QPushButton> play_btn;
    QPointer<QSlider> seek_slider;
    bool is_playing = false;
    int current_sec = 0;
    int total_sec = 225; // 03:45 default
    std::function<void(bool)> play_handler;
    std::function<void(int)> seek_handler;

    void update_time_display() {
        if (!time_lbl) return;
        char buf[64];
        snprintf(buf, sizeof(buf), "%02d:%02d / %02d:%02d",
                 current_sec / 60, current_sec % 60,
                 total_sec / 60, total_sec % 60);
        time_lbl->setText(QString::fromUtf8(buf));
    }
};

MediaPlayer::MediaPlayer(const std::string& title,
                         const std::string& artist,
                         const std::string& duration_str)
    : pimpl(std::make_shared<Impl>())
{
    auto* frame = new QFrame();
    pimpl->container = frame;
    frame->setFrameShape(QFrame::StyledPanel);
    frame->setStyleSheet(
        "QFrame {"
        "  background: qlineargradient(x1:0, y1:0, x2:1, y2:1, stop:0 #18181b, stop:1 #09090b);"
        "  border: 1px solid #27272a;"
        "  border-radius: 12px;"
        "  padding: 12px;"
        "}"
    );

    auto* layout = new QVBoxLayout(frame);
    layout->setContentsMargins(14, 14, 14, 14);
    layout->setSpacing(10);

    // Track metadata row
    auto* top_row = new QHBoxLayout();
    top_row->setContentsMargins(0, 0, 0, 0);
    top_row->setSpacing(12);

    auto* art_box = new QFrame(frame);
    art_box->setFixedSize(48, 48);
    art_box->setStyleSheet(
        "QFrame {"
        "  background: qlineargradient(x1:0, y1:0, x2:1, y2:1, stop:0 #a855f7, stop:1 #6366f1);"
        "  border-radius: 8px;"
        "  border: none;"
        "}"
    );
    auto* art_layout = new QVBoxLayout(art_box);
    art_layout->setContentsMargins(0, 0, 0, 0);
    auto* art_icon = new QLabel(QString::fromUtf8("♪"), art_box);
    art_icon->setAlignment(Qt::AlignCenter);
    art_icon->setStyleSheet("color: white; font-size: 20px; font-weight: bold; background: transparent; border: none;");
    art_layout->addWidget(art_icon);
    top_row->addWidget(art_box);

    auto* meta_layout = new QVBoxLayout();
    meta_layout->setContentsMargins(0, 0, 0, 0);
    meta_layout->setSpacing(2);

    auto* title_lbl = new QLabel(QString::fromStdString(title), frame);
    pimpl->title_lbl = title_lbl;
    title_lbl->setStyleSheet("color: #fafafa; font-weight: bold; font-size: 14px; border: none; background: transparent;");
    meta_layout->addWidget(title_lbl);

    auto* artist_lbl = new QLabel(QString::fromStdString(artist), frame);
    pimpl->artist_lbl = artist_lbl;
    artist_lbl->setStyleSheet("color: #a1a1aa; font-size: 12px; border: none; background: transparent;");
    meta_layout->addWidget(artist_lbl);

    top_row->addLayout(meta_layout);
    top_row->addStretch();
    layout->addLayout(top_row);

    // Slider row
    auto* slider = new QSlider(Qt::Horizontal, frame);
    pimpl->seek_slider = slider;
    slider->setRange(0, pimpl->total_sec);
    slider->setValue(0);
    slider->setStyleSheet(
        "QSlider::groove:horizontal {"
        "  height: 4px;"
        "  background: #3f3f46;"
        "  border-radius: 2px;"
        "}"
        "QSlider::sub-page:horizontal {"
        "  background: #a855f7;"
        "  border-radius: 2px;"
        "}"
        "QSlider::handle:horizontal {"
        "  background: #ffffff;"
        "  border: none;"
        "  width: 12px;"
        "  margin-top: -4px;"
        "  margin-bottom: -4px;"
        "  border-radius: 6px;"
        "}"
    );
    layout->addWidget(slider);

    // Bottom controls row: Time & Buttons
    auto* bottom_row = new QHBoxLayout();
    bottom_row->setContentsMargins(0, 0, 0, 0);

    auto* time_lbl = new QLabel(QString::fromStdString("00:00 / " + duration_str), frame);
    pimpl->time_lbl = time_lbl;
    time_lbl->setStyleSheet("color: #71717a; font-size: 11px; border: none; background: transparent;");
    bottom_row->addWidget(time_lbl);

    bottom_row->addStretch();

    auto make_ctrl_btn = [&](const QString& text, int size) {
        auto* b = new QPushButton(text, frame);
        b->setCursor(Qt::PointingHandCursor);
        b->setFixedSize(size, size);
        return b;
    };

    auto* prev_btn = make_ctrl_btn(QString::fromUtf8("⏮"), 28);
    prev_btn->setStyleSheet("QPushButton { background: transparent; color: #a1a1aa; font-size: 14px; border: none; } QPushButton:hover { color: white; }");
    bottom_row->addWidget(prev_btn);

    auto* play_btn = make_ctrl_btn(QString::fromUtf8("▶"), 32);
    pimpl->play_btn = play_btn;
    play_btn->setStyleSheet(
        "QPushButton {"
        "  background: #a855f7;"
        "  color: white;"
        "  border-radius: 16px;"
        "  border: none;"
        "  font-size: 13px;"
        "  padding-left: 2px;"
        "}"
        "QPushButton:hover {"
        "  background: #9333ea;"
        "}"
    );
    bottom_row->addWidget(play_btn);

    auto* next_btn = make_ctrl_btn(QString::fromUtf8("⏭"), 28);
    next_btn->setStyleSheet("QPushButton { background: transparent; color: #a1a1aa; font-size: 14px; border: none; } QPushButton:hover { color: white; }");
    bottom_row->addWidget(next_btn);

    layout->addLayout(bottom_row);

    QObject::connect(play_btn, &QPushButton::clicked, [this]() {
        set_playing(!pimpl->is_playing);
        if (pimpl->play_handler) {
            pimpl->play_handler(pimpl->is_playing);
        }
    });

    QObject::connect(slider, &QSlider::sliderMoved, [this](int val) {
        pimpl->current_sec = val;
        pimpl->update_time_display();
        if (pimpl->seek_handler) {
            pimpl->seek_handler(val);
        }
    });
}

MediaPlayer::~MediaPlayer() = default;

void MediaPlayer::set_track(const std::string& title, const std::string& artist, const std::string& duration_str) {
    if (pimpl->title_lbl) pimpl->title_lbl->setText(QString::fromStdString(title));
    if (pimpl->artist_lbl) pimpl->artist_lbl->setText(QString::fromStdString(artist));
    pimpl->update_time_display();
}

void MediaPlayer::set_position(int seconds, int total_seconds) {
    pimpl->current_sec = seconds;
    pimpl->total_sec = total_seconds;
    if (pimpl->seek_slider) {
        pimpl->seek_slider->setRange(0, total_seconds);
        pimpl->seek_slider->setValue(seconds);
    }
    pimpl->update_time_display();
}

void MediaPlayer::set_playing(bool is_playing) {
    pimpl->is_playing = is_playing;
    if (pimpl->play_btn) {
        pimpl->play_btn->setText(is_playing ? QString::fromUtf8("⏸") : QString::fromUtf8("▶"));
    }
}

bool MediaPlayer::is_playing() const {
    return pimpl->is_playing;
}

EventConnection MediaPlayer::on_play_pause(std::function<void(bool is_playing)> handler) {
    pimpl->play_handler = handler;
    return EventConnection();
}

EventConnection MediaPlayer::on_seek(std::function<void(int seconds)> handler) {
    pimpl->seek_handler = handler;
    return EventConnection();
}

QWidget* MediaPlayer::get_qwidget() const {
    return pimpl->container.data();
}

}
