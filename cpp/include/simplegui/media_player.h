#pragma once
#include "simplegui/control.h"
#include "simplegui/event_connection.h"
#include <memory>
#include <string>
#include <functional>

namespace simplegui {

class MediaPlayer : public Control {
public:
    MediaPlayer(const std::string& title = "Song Title",
                const std::string& artist = "Artist Name",
                const std::string& duration_str = "03:45");
    ~MediaPlayer() override;

    void set_track(const std::string& title, const std::string& artist, const std::string& duration_str);
    void set_position(int seconds, int total_seconds);
    void set_playing(bool is_playing);
    bool is_playing() const;

    EventConnection on_play_pause(std::function<void(bool is_playing)> handler);
    EventConnection on_seek(std::function<void(int seconds)> handler);

    QWidget* get_qwidget() const override;

private:
    struct Impl;
    std::shared_ptr<Impl> pimpl;
};

}
