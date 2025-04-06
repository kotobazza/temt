#include "text_writer.hpp"

#include <fstream>
#include <iomanip>
#include <iostream>
#include <sstream>
#include "app_data.hpp"
#include "ftxui/component/event.hpp"
#include "ftxui/component/screen_interactive.hpp"
#include "spdlog/spdlog.h"
#include "text_editor.hpp"

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

        textSource_ = TextEditor(content_, is_file_changed);
        Add(textSource_);
    }

    TextWriterImpl(std::string& content, std::function<void()> exitClosure)
        : exitClosure_(exitClosure), content_(content) {
        textSource_ = TextEditor(content_, is_file_changed);
        is_opened_file_ = true;
        original_filepath = "Untitled.txt";  // needs some works with checking available names
    }

    Element OnRender() override {
        if (!is_opened_file_) {
            return vbox({text("Failed to load file"), text(original_filepath) | underlined}) | center;
        }
        
        return vbox({
            hbox(text(original_filepath), filler(), (is_file_changed ? text("Unsaved ❌")|underlined : text("Saved ✅"))),
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
        return true;
    }

    bool SaveToFile(const std::string& path) const {
        std::ofstream file(path);
        if (!file.is_open())
            return false;

        file << content_;
        file.close();
        auto logger = spdlog::get("file_logger");
        logger->info("Saved file");
        logger->flush();
        return true;
    }

    bool Focusable() const final { return true; }

    bool OnEvent(Event event) {
        if (event == Event::CtrlS) {
            SaveToFile(original_filepath);
            is_file_changed = false;
            auto logger = spdlog::get("file_logger");
            logger->info("CtrlS pressed");
            logger->flush();
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

ftxui::Component TextWriter(temt::AppData& appData, std::function<void()> exitClosure) {
    return Make<TextWriterImpl>(appData, exitClosure);
};

ftxui::Component TextWriter(std::string& content, std::function<void()> exitClosure) {
    return Make<TextWriterImpl>(content, exitClosure);
};