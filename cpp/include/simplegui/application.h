#pragma once
#include <memory>
#include <string>
#include <vector>

namespace simplegui {

// Every SimpleGUI program creates exactly one Application first, then windows,
// then calls run().  (Delphi/Lazarus: the global Application object.)
//
//   int main(int argc, char** argv) {
//       simplegui::Application app(argc, argv);
//       ... create windows ...
//       return app.run();
//   }
class Application {
public:
    Application(int& argc, char** argv);
    ~Application();

    // Starts the program and waits until the last window closes (or quit() is called).
    int run();

    // Changes the look of the whole program. Returns false for an unknown name.
    // Names: see available_themes(), e.g. "dark", "light", "neon", "windows", "gnome", "kde".
    bool set_theme(const std::string& theme_name);
    std::string theme() const;                         // the last theme that was applied
    static std::vector<std::string> available_themes();

    // Advanced: apply your own Qt style sheet (CSS-like text) to the whole program.
    void set_stylesheet(const std::string& qss);

    void set_app_name(const std::string& name);        // used in dialogs and the task bar
    void set_font(const std::string& family, int point_size = 0); // default font for everything

    // Ends run() from anywhere, like Delphi's Application.Terminate.
    static void quit(int exit_code = 0);
    // Lets the screen refresh during a long loop (like VB's DoEvents).
    static void process_events();

private:
    struct Impl;
    std::unique_ptr<Impl> pimpl;
};

}  // namespace simplegui
