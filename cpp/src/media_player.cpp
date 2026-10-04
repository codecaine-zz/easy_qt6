#include "simplegui/media_player.h"
#include "detail/common.h"

#include <QFrame>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QSignalBlocker>
#include <QSlider>
#include <QStringList>
#include <QVBoxLayout>

#include <algorithm>

namespace simplegui {
namespace {

// Parses "ss", "mm:ss" or "hh:mm:ss". Returns -1 when the text is not a valid duration.
int parse_duration(const std::string& text) {
    const QStringList parts = detail::qs(text).trimmed().split(QLatin1Char(':'));
    if (parts.isEmpty() || parts.size() > 3) return -1;
    long long total = 0;
    for (const QString& part : parts) {
        bool ok = false;
        const int v = part.toInt(&ok);
        if (!ok || v < 0) return -1;
        total = total * 60 + v;
        if (total > 24LL * 3600 * 365) return -1;  // reject absurd values
    }
    return static_cast<int>(total);
}

QString format_time(int seconds) {
    const int h = seconds / 3600;
    const int m = (seconds / 60) % 60;
    const int s = seconds % 60;
    if (h > 0) return QStringLiteral("%1:%2:%3").arg(h).arg(m, 2, 10, QLatin1Char('0')).arg(s, 2, 10, QLatin1Char('0'));
    return QStringLiteral("%1:%2").arg(m, 2, 10, QLatin1Char('0')).arg(s, 2, 10, QLatin1Char('0'));
}

QString play_icon(bool playing) { return QString(QChar(playing ? 0x23F8 : 0x25B6)); }

}  // namespace

struct MediaPlayer::Impl {
    QPointer<QFrame> container = new QFrame();
    QPointer<QLabel> title_lbl;
    QPointer<QLabel> artist_lbl;
    QPointer<QLabel> time_lbl;
    QPointer<QPushButton> play_btn;
    QPointer<QSlider> seek_slider;
    bool playing = false;
    int current_sec = 0;
    int total_sec = 225;
    detail::Event<bool> play_evt = detail::make_event<bool>(container);
    detail::Event<int> seek_evt = detail::make_event<int>(container);
    detail::Event<> prev_evt = detail::make_event<>(container);
    detail::Event<> next_evt = detail::make_event<>(container);

    ~Impl() { detail::delete_if_orphan(container); }

    void refresh() {
        current_sec = std::clamp(current_sec, 0, total_sec);
        if (seek_slider) {
            const QSignalBlocker block(seek_slider);
            seek_slider->setRange(0, total_sec);
            seek_slider->setValue(current_sec);
        }
        if (time_lbl) time_lbl->setText(format_time(current_sec) + QStringLiteral(" / ") + format_time(total_sec));
    }
};

MediaPlayer::MediaPlayer(const std::string& title, const std::string& artist, const std::string& duration_str)
    : pimpl(std::make_shared<Impl>()) {
    QFrame* frame = pimpl->container;
    frame->setProperty("sg_role", QStringLiteral("media_player"));
    detail::set_base_style(frame, QStringLiteral(
        "QFrame[sg_role=\"media_player\"] {"
        "  background: qlineargradient(x1:0, y1:0, x2:1, y2:1, stop:0 #18181b, stop:1 #09090b);"
        "  border: 1px solid #27272a; border-radius: 12px; }"));

    auto* layout = new QVBoxLayout(frame);
    layout->setContentsMargins(14, 14, 14, 14);
    layout->setSpacing(10);

    // Track info row
    auto* top_row = new QHBoxLayout();
    top_row->setSpacing(12);
    auto* art_box = new QLabel(QString(QChar(0x266A)), frame);  // musical note
    art_box->setFixedSize(48, 48);
    art_box->setAlignment(Qt::AlignCenter);
    art_box->setStyleSheet(QStringLiteral(
        "background: qlineargradient(x1:0, y1:0, x2:1, y2:1, stop:0 #a855f7, stop:1 #6366f1);"
        "border-radius: 8px; color: white; font-size: 20px; font-weight: bold;"));
    top_row->addWidget(art_box);

    auto* meta = new QVBoxLayout();
    meta->setSpacing(2);
    pimpl->title_lbl = detail::plain_label(title, frame);
    pimpl->title_lbl->setStyleSheet(QStringLiteral("color: #fafafa; font-weight: bold; font-size: 14px; background: transparent;"));
    pimpl->artist_lbl = detail::plain_label(artist, frame);
    pimpl->artist_lbl->setStyleSheet(QStringLiteral("color: #a1a1aa; font-size: 12px; background: transparent;"));
    meta->addWidget(pimpl->title_lbl);
    meta->addWidget(pimpl->artist_lbl);
    top_row->addLayout(meta);
    top_row->addStretch();
    layout->addLayout(top_row);

    // Seek bar
    pimpl->seek_slider = new QSlider(Qt::Horizontal, frame);
    pimpl->seek_slider->setStyleSheet(QStringLiteral(
        "QSlider::groove:horizontal { height: 4px; background: #3f3f46; border-radius: 2px; }"
        "QSlider::sub-page:horizontal { background: #a855f7; border-radius: 2px; }"
        "QSlider::handle:horizontal { background: #ffffff; border: none; width: 12px;"
        "  margin-top: -4px; margin-bottom: -4px; border-radius: 6px; }"));
    layout->addWidget(pimpl->seek_slider);

    // Time + buttons
    auto* bottom = new QHBoxLayout();
    pimpl->time_lbl = new QLabel(frame);
    pimpl->time_lbl->setStyleSheet(QStringLiteral("color: #71717a; font-size: 11px; background: transparent;"));
    bottom->addWidget(pimpl->time_lbl);
    bottom->addStretch();

    const QString side_css = QStringLiteral(
        "QPushButton { background: transparent; color: #a1a1aa; font-size: 14px; border: none; }"
        "QPushButton:hover { color: white; }");
    auto make_btn = [frame](const QString& text, int size, const QString& css) {
        auto* b = new QPushButton(text, frame);
        b->setCursor(Qt::PointingHandCursor);
        b->setFixedSize(size, size);
        b->setStyleSheet(css);
        return b;
    };
    auto* prev_btn = make_btn(QString(QChar(0x23EE)), 28, side_css);
    pimpl->play_btn = make_btn(play_icon(false), 32, QStringLiteral(
        "QPushButton { background: #a855f7; color: white; border-radius: 16px; border: none;"
        "  font-size: 13px; padding-left: 2px; }"
        "QPushButton:hover { background: #9333ea; }"));
    auto* next_btn = make_btn(QString(QChar(0x23ED)), 28, side_css);
    prev_btn->setToolTip(QStringLiteral("Previous"));
    pimpl->play_btn->setToolTip(QStringLiteral("Play / Pause"));
    next_btn->setToolTip(QStringLiteral("Next"));
    bottom->addWidget(prev_btn);
    bottom->addWidget(pimpl->play_btn);
    bottom->addWidget(next_btn);
    layout->addLayout(bottom);

    std::weak_ptr<Impl> weak = pimpl;
    QObject::connect(pimpl->play_btn.data(), &QPushButton::clicked, [weak]() {
        auto d = weak.lock();
        if (!d) return;
        d->playing = !d->playing;
        if (d->play_btn) d->play_btn->setText(play_icon(d->playing));
        detail::fire(d->play_evt, d->playing);
    });
    QObject::connect(pimpl->seek_slider.data(), &QSlider::valueChanged, [weak](int value) {
        auto d = weak.lock();
        if (!d) return;
        d->current_sec = value;
        d->refresh();
        detail::fire(d->seek_evt, value);
    });
    QObject::connect(prev_btn, &QPushButton::clicked, [weak]() {
        if (auto d = weak.lock()) detail::fire(d->prev_evt);
    });
    QObject::connect(next_btn, &QPushButton::clicked, [weak]() {
        if (auto d = weak.lock()) detail::fire(d->next_evt);
    });

    set_track(title, artist, duration_str);
}

MediaPlayer::~MediaPlayer() = default;

void MediaPlayer::set_track(const std::string& title, const std::string& artist, const std::string& duration_str) {
    if (pimpl->title_lbl) pimpl->title_lbl->setText(detail::qs(title));
    if (pimpl->artist_lbl) pimpl->artist_lbl->setText(detail::qs(artist));
    const int total = parse_duration(duration_str);
    if (total >= 0) pimpl->total_sec = total;
    pimpl->current_sec = 0;
    pimpl->refresh();
}

void MediaPlayer::set_position(int seconds, int total_seconds) {
    pimpl->total_sec = std::max(0, total_seconds);
    pimpl->current_sec = seconds;
    pimpl->refresh();
}

void MediaPlayer::set_position(int seconds) {
    pimpl->current_sec = seconds;
    pimpl->refresh();
}

int MediaPlayer::position() const { return pimpl->current_sec; }
int MediaPlayer::duration() const { return pimpl->total_sec; }

void MediaPlayer::set_playing(bool is_playing) {
    pimpl->playing = is_playing;
    if (pimpl->play_btn) pimpl->play_btn->setText(play_icon(is_playing));
}

bool MediaPlayer::is_playing() const { return pimpl->playing; }

EventConnection MediaPlayer::on_play_pause(std::function<void(bool)> handler) {
    return detail::add_handler(pimpl->play_evt, std::move(handler));
}
EventConnection MediaPlayer::on_seek(std::function<void(int)> handler) {
    return detail::add_handler(pimpl->seek_evt, std::move(handler));
}
EventConnection MediaPlayer::on_previous(std::function<void()> handler) {
    return detail::add_handler(pimpl->prev_evt, std::move(handler));
}
EventConnection MediaPlayer::on_next(std::function<void()> handler) {
    return detail::add_handler(pimpl->next_evt, std::move(handler));
}

QWidget* MediaPlayer::get_qwidget() const { return pimpl->container.data(); }

}  // namespace simplegui
