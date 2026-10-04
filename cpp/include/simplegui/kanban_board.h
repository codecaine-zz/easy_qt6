#pragma once
#include "simplegui/control.h"
#include "simplegui/event_connection.h"
#include <functional>
#include <memory>
#include <string>
#include <vector>

namespace simplegui {

// A task board with columns (e.g. "To do", "Doing", "Done") holding cards.
// Columns and cards are identified by IDs you choose, such as "todo" or "task-42".
class KanbanBoard : public Control {
public:
    KanbanBoard();
    ~KanbanBoard() override;

    // Returns false if the ID is empty or already used.
    bool add_column(const std::string& column_id, const std::string& title);
    bool add_card(const std::string& column_id,
                  const std::string& card_id,
                  const std::string& title,
                  const std::string& tag = "",
                  const std::string& description = "");
    // Returns false (and leaves the card where it is) if the card or column is unknown.
    bool move_card(const std::string& card_id, const std::string& target_column_id);
    bool remove_card(const std::string& card_id);
    void clear_column(const std::string& column_id);

    std::vector<std::string> card_ids(const std::string& column_id) const;  // top to bottom
    std::string card_column(const std::string& card_id) const;              // "" if unknown

    // Runs when the user clicks a card.
    EventConnection on_card_clicked(std::function<void(const std::string& card_id)> handler);

    QWidget* get_qwidget() const override;

private:
    struct Impl;
    std::shared_ptr<Impl> pimpl;
};

}  // namespace simplegui
