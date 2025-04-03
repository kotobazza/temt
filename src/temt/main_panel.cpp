#include "main_panel.hpp"
#include <cassert>
#include "ftxui/component/event.hpp"

using namespace ftxui;

class MainPanelImpl : public ComponentBase {
   public:
    MainPanelImpl(Components children) {
        for (Component& child : children) {
            Add(child);
        }
    }

    void SetChildren(ftxui::Components new_children) {
        children_.clear();
        for (auto& child : new_children) {
            Add(child);
        }
    }

    Element OnRender() override final {
        Elements elements;
        for (auto& child : children_) {
            elements.push_back(child->Render());
        }
        // Reverse the order of the elements.
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

    void SetActiveChild(ComponentBase* child) final {
        if (children_.size() == 0) {
            return;
        }

        int previous_index = 0;
        for (size_t i = 0; i < children_.size(); ++i) {
            if (children_[i].get() == child) {
                previous_index = i;
                break;
            }
        }

        // Rotate to make the `previous_index` become the first element.
        std::rotate(children_.begin(), children_.begin() + previous_index, children_.end());

        assert(ActiveChild().get() == child);
    }

    bool OnEvent(Event event) final {
        for (auto& child : children_) {
            if (child->OnEvent(event)) {
                return true;
            }
        }
        return false;
    }
};

ftxui::Component MainPanel(ftxui::Components childs) {
    return Make<MainPanelImpl>(childs) | flex | border;
};

void UpdateChildren(Component main_panel, ftxui::Components new_children) {
    if (auto impl = dynamic_cast<MainPanelImpl*>(main_panel.get())) {
        impl->SetChildren(std::move(new_children));
    }
}