#include "text_editor.hpp"

#include <fstream>
#include <iomanip>
#include <iostream>
#include <sstream>

#include "ftxui/component/event.hpp"
#include "ftxui/component/screen_interactive.hpp"
#include "spdlog/spdlog.h"

using namespace ftxui;

std::string AlignNumberLeft1(int number, size_t width) {
    std::ostringstream oss;
    oss << std::setw(width) << std::right << number;
    return oss.str();
}

struct TextBufferImpl {
    std::string& content;
    int cursor_offset = 0;

    TextBufferImpl(std::string& content_) : content(content_) {}

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

    std::pair<int, int> CursorPosition() const {
        auto lines = SplitLines();
        size_t line = 0;
        int col = cursor_offset;
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

        cursor_offset = 0;
        for (int i = 0; i < x; ++i) {
            cursor_offset += lines[i].size() + 1;
        }
        cursor_offset += y;
    }

    void MoveCursorLeft() { cursor_offset = std::max(0, cursor_offset - 1); }

    void MoveCursorRight() { cursor_offset = std::min(static_cast<int>(content.size()), cursor_offset + 1); }

    void MoveCursorUp() { MoveCursorVertically(-1); }

    void MoveCursorDown() { MoveCursorVertically(+1); }

    void MoveCursorVertically(int distance) {
        auto lines = SplitLines();
        auto [line, col] = CursorPosition();
        int new_line = line + distance;
        if (new_line < 0 || new_line >= static_cast<int>(lines.size()))
            return;
        cursor_offset = 0;
        for (int i = 0; i < new_line; ++i) {
            cursor_offset += lines[i].size() + 1;
        }
        cursor_offset += std::min(col, static_cast<int>(lines[new_line].size()));
    }

    void MoveToLineStart() {
        auto lines = SplitLines();
        auto [line, _] = CursorPosition();
        cursor_offset = 0;
        for (int i = 0; i < (int)line; ++i) {
            cursor_offset += lines[i].size() + 1;
        }
    }

    void MoveToLineEnd() {
        auto lines = SplitLines();
        auto [line, _] = CursorPosition();
        cursor_offset = 0;
        for (int i = 0; i < (int)line; ++i) {
            cursor_offset += lines[i].size() + 1;
        }
        cursor_offset += lines[line].size();
    }

    void InsertChar(char c) { content.insert(cursor_offset++, 1, c); }

    void Backspace() {
        if (cursor_offset > 0) {
            content.erase(cursor_offset - 1, 1);
            cursor_offset--;
        }
    }

    void Delete() {
        if (cursor_offset < (int)content.size()) {
            content.erase(cursor_offset, 1);
        }
    }
};
class TextEditorImpl : public ComponentBase {
   public:
    TextEditorImpl(std::string& content, bool& changed)
        : buffer_(content), height(10), width(10), is_changed_(changed) {
        width = 0;
        height = 0;
    }

    bool Focusable() const final { return true; }

    Element PrepareLineNumbers(int cursor_y) {
        auto lines = buffer_.SplitLines();
        Elements line_numbers;

        int last_visible_line = std::min(scroll_y + height, static_cast<int>(lines.size()));

        for (int i = scroll_y; i < last_visible_line; ++i) {
            auto line_number = text(AlignNumberLeft1(i + 1, 3));
            if (cursor_y != i) {
                line_number |= dim;
            }
            line_numbers.push_back(line_number);
        }

        return vbox(line_numbers);
    }
    Element PrepareTextLines(int cursor_y, int cursor_x) {
        auto lines = buffer_.SplitLines();
        Elements text_lines;

        int last_visible_line = std::min(scroll_y + height, static_cast<int>(lines.size()));

        for (int i = scroll_y; i < last_visible_line; ++i) {
            std::string visible_text;
            if (scroll_x < static_cast<int>(lines[i].size())) {
                visible_text = lines[i].substr(scroll_x, std::min(width, static_cast<int>(lines[i].size()) - scroll_x));
            }

            Element line_text;
            if (i == cursor_y) {
                int visible_cursor_col = cursor_x - scroll_x;

                if (visible_cursor_col < 0) {
                    line_text = text(" ") | inverted;
                }

                else if (visible_cursor_col >= static_cast<int>(visible_text.size())) {
                    if (visible_text.empty()) {
                        line_text = hbox({text(" ") | inverted, text("")});
                    } else {
                        line_text = hbox({text(visible_text), text(" ") | inverted});
                    }
                }

                else {
                    line_text = hbox({text(visible_text.substr(0, visible_cursor_col)),
                                      text(std::string(1, visible_text[visible_cursor_col])) | inverted,
                                      text(visible_text.substr(visible_cursor_col + 1))});
                }
            } else {
                line_text = text(visible_text.empty() ? " " : visible_text);
            }

            text_lines.push_back(line_text);
        }

        return vbox(text_lines) | flex | reflect(text_box_);
    }

    Element CreateScrollIndicator(int total_lines, int visible_height, int scroll_pos) {
        if (total_lines <= visible_height)
            return text("");

        float thumb_position = (float)scroll_pos / (total_lines - visible_height);
        return vbox({
                   filler() | size(HEIGHT, EQUAL, thumb_position * visible_height),
                   text("▐"),
                   filler(),
               }) |
               size(WIDTH, EQUAL, 1) | frame;
    }

    // В private-секции класса TextEditorImpl:
    

    // Вычисляем максимальную ширину строки
    void UpdateMaxLineWidth() {
        auto lines = buffer_.SplitLines();
        max_line_width_ = 0;
        for (const auto& line : lines) {
            max_line_width_ = std::max(max_line_width_, static_cast<int>(line.size()));
        }
    }

    // Создаём горизонтальный индикатор
    Element CreateHScrollIndicator(int total_width, int visible_width, int scroll_pos) {
        if (total_width <= visible_width)
            return text("");

        float thumb_position = (float)scroll_pos / (total_width - visible_width);
        float thumb_width = std::max(5.f, (float)visible_width / total_width * visible_width);

        return hbox({
                   filler() | size(WIDTH, EQUAL, thumb_position * visible_width),
                   text("▁▁▁") | size(WIDTH, EQUAL, thumb_width),
                   filler(),
               }) |
               size(HEIGHT, EQUAL, 1) | frame;
    }

    ftxui::Element OnRender() {
        auto lines = buffer_.SplitLines();
        auto [cursor_y, cursor_x] = buffer_.CursorPosition();

        if (cursor_y < scroll_y) {
            scroll_y = cursor_y;
        } else if (cursor_y >= scroll_y + height) {
            scroll_y = cursor_y - height + 1;
        }

        if (cursor_x < scroll_x) {
            scroll_x = cursor_x;
        } else if (cursor_x >= scroll_x + width) {
            scroll_x = cursor_x - width + 1;
        }

        UpdateMaxLineWidth();

        scroll_y = std::max(0, std::min(scroll_y, static_cast<int>(lines.size()) - height));
        scroll_x = std::max(0, scroll_x);

        auto line_numbers_column = PrepareLineNumbers(cursor_y);
        auto text_column = PrepareTextLines(cursor_y, cursor_x);

        auto vscroll_bar = CreateScrollIndicator(lines.size(), height, scroll_y);
        auto hscroll_bar = CreateHScrollIndicator(max_line_width_, width, scroll_x);

        

        auto element = vbox({
            // Основное содержимое + вертикальный скролл
            hbox({
                line_numbers_column,
                separator(),
                text_column | flex,
                vscroll_bar
            }) | flex,
            
            // Горизонтальный скролл
            hbox({
                filler(),
                hscroll_bar | size(WIDTH, EQUAL, width) | flex
            })
        });

        if (text_box_.y_max >= text_box_.y_min) {
            height = text_box_.y_max - text_box_.y_min + 1;
        }
        if (text_box_.x_max >= text_box_.x_min) {
            width = text_box_.x_max - text_box_.x_min;
            width = std::max(1, width);
        }

        return element;
    }

    // Остальные методы остаются без изменений
    bool OnEvent(Event event) override {
        if (event == Event::ArrowLeft) {
            buffer_.MoveCursorLeft();
            return true;
        }
        if (event == Event::ArrowRight) {
            buffer_.MoveCursorRight();
            return true;
        }
        if (event == Event::ArrowUp) {
            buffer_.MoveCursorUp();
            return true;
        }
        if (event == Event::ArrowDown) {
            buffer_.MoveCursorDown();
            return true;
        }
        if (event == Event::Backspace) {
            buffer_.Backspace();
            is_changed_ = true;
            return true;
        }
        if (event == Event::Delete) {
            buffer_.Delete();
            is_changed_ = true;
            return true;
        }
        if (event == Event::Return) {
            buffer_.InsertChar('\n');
            is_changed_ = true;
            return true;
        }
        if (event.is_character()) {
            buffer_.InsertChar(event.character()[0]);
            is_changed_ = true;
            return true;
        }

        if (event.is_mouse()) {
            if (event.mouse().button == Mouse::Left && event.mouse().motion == Mouse::Pressed) {
                if (text_box_.Contain(event.mouse().x, event.mouse().y)) {
                    int y = event.mouse().y - text_box_.y_min;
                    int x = event.mouse().x - text_box_.x_min;

                    // Adjust for scroll position
                    y += scroll_y;
                    x += scroll_x;

                    buffer_.MoveCursorToClosestPosition(y, x);
                    return true;
                }
            }
            if (event.mouse().button == Mouse::Button::WheelUp) {
                buffer_.MoveCursorUp();
                return true;
            }
            if (event.mouse().button == Mouse::Button::WheelDown) {
                buffer_.MoveCursorDown();
                return true;
            }
        }

        return ComponentBase::OnEvent(event);
    }

   private:
    TextBufferImpl buffer_;
    ftxui::Box text_box_;
    int scroll_y = 0;
    int height;
    int scroll_x = 0;
    int width;
    int max_line_width_ = 0;  
    bool& is_changed_;
};

ftxui::Component TextEditor(std::string& content, bool& changed) {
    return Make<TextEditorImpl>(content, changed);
};
