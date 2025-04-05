#include "text_writer.hpp"

#include <fstream>
#include "app_data.hpp"
#include "ftxui/component/event.hpp"

using namespace ftxui;

struct TextBuffer {
    std::string content;
    int cursor_pos = 0;

    std::vector<std::string> SplitLines() const {
        std::vector<std::string> lines;
        size_t start = 0;
        size_t end = content.find('\n');
        while (end != std::string::npos) {
            lines.push_back(content.substr(start, end - start));
            start = end + 1;
            end = content.find('\n', start);
        }
        lines.push_back(content.substr(start));
        return lines;
    }

    std::pair<int, int> CursorLineCol() const {
        auto lines = SplitLines();
        size_t line = 0;
        int col = cursor_pos;
        for (size_t i = 0; i < lines.size(); ++i) {
            if (col <= (int)lines[i].size()) {
                line = i;
                break;
            }
            col -= lines[i].size() + 1;
        }
        return {static_cast<int>(line), col};
    }

    void MoveCursorToClosestPosition(int x, int y) {
        auto lines = SplitLines();

        x = std::min(static_cast<int>(lines.size()), x);
        y = std::min(static_cast<int>(lines[x].size()), y);

        cursor_pos = 0;
        for (int i = 0; i < x; ++i) {
            cursor_pos += lines[i].size() + 1;
        }
        cursor_pos += y;
    }

    void MoveCursorLeft() { cursor_pos = std::max(0, cursor_pos - 1); }

    void MoveCursorRight() { cursor_pos = std::min(static_cast<int>(content.size()), cursor_pos + 1); }

    void MoveCursorUp() {
        auto lines = SplitLines();
        auto [line, col] = CursorLineCol();
        int new_line = line - 1;
        if (new_line < 0 || new_line >= static_cast<int>(lines.size()))
            return;
        cursor_pos = 0;
        for (int i = 0; i < new_line; ++i) {
            cursor_pos += lines[i].size() + 1;
        }
        cursor_pos += std::min(col, static_cast<int>(lines[new_line].size()));
    }

    void MoveCursorDown() {
        auto lines = SplitLines();
        auto [line, col] = CursorLineCol();
        int new_line = line + 1;
        if (new_line < 0 || new_line >= static_cast<int>(lines.size()))
            return;
        cursor_pos = 0;
        for (int i = 0; i < new_line; ++i) {
            cursor_pos += lines[i].size() + 1;
        }
        cursor_pos += std::min(col, static_cast<int>(lines[new_line].size()));
    }

    void MoveToLineStart() {
        auto lines = SplitLines();
        auto [line, _] = CursorLineCol();
        cursor_pos = 0;
        for (int i = 0; i < (int)line; ++i) {
            cursor_pos += lines[i].size() + 1;
        }
    }

    void MoveToLineEnd() {
        auto lines = SplitLines();
        auto [line, _] = CursorLineCol();
        cursor_pos = 0;
        for (int i = 0; i < (int)line; ++i) {
            cursor_pos += lines[i].size() + 1;
        }
        cursor_pos += lines[line].size();
    }

    void InsertChar(char c) { content.insert(cursor_pos++, 1, c); }

    void InsertNewLine() { content.insert(cursor_pos++, 1, '\n'); }

    void Backspace() {
        if (cursor_pos > 0) {
            content.erase(cursor_pos - 1, 1);
            cursor_pos--;
        }
    }

    void Delete() {
        if (cursor_pos < (int)content.size()) {
            content.erase(cursor_pos, 1);
        }
    }

    bool LoadFromFile(const std::string& path) {
        std::ifstream file(path);
        if (!file.is_open())
            return false;

        content.clear();
        std::string line;
        while (std::getline(file, line)) {
            content += line + "\n";
        }
        file.close();
        cursor_pos = 0;
        return true;
    }

    bool SaveToFile(const std::string& path) const {
        std::ofstream file(path);
        if (!file.is_open())
            return false;

        file << content;
        file.close();
        return true;
    }
};

class TextWriterImpl : public ComponentBase {
   public:
    TextWriterImpl(temt::AppData& appData, std::function<void()> exitClosure) : exitClosure_(exitClosure) {
        temt::FileManip::FileInfo dirEntry = appData.selectedEntry();
        std::string filepath = temt::FileManip::assemblePath(dirEntry.parentDirectory, dirEntry.path);

        if (textBuffer_.LoadFromFile(filepath)) {
            appData.file_logger_->info("Loaded file: {}", filepath);
            is_opened_file_ = true;

        } else {
            appData.file_logger_->info("Failed to load file: {}", filepath);
            is_opened_file_ = false;
        }

        textSource_ = Renderer([this]() {
            auto lines = textBuffer_.SplitLines();
            Elements text_lines;
            for (size_t i = 0; i < lines.size(); ++i) {
                auto line_number = text(std::to_string(i + 1) + " ") | dim;
                auto line_text = text(lines[i]);
                text_lines.push_back(hbox({line_number, line_text}));
            }

            return vbox(text_lines) | flex;
        });
    }

    bool Focusable() const final { return true; }

    Element OnRender() override final {
        if (!is_opened_file_) {
            return vbox(text("Failed to load file"), text(original_filepath) | underlined) | center;
        }

        return vbox({textSource_->Render() | vscroll_indicator | yframe | flex | reflect(textSourceBox_)});
    }

    bool OnEvent(Event event) {
        if (event == Event::ArrowLeft) {
            textBuffer_.MoveCursorLeft();
            return true;
        }
        if (event == Event::ArrowRight) {
            textBuffer_.MoveCursorRight();
            return true;
        }
        if (event == Event::ArrowUp) {
            textBuffer_.MoveCursorUp();
            return true;
        }
        if (event == Event::ArrowDown) {
            textBuffer_.MoveCursorDown();
            return true;
        }
        if (event == Event::Backspace) {
            textBuffer_.Backspace();
            return true;
        }
        if (event == Event::Delete) {
            textBuffer_.Delete();
            return true;
        }
        if (event == Event::Return) {
            textBuffer_.InsertNewLine();
            return true;
        }
        if (event == Event::CtrlS) {
            textBuffer_.SaveToFile(original_filepath);
            return true;
        }
        if (event == Event::CtrlQ) {
            exitClosure_();
            return true;
        }
        if(event.is_character()){
            textBuffer_.InsertChar(event.character()[0]);
            return true;
        }

        if (event.is_mouse() && event.mouse().button == Mouse::Left) {
            if (event.mouse().motion == Mouse::Pressed && textSourceBox_.Contain(event.mouse().x, event.mouse().y)) {
                textBuffer_.MoveCursorToClosestPosition(event.mouse().x, event.mouse().y);
                return true;
            }
        }

        return ComponentBase::OnEvent(event);
    }

   private:
    std::function<void()> exitClosure_;
    TextBuffer textBuffer_;
    std::string original_filepath;
    ftxui::Component textSource_;
    ftxui::Box textSourceBox_;
    bool is_opened_file_ = false;
};

ftxui::Component TextWriter(temt::AppData& appData, std::function<void()> exitClosure) {
    return Make<TextWriterImpl>(appData, exitClosure);
};