#include "text_writer.hpp"

#include <fstream>
#include <iomanip>
#include <iostream>
#include <sstream>
#include "app_data.hpp"
#include "ftxui/component/event.hpp"
#include "ftxui/component/screen_interactive.hpp"
#include "text_editor.hpp"
#include "spdlog/spdlog.h"

using namespace ftxui;

class TextWriterImpl : public ComponentBase {
   public:
    TextWriterImpl(temt::AppData& appData, std::function<void()> exitClosure) : exitClosure_(exitClosure) {
        temt::FileManip::FileInfo dirEntry = appData.selectedEntry();
        original_filepath = temt::FileManip::assemblePath(dirEntry.parentDirectory, dirEntry.path);

        if (LoadFromFile(original_filepath)) {
            appData.file_logger_->info("Loaded file: {}", original_filepath);
            is_opened_file_ = true;
        } else {
            appData.file_logger_->error("Failed to load file: {}", original_filepath);
            is_opened_file_ = false;
        }

        textSource_ = TextEditor(content_, contentWidth_, contentHeight_);
    }

    TextWriterImpl(std::string& content, std::function<void()> exitClosure)
        : exitClosure_(exitClosure), content_(content) {
        textSource_ = TextEditor(content_, contentWidth_, contentHeight_);
        is_opened_file_ = true;
        original_filepath = "Untitled.txt";  // needs some works with checking available names
    }

    Element OnRender() override {
        if (!is_opened_file_) {
            return vbox({text("Failed to load file"), text(original_filepath) | underlined}) | center;
        }

        Element res = textSource_->Render() | reflect(textSourceBox_);
        contentWidth_ = textSourceBox_.x_max - textSourceBox_.x_min;
        contentHeight_ = textSourceBox_.y_max - textSourceBox_.y_min;
        res = textSource_->Render() | reflect(textSourceBox_);


        return vbox({
            hbox(text(original_filepath), filler(), text("Saved?")),
            separator(),
            textSource_->Render()|flex,
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
        return true;
    }

    bool SaveToFile(const std::string& path) const {
        std::ofstream file(path);
        if (!file.is_open())
            return false;

        file << content_;
        file.close();
        return true;
    }

    bool OnEvent(Event event) override { return textSource_->OnEvent(event); }

    bool Focusable() const final { return true; }

   private:
    std::function<void()> exitClosure_;
    std::string content_;
    int contentWidth_;
    int contentHeight_;
    std::string original_filepath;
    Component textSource_;
    Box textSourceBox_;
    bool is_opened_file_ = false;
    bool is_first_render = true;
};

ftxui::Component TextWriter(temt::AppData& appData, std::function<void()> exitClosure) {
    return Make<TextWriterImpl>(appData, exitClosure);
};

ftxui::Component TextWriter(std::string& content, std::function<void()> exitClosure) {
    return Make<TextWriterImpl>(content, exitClosure);
};