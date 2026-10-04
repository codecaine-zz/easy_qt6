#include "simplegui/user_profile_card.h"
#include "detail/common.h"

#include <QFrame>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>

namespace simplegui {

struct UserProfileCard::Impl {
    QPointer<QFrame> frame = new QFrame();
    QPointer<QLabel> status_dot;
    QPointer<QLabel> lbl_bio;
    QPointer<QPushButton> btn_action;
    bool online = true;

    ~Impl() { detail::delete_if_orphan(frame); }

    void update_status() {
        if (!status_dot) return;
        status_dot->setStyleSheet(online ? QStringLiteral("background-color: #10b981; border-radius: 4px;")
                                         : QStringLiteral("background-color: #71717a; border-radius: 4px;"));
        status_dot->setToolTip(online ? QStringLiteral("Online") : QStringLiteral("Offline"));
    }
};

UserProfileCard::UserProfileCard(const std::string& name, const std::string& handle, const std::string& role,
                                 const std::string& bio, bool is_online, const std::string& action_label)
    : pimpl(std::make_shared<Impl>()) {
    QFrame* frame = pimpl->frame;
    frame->setProperty("sg_role", QStringLiteral("profile_card"));
    detail::set_base_style(frame, QStringLiteral(
        "QFrame[sg_role=\"profile_card\"] { background-color: #1a1a1e; border: 1px solid #27272a; border-radius: 10px; }"));
    frame->setMinimumWidth(260);

    auto* layout = new QVBoxLayout(frame);
    layout->setContentsMargins(12, 12, 12, 12);
    layout->setSpacing(8);

    auto* header = new QHBoxLayout();
    header->setSpacing(10);

    // First character of the name (works for any language, not just ASCII).
    const QString qname = detail::qs(name).trimmed();
    QString initial = QStringLiteral("U");
    if (!qname.isEmpty()) {
        const int len = qname.at(0).isHighSurrogate() && qname.size() > 1 ? 2 : 1;
        initial = qname.left(len).toUpper();
    }
    QLabel* avatar = detail::plain_label("", frame);
    avatar->setText(initial);
    avatar->setFixedSize(40, 40);
    avatar->setAlignment(Qt::AlignCenter);
    avatar->setStyleSheet(QStringLiteral(
        "background-color: #2563eb; color: #ffffff; font-weight: bold; font-size: 16px; border-radius: 20px;"));

    auto* name_box = new QVBoxLayout();
    name_box->setSpacing(2);
    auto* name_row = new QHBoxLayout();
    name_row->setSpacing(6);
    QLabel* lbl_name = detail::plain_label(name, frame);
    lbl_name->setStyleSheet(QStringLiteral("color: #f4f4f5; font-size: 14px; font-weight: bold; background: transparent;"));
    pimpl->status_dot = new QLabel(frame);
    pimpl->status_dot->setFixedSize(8, 8);
    name_row->addWidget(lbl_name);
    name_row->addWidget(pimpl->status_dot);
    name_row->addStretch();

    QString subtitle;
    if (!handle.empty()) subtitle = (handle.front() == '@' ? QString() : QStringLiteral("@")) + detail::qs(handle);
    if (!role.empty()) {
        if (!subtitle.isEmpty()) subtitle += QStringLiteral(" \u00B7 ");  // middle dot
        subtitle += detail::qs(role);
    }
    QLabel* lbl_handle = detail::plain_label("", frame);
    lbl_handle->setText(subtitle);
    lbl_handle->setStyleSheet(QStringLiteral("color: #a1a1aa; font-size: 11px; background: transparent;"));
    name_box->addLayout(name_row);
    name_box->addWidget(lbl_handle);

    header->addWidget(avatar);
    header->addLayout(name_box);
    header->addStretch();

    pimpl->lbl_bio = detail::plain_label(bio, frame);
    pimpl->lbl_bio->setWordWrap(true);
    pimpl->lbl_bio->setStyleSheet(QStringLiteral("color: #d4d4d8; font-size: 12px; background: transparent;"));

    pimpl->btn_action = new QPushButton(detail::qs(action_label), frame);
    pimpl->btn_action->setCursor(Qt::PointingHandCursor);
    pimpl->btn_action->setStyleSheet(QStringLiteral(
        "QPushButton { background-color: #27272a; color: #f4f4f5; border: 1px solid #3f3f46;"
        "  border-radius: 6px; padding: 6px; font-weight: 500; }"
        "QPushButton:hover { background-color: #3f3f46; }"));

    layout->addLayout(header);
    layout->addWidget(pimpl->lbl_bio);
    layout->addWidget(pimpl->btn_action);

    set_online_status(is_online);
}

UserProfileCard::~UserProfileCard() = default;

void UserProfileCard::set_online_status(bool is_online) {
    pimpl->online = is_online;
    pimpl->update_status();
}

bool UserProfileCard::is_online() const { return pimpl->online; }

void UserProfileCard::set_bio(const std::string& bio) {
    if (pimpl->lbl_bio) pimpl->lbl_bio->setText(detail::qs(bio));
}

void UserProfileCard::set_action_text(const std::string& text) {
    if (pimpl->btn_action) pimpl->btn_action->setText(detail::qs(text));
}

EventConnection UserProfileCard::on_action(std::function<void()> handler) {
    if (!pimpl->btn_action || !handler) return {};
    return detail::wrap(QObject::connect(pimpl->btn_action.data(), &QPushButton::clicked,
                                         [handler = std::move(handler)]() { handler(); }));
}

QWidget* UserProfileCard::get_qwidget() const { return pimpl->frame.data(); }

}  // namespace simplegui
