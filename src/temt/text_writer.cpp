#include "text_writer.hpp"

#include <fstream>
#include <sstream>
#include "FileManip.hpp"
#include "app_data.hpp"
#include "ftxui/component/event.hpp"
#include "ftxui/component/screen_interactive.hpp"
#include "spdlog/spdlog.h"
#include "text_editor.hpp"

using namespace ftxui;

class TextWriterImpl : public ComponentBase {
   public:
    TextWriterImpl(std::string_view file_path, std::function<void()> exitClosure)
        : exitClosure_(exitClosure), original_filepath(file_path.data()) {
        auto logger = spdlog::get("file_logger");

        if (LoadFromFile(original_filepath)) {
            logger->info("Loaded file: {}", original_filepath);
            is_opened_file_ = true;
        } else {
            logger->error("Failed to load file: {}", original_filepath);
            is_opened_file_ = false;
        }

        textSource_ = TextEditor(content_, is_file_changed);
        Add(textSource_);
    }

    TextWriterImpl(std::string_view content, std::string_view current_dir, std::function<void()> exitClosure)
        : exitClosure_(exitClosure), content_(content.data()) {
        textSource_ = TextEditor(content_, is_file_changed);
        Add(textSource_);
        is_opened_file_ = true;
        is_file_changed = true;

        original_filepath = temt::FileManip::assemblePath(current_dir, "Untitled.txt");
    }

    Element OnRender() override {
        if (!is_opened_file_) {
            return vbox({text("Failed to load file"), text(original_filepath) | underlined}) | center;
        }

        return vbox({
            hbox(text(original_filepath), filler(),
                 (is_file_changed ? hbox({text("Unsaved") | underlined, text(" ❌ ")}) : text("Saved ✅ "))),
            separator(),
            textSource_->Render() | flex,
        });
    }

    bool LoadFromFile(const std::string& path) {
        std::ifstream file(path);
        if (!file.is_open())
            return false;

        content_.clear();
        std::string line;
        while (std::getline(file, line)) {
            content_ += line + "\n";
        }
        file.close();

        auto logger = spdlog::get("file_logger");
        logger->info("TextWriter: file {} is loaded", original_filepath);
        logger->flush();

        return true;
    }

    bool SaveToFile(const std::string& path) const {
        std::ofstream file(path);
        if (!file.is_open())
            return false;

        file << content_;
        file.close();

        auto logger = spdlog::get("file_logger");
        logger->info("TextWriter: file {} is saved", original_filepath);
        logger->flush();

        return true;
    }

    bool Focusable() const final { return true; }

    bool OnEvent(Event event) {
        if (event == Event::CtrlS) {
            SaveToFile(original_filepath);
            is_file_changed = false;
        }
        if (event == Event::CtrlQ) {
            exitClosure_();
        }

        return ComponentBase::OnEvent(event);
    }

   private:
    std::function<void()> exitClosure_;
    std::string content_;
    std::string original_filepath;
    Component textSource_;
    bool is_opened_file_ = false;
    bool is_file_changed = false;
};

ftxui::Component TextWriter(std::string_view file_path, std::function<void()> exitClosure) {
    return Make<TextWriterImpl>(file_path, exitClosure);
};

ftxui::Component TextWriter(std::string_view content,
                            std::string_view current_location,
                            std::function<void()> exitClosure) {
    return Make<TextWriterImpl>(content, current_location, exitClosure);
};