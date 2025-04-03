#include "main_panel.hpp"

#include "ftxui/component/event.hpp"

using namespace ftxui;

class Impl : public ComponentBase {
   public:
    Impl(ftxui::Components children) {
        for (auto child : children) {
            Add(child);
        }
    }

    void SetChildren(ftxui::Components new_children) {
        children_.clear();
        for (auto& child : new_children) {
            Add(child);
        }
    }

    Element OnRender() {
        Elements elements;
        for (auto& child : children_) {
            elements.push_back(child->Render());
        }
        return vbox(std::move(elements));
    }

    bool Focusable() const final {
        if (children_.size() == 0)
            return false;
        return children_[0]->Focusable();
    }

    Component ActiveChild() final {
        if (children_.size() == 0)
            return nullptr;
        return children_[0];
    }

    bool OnEvent(Event event) final {
        for (auto& child : children_) {
            if (child->OnEvent(event)) {
                return true;
            }
        }
        return false;
    }

   private:
    ftxui::Component mainContainer_;
};

ftxui::Component MainPanel(ftxui::Components childs) {
    return Make<Impl>(childs) | flex_grow | border;
};

void UpdateChildren(Component main_panel, ftxui::Components new_children) {
    if (auto impl = dynamic_cast<Impl*>(main_panel.get())) {
        impl->SetChildren(std::move(new_children));
    }
}