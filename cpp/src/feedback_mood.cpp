#include "simplegui/feedback_mood.h"
#include "detail/common.h"

#include <QFrame>
#include <QHBoxLayout>
#include <QPushButton>

#include <algorithm>
#include <vector>

namespace simplegui {

struct FeedbackMood::Impl {
    QPointer<QFrame> container = new QFrame();
    std::vector<QPointer<QPushButton>> buttons;
    int current_rating = 0;
    detail::Event<int> changed = detail::make_event<int>(container);

    ~Impl() { detail::delete_if_orphan(container); }

    void update_styles() {
        for (size_t i = 0; i < buttons.size(); ++i) {
            if (!buttons[i]) continue;
            const bool selected = static_cast<int>(i + 1) == current_rating;
            buttons[i]->setStyleSheet(selected
                ? QStringLiteral("QPushButton { background: #2563eb; border: 2px solid #60a5fa;"
                                 "  border-radius: 12px; padding: 8px 12px; font-size: 20px; }")
                : QStringLiteral("QPushButton { background: #1e293b; border: 1px solid #334155;"
                                 "  border-radius: 12px; padding: 8px 12px; font-size: 20px; }"
                                 "QPushButton:hover { background: #334155; border-color: #475569; }"));
        }
    }
};

FeedbackMood::FeedbackMood(int initial_rating) : pimpl(std::make_shared<Impl>()) {
    QFrame* frame = pimpl->container;
    frame->setStyleSheet(QStringLiteral("background: transparent;"));
    auto* layout = new QHBoxLayout(frame);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(10);

    // Emoji code points: angry, slightly frowning, neutral, slightly smiling, star-struck.
    const char32_t faces[] = {0x1F621, 0x1F641, 0x1F610, 0x1F642, 0x1F929};
    const char* tips[] = {"Terrible", "Bad", "Okay", "Good", "Great"};

    std::weak_ptr<Impl> weak = pimpl;
    for (int i = 0; i < 5; ++i) {
        auto* btn = new QPushButton(QString::fromUcs4(&faces[i], 1), frame);
        btn->setToolTip(QString::fromLatin1(tips[i]));
        btn->setCursor(Qt::PointingHandCursor);
        btn->setFixedSize(52, 52);
        const int value = i + 1;
        QObject::connect(btn, &QPushButton::clicked, [weak, value]() {
            auto d = weak.lock();
            if (!d) return;
            d->current_rating = value;
            d->update_styles();
            detail::fire(d->changed, value);
        });
        layout->addWidget(btn);
        pimpl->buttons.push_back(btn);
    }
    set_rating(initial_rating);
}

FeedbackMood::~FeedbackMood() = default;

void FeedbackMood::set_rating(int rating) {
    pimpl->current_rating = std::clamp(rating, 0, 5);
    pimpl->update_styles();
}

int FeedbackMood::rating() const { return pimpl->current_rating; }

EventConnection FeedbackMood::on_change(std::function<void(int)> handler) {
    return detail::add_handler(pimpl->changed, std::move(handler));
}

QWidget* FeedbackMood::get_qwidget() const { return pimpl->container.data(); }

}  // namespace simplegui
