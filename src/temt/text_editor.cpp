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

    void MoveCursorUp() {
        MoveCursorVertically(-1);
    }

    void MoveCursorDown() {
        MoveCursorVertically(+1);
    }

    void MoveCursorVertically(int distance){
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
    TextEditorImpl(std::string& content, int& parent_width, int& parent_height) : buffer_(content), height(parent_height), width(parent_width) {
        
    }

    bool Focusable() const final { return true; }

    ftxui::Element OnRender() {
        auto file_logger = spdlog::get("file_logger");
        file_logger->info("Editor's sizes: {}, {}", width, height);
        auto lines = buffer_.SplitLines();
        auto [cusror_y, cusror_x] = buffer_.CursorPosition();

        // Calculate visible lines range based on cursor position (vertical scroll)
        if (scroll_y > cusror_y) {
            scroll_y = cusror_y;
        } else if (cusror_y >= scroll_y + height) {
            scroll_y = cusror_y - height + 1;
        }

        // Calculate horizontal scroll based on cursor position
        if (cusror_x < scroll_x) {
            scroll_x = cusror_x;
        } else if (cusror_x >= scroll_x + width) {
            scroll_x = cusror_x - width + 1;
        }

        // Ensure scroll positions are within bounds
        scroll_y = std::max(0, std::min(scroll_y, static_cast<int>(lines.size()) - height));
        scroll_x = std::max(0, scroll_x);

        // Calculate the last visible line
        int last_visible_line = std::min(scroll_y + height, static_cast<int>(lines.size()));

        // Prepare line numbers column
        Elements line_numbers;
        for (int i = scroll_y; i < last_visible_line; ++i) {
            auto line_number = text(AlignNumberLeft1(i + 1, 3));
            if (cusror_y != i) {
                line_number |= dim;
            }
            line_numbers.push_back(line_number);
        }

        // Prepare text lines with horizontal scrolling
        Elements text_lines;
        for (int i = scroll_y; i < last_visible_line; ++i) {
            std::string visible_text;
            if (scroll_x < static_cast<int>(lines[i].size())) {
                visible_text = lines[i].substr(
                    scroll_x,
                    std::min(width, static_cast<int>(lines[i].size()) - scroll_x)
                );
            }

            Element line_text;
            if (i == cusror_y) {
                int visible_cursor_col = cusror_x - scroll_x;
                if (visible_cursor_col < 0) {
                    // Cursor is outside visible area, show empty inverted space
                    line_text = text(" ") | inverted;
                } else if(visible_cursor_col >= static_cast<int>(visible_text.size())){
                    line_text = hbox({
                        text(visible_text.substr(0, visible_text.size()-1)),
                        text(" ") | inverted
                    });
                } else {
                    line_text = hbox({
                        text(visible_text.substr(0, visible_cursor_col)),
                        text(std::string(1, visible_text[visible_cursor_col])) | inverted,
                        text(visible_text.substr(visible_cursor_col + 1))
                    });
                }
            } else {
                line_text = text(visible_text);
            }

            text_lines.push_back(line_text);
        }

        auto line_numbers_column = vbox(line_numbers);
        auto text_column = vbox(text_lines) | flex;

        auto element = hbox({
            line_numbers_column,
            separator(),
            text_column
        }) | reflect(textSourceBox_);

        // Update visible area size based on actual box size
        if (textSourceBox_.y_max >= textSourceBox_.y_min) {
            height = textSourceBox_.y_max - textSourceBox_.y_min + 1;
        }
        if (textSourceBox_.x_max >= textSourceBox_.x_min) {
            // Subtract space for line numbers and separator
            width = (textSourceBox_.x_max - textSourceBox_.x_min) - 5;
            width = std::max(1, width);
        }




        return element;
    }

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
            return true;
        }
        if (event == Event::Delete) {
            buffer_.Delete();
            return true;
        }
        if (event == Event::Return) {
            buffer_.InsertChar('\n');
            return true;
        }
        if (event.is_character()) {
            buffer_.InsertChar(event.character()[0]);
            return true;
        }

        if (event.is_mouse()) {
            if (event.mouse().button == Mouse::Left && event.mouse().motion == Mouse::Pressed) {
                if (textSourceBox_.Contain(event.mouse().x, event.mouse().y)) {
                    int y = event.mouse().y - textSourceBox_.y_min;
                    int x = event.mouse().x - textSourceBox_.x_min;

                    // Adjust for vertical scroll position
                    y += scroll_y;

                    // Check if click was in line numbers area
                    if (x < 4) { // Width of line numbers column
                        buffer_.MoveCursorToClosestPosition(y, 0);
                    } else {
                        // Adjust for horizontal scroll and line numbers width
                        x = x - 4 + scroll_x;
                        buffer_.MoveCursorToClosestPosition(y, x);
                    }
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
    ftxui::Box textSourceBox_;
    int scroll_y = 0;
    int& height;
    int scroll_x = 0;
    int& width; 
};

ftxui::Component TextEditor(std::string& content, int& parent_width, int& parent_height) {
    return Make<TextEditorImpl>(content, parent_width, parent_height);
};
