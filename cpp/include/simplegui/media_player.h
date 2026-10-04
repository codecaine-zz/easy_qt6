#pragma once
#include "simplegui/control.h"
#include "simplegui/event_connection.h"
#include <functional>
#include <memory>
#include <string>

namespace simplegui {

// A music-player style panel: title, artist, seek bar, time, and
// previous / play-pause / next buttons.
// NOTE: this control is only the user interface. It does not play audio by itself;
// connect its events to your own playback code.
// Durations are text like "03:45" (minutes:seconds) or "1:02:03" (hours:minutes:seconds).
class MediaPlayer : public Control {
public:
    MediaPlayer(const std::string& title = "Song Title",
                const std::string& artist = "Artist Name",
                const std::string& duration_str = "03:45");
    ~MediaPlayer() override;

    void set_track(const std::string& title, const std::string& artist, const std::string& duration_str);
    void set_position(int seconds, int total_seconds);   // move the seek bar
    void set_position(int seconds);                      // keep the current total length
    int position() const;                                // current second
    int duration() const;                                // total seconds
    void set_playing(bool is_playing);                   // switch the play/pause icon
    bool is_playing() const;

    EventConnection on_play_pause(std::function<void(bool is_playing)> handler);
    EventConnection on_seek(std::function<void(int seconds)> handler);   // user dragged the seek bar
    EventConnection on_previous(std::function<void()> handler);
    EventConnection on_next(std::function<void()> handler);

    QWidget* get_qwidget() const override;

private:
    struct Impl;
    std::shared_ptr<Impl> pimpl;
};

}  // namespace simplegui
