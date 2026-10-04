#include "simplegui/user_profile_card.h"
#include <QFrame>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QPointer>
#include <QString>

namespace simplegui {

struct UserProfileCard::Impl {
    QPointer<QFrame> frame;
    QLabel* status_dot;
    QLabel* lbl_bio;
    QPushButton* btn_action;

    Impl(const std::string& name, const std::string& handle, const std::string& role, const std::string& bio, bool is_online, const std::string& action_label) {
        frame = new QFrame();
        frame->setStyleSheet(
            "QFrame {"
            "  background-color: #1a1a1e;"
            "  border: 1px solid #27272a;"
            "  border-radius: 10px;"
            "}"
        );
        frame->setMinimumWidth(260);

        QVBoxLayout* layout = new QVBoxLayout(frame);
        layout->setContentsMargins(12, 12, 12, 12);
        layout->setSpacing(8);

        // Header: Avatar Circle + Names
        QHBoxLayout* header = new QHBoxLayout();
        header->setSpacing(10);

        QLabel* avatar = new QLabel();
        avatar->setFixedSize(40, 40);
        avatar->setStyleSheet("background-color: #2563eb; color: #ffffff; font-weight: bold; font-size: 16px; border-radius: 20px; border: none; qproperty-alignment: AlignCenter;");
        std::string initials = name.empty() ? "U" : name.substr(0, 1);
        avatar->setText(QString::fromStdString(initials));

        QVBoxLayout* name_box = new QVBoxLayout();
        name_box->setSpacing(2);

        QHBoxLayout* name_row = new QHBoxLayout();
        name_row->setSpacing(6);
        QLabel* lbl_name = new QLabel(QString::fromStdString(name));
        lbl_name->setStyleSheet("color: #f4f4f5; font-size: 14px; font-weight: bold; border: none; background: transparent;");

        status_dot = new QLabel();
        status_dot->setFixedSize(8, 8);
        update_status(is_online);

        name_row->addWidget(lbl_name);
        name_row->addWidget(status_dot);
        name_row->addStretch();

        std::string formatted_handle = handle.empty() ? "" : (handle.front() == '@' ? handle : "@" + handle);
        std::string subtitle = formatted_handle.empty() ? role : (role.empty() ? formatted_handle : formatted_handle + " · " + role);
        QLabel* lbl_handle = new QLabel(QString::fromStdString(subtitle));
        lbl_handle->setStyleSheet("color: #a1a1aa; font-size: 11px; border: none; background: transparent;");

        name_box->addLayout(name_row);
        name_box->addWidget(lbl_handle);

        header->addWidget(avatar);
        header->addLayout(name_box);
        header->addStretch();

        lbl_bio = new QLabel(QString::fromStdString(bio));
        lbl_bio->setWordWrap(true);
        lbl_bio->setStyleSheet("color: #d4d4d8; font-size: 12px; border: none; background: transparent;");

        btn_action = new QPushButton(QString::fromStdString(action_label));
        btn_action->setStyleSheet("background-color: #27272a; color: #f4f4f5; border: 1px solid #3f3f46; border-radius: 6px; padding: 6px; font-weight: 500;");

        layout->addLayout(header);
        layout->addWidget(lbl_bio);
        layout->addWidget(btn_action);
    }
    ~Impl() { if (frame && !frame->parent()) delete frame; }

    void update_status(bool is_online) {
        if (status_dot) {
            QString col = is_online ? "#10b981" : "#71717a";
            status_dot->setStyleSheet(QString("background-color: %1; border-radius: 4px; border: none;").arg(col));
        }
    }
};

UserProfileCard::UserProfileCard(const std::string& name, const std::string& handle, const std::string& role, const std::string& bio, bool is_online, const std::string& action_label)
    : pimpl(std::make_shared<Impl>(name, handle, role, bio, is_online, action_label)) {}

UserProfileCard::~UserProfileCard() = default;

void UserProfileCard::set_online_status(bool is_online) {
    pimpl->update_status(is_online);
}

void UserProfileCard::set_bio(const std::string& bio) {
    if (pimpl->lbl_bio) {
        pimpl->lbl_bio->setText(QString::fromStdString(bio));
    }
}

EventConnection UserProfileCard::on_action(std::function<void()> handler) {
    if (pimpl->btn_action) {
        auto conn = QObject::connect(pimpl->btn_action, &QPushButton::clicked, [handler]() {
            handler();
        });
        return EventConnection([conn]() { QObject::disconnect(conn); });
    }
    return EventConnection();
}

QWidget* UserProfileCard::get_qwidget() const {
    return pimpl->frame.data();
}

}
