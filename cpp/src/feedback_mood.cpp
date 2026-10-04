#include "simplegui/feedback_mood.h"
#include <QFrame>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QPushButton>
#include <QLabel>
#include <QPointer>
#include <vector>

namespace simplegui {

struct FeedbackMood::Impl {
    QPointer<QFrame> container;
    std::vector<QPointer<QPushButton>> buttons;
    int current_rating = 0;
    std::function<void(int)> change_handler;

    void update_styles() {
        for (size_t i = 0; i < buttons.size(); ++i) {
            if (!buttons[i]) continue;
            bool selected = (static_cast<int>(i + 1) == current_rating);
            if (selected) {
                buttons[i]->setStyleSheet(
                    "QPushButton {"
                    "  background: #2563eb;"
                    "  border: 2px solid #60a5fa;"
                    "  border-radius: 12px;"
                    "  padding: 8px 12px;"
                    "  font-size: 20px;"
                    "}"
                );
            } else {
                buttons[i]->setStyleSheet(
                    "QPushButton {"
                    "  background: #1e293b;"
                    "  border: 1px solid #334155;"
                    "  border-radius: 12px;"
                    "  padding: 8px 12px;"
                    "  font-size: 20px;"
                    "}"
                    "QPushButton:hover {"
                    "  background: #334155;"
                    "  border-color: #475569;"
                    "}"
                );
            }
        }
    }
};

FeedbackMood::FeedbackMood(int initial_rating)
    : pimpl(std::make_shared<Impl>())
{
    pimpl->current_rating = initial_rating;

    auto* frame = new QFrame();
    pimpl->container = frame;
    frame->setStyleSheet("background: transparent;");

    auto* layout = new QHBoxLayout(frame);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(10);

    const char* emojis[] = {"😡", "🙁", "😐", "🙂", "🤩"};
    const char* tooltips[] = {"Terrible", "Bad", "Okay", "Good", "Great"};

    for (int i = 0; i < 5; ++i) {
        auto* btn = new QPushButton(QString::fromUtf8(emojis[i]), frame);
        btn->setToolTip(QString::fromUtf8(tooltips[i]));
        btn->setCursor(Qt::PointingHandCursor);
        btn->setFixedSize(52, 52);

        int rating_val = i + 1;
        QObject::connect(btn, &QPushButton::clicked, [this, rating_val]() {
            set_rating(rating_val);
            if (pimpl->change_handler) {
                pimpl->change_handler(rating_val);
            }
        });

        layout->addWidget(btn);
        pimpl->buttons.push_back(btn);
    }

    pimpl->update_styles();
}

FeedbackMood::~FeedbackMood() = default;

void FeedbackMood::set_rating(int rating) {
    pimpl->current_rating = rating;
    pimpl->update_styles();
}

int FeedbackMood::rating() const {
    return pimpl->current_rating;
}

EventConnection FeedbackMood::on_change(std::function<void(int rating)> handler) {
    pimpl->change_handler = handler;
    return EventConnection();
}

QWidget* FeedbackMood::get_qwidget() const {
    return pimpl->container.data();
}

}
