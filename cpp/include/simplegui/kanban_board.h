#pragma once
#include "simplegui/control.h"
#include "simplegui/event_connection.h"
#include <memory>
#include <string>
#include <vector>
#include <functional>

namespace simplegui {

class KanbanBoard : public Control {
public:
    KanbanBoard();
    ~KanbanBoard() override;

    void add_column(const std::string& column_id, const std::string& title);
    void add_card(const std::string& column_id,
                  const std::string& card_id,
                  const std::string& title,
                  const std::string& tag,
                  const std::string& description);
    void move_card(const std::string& card_id, const std::string& target_column_id);
    void clear_column(const std::string& column_id);

    EventConnection on_card_clicked(std::function<void(const std::string& card_id)> handler);

    QWidget* get_qwidget() const override;

private:
    struct Impl;
    std::shared_ptr<Impl> pimpl;
};

}
