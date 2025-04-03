#include "hideable.hpp"

#include <assert.h>
#include <algorithm>
#include "ftxui/component/event.hpp"

using namespace ftxui;

class HideableImpl : public ComponentBase {
   public:
    HideableImpl(Components children, bool& toggle) : toggled(toggle) {
        for (Component& child : children) {
            Add(child);
        }
    }

    Element OnRender() override final {
        if (!toggled)
            return emptyElement();

        Elements elements;
        for (auto& child : children_) {
            elements.push_back(child->Render());
        }
        // Reverse the order of the elements.
        return vbox(std::move(elements));
    }

    bool Focusable() const final {
        if (!toggled)
            return false;
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

   private:
    bool& toggled;
};

Component Hideable(Components children, bool& toggle) {
    return Make<HideableImpl>(children, toggle);
}