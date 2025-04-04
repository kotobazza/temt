#include "text_writer.hpp"

#include <fstream>
#include "app_data.hpp"
#include "ftxui/component/event.hpp"

using namespace ftxui;

class TextWriterImpl : public ComponentBase {
   public:
    TextWriterImpl(temt::AppData& appData, std::function<void()> exitClosure) : exitClosure_(exitClosure) {
        temt::FileManip::FileInfo dirEntry = appData.selectedEntry();
        std::string filepath = temt::FileManip::assemblePath(dirEntry.parentDirectory, dirEntry.path);
        is_opened_file_ = LoadFile(filepath);
        appData.file_logger_->info("Created TextWriter: index in parent {}", Index());
        appData.file_logger_->flush();
    }

    bool LoadFile(const std::string& filepath) {
        std::ifstream file(filepath);
        if (!file.is_open()) {
            return false;
        }

        original_filepath = filepath;
        text_content.clear();
        std::string line;
        while (std::getline(file, line)) {
            text_content += line + "\n";
        }
        file.close();

        if (!text_content.empty() && text_content.back() == '\n') {
            text_content.pop_back();
        }

        cursor_pos = 0;
        return true;
    }

    bool SaveFile() {
        if (original_filepath.empty()) {
            return false;
        }

        std::ofstream file(original_filepath);
        if (!file.is_open()) {
            return false;
        }

        file << text_content;
        file.close();
        return true;
    }

    Element OnRender() override {
        if (!is_opened_file_) {
            return vbox(text("TextWriter:Selected file is not a character file")) | center;
        }
        std::vector<std::string> lines = SplitLines();

        auto [cursor_line, cursor_col] = GetCursorPosition(lines);

        Elements text_lines;
        for (size_t i = 0; i < lines.size(); ++i) {
            auto line_number = text(std::to_string(i + 1) + " ") | dim;

            if (i == cursor_line) {
                std::string before_cursor = lines[i].substr(0, cursor_col);
                std::string at_cursor = "";
                if (cursor_col < (int)lines[i].size()) {
                    at_cursor = std::string(1, lines[i][cursor_col]);
                } else if (cursor_col == (int)lines[i].size() && i < lines.size() - 1) {
                    at_cursor = " ";
                }
                std::string after_cursor = lines[i].substr(cursor_col + (at_cursor.empty() ? 0 : 1));

                auto line_text = hbox({text(before_cursor), text(at_cursor) | inverted, text(after_cursor)});
                text_lines.push_back(hbox({line_number, line_text}));
            } else {
                auto line_text = text(lines[i]);
                text_lines.push_back(hbox({line_number, line_text}));
            }
        }

        auto editor = vbox(text_lines) | frame | flex;

        return editor;
    }

    bool Focusable() const final{return true;}

    bool OnEvent(Event event) override {
        if (event == Event::CtrlS) {
            return SaveFile();
        }
        if (event == Event::CtrlQ) {
            exitClosure_();
            return true;
        }

        // Обработка перемещения курсора
        if (event == Event::ArrowLeft) {
            cursor_pos = std::max(0, cursor_pos - 1);
            return true;
        }
        if (event == Event::ArrowRight) {
            cursor_pos = std::min((int)text_content.size(), cursor_pos + 1);
            return true;
        }
        if (event == Event::ArrowUp) {
            MoveCursorVertical(-1);
            return true;
        }
        if (event == Event::ArrowDown) {
            MoveCursorVertical(1);
            return true;
        }
        if (event == Event::Home) {
            MoveCursorToLineStart();
            return true;
        }
        if (event == Event::End) {
            MoveCursorToLineEnd();
            return true;
        }

        // Обработка ввода текста

        if (event == Event::Backspace) {
            if (cursor_pos > 0 && !text_content.empty()) {
                text_content.erase(cursor_pos - 1, 1);
                cursor_pos--;
            }
            return true;
        }
        if (event == Event::Delete) {
            if (cursor_pos < (int)text_content.size()) {
                text_content.erase(cursor_pos, 1);
            }
            return true;
        }
        if (event == Event::Return) {
            text_content.insert(cursor_pos, 1, '\n');
            cursor_pos++;
            return true;
        }

        if (event.is_character()) {
            text_content.insert(cursor_pos, event.character());
            cursor_pos++;
            return true;
        }

        return ComponentBase::OnEvent(event);
    }

   private:
    std::string text_content;
    std::string original_filepath;
    bool is_opened_file_ = false;
    int cursor_pos = 0;
    std::function<void()> exitClosure_;

    std::vector<std::string> SplitLines() const {
        std::vector<std::string> lines;
        size_t start = 0;
        size_t end = text_content.find('\n');
        while (end != std::string::npos) {
            lines.push_back(text_content.substr(start, end - start));
            start = end + 1;
            end = text_content.find('\n', start);
        }
        lines.push_back(text_content.substr(start));
        return lines;
    }

    std::pair<size_t, int> GetCursorPosition(const std::vector<std::string>& lines) const {
        size_t cursor_line = 0;
        int cursor_col = cursor_pos;
        for (size_t i = 0; i < lines.size(); ++i) {
            if (cursor_col <= (int)lines[i].size()) {
                cursor_line = i;
                break;
            }
            cursor_col -= lines[i].size() + 1;
        }
        return {cursor_line, cursor_col};
    }

    void MoveCursorVertical(int direction) {
        auto lines = SplitLines();
        auto [current_line, current_col] = GetCursorPosition(lines);

        int new_line = current_line + direction;
        if (new_line < 0 || new_line >= (int)lines.size()) {
            return;
        }

        // Вычисляем новую позицию курсора
        int new_pos = 0;
        for (int i = 0; i < new_line; ++i) {
            new_pos += lines[i].size() + 1;
        }

        // Ограничиваем столбец длиной новой строки
        int new_col = std::min(current_col, (int)lines[new_line].size());
        // Если мы на пустой строке, курсор должен быть в позиции 0
        if (lines[new_line].empty()) {
            new_col = 0;
        }
        cursor_pos = new_pos + new_col;
    }

    void MoveCursorToLineStart() {
        auto lines = SplitLines();
        auto [current_line, _] = GetCursorPosition(lines);

        cursor_pos = 0;
        for (int i = 0; i < static_cast<int>(current_line); ++i) {
            cursor_pos += lines[i].size() + 1;
        }
    }

    void MoveCursorToLineEnd() {
        auto lines = SplitLines();
        auto [current_line, _] = GetCursorPosition(lines);

        cursor_pos = 0;
        for (int i = 0; i < static_cast<int>(current_line); ++i) {
            cursor_pos += lines[i].size() + 1;
        }
        cursor_pos += lines[current_line].size();
    }
};

ftxui::Component TextWriter(temt::AppData& appData, std::function<void()> exitClosure){
    return Make<TextWriterImpl>(appData, exitClosure);
};