#pragma once

#include <ui/lib/Element.h>

namespace UI
{

class Label;

enum class TransientMessageState
{
    FadingIn,
    Waiting,
    FadingOut
};

class TransientMessage : public Element
{
public:
    TransientMessage(Manager* manager, Element* parent, float fadeInTime, float visibleTime, float fadeOutTime );
    ~TransientMessage() override = default;

    std::shared_ptr<Label> getLabel();

protected:
    void updateSelf(uint32_t ticks, InputInterface &iinter, RenderInterface &rInter) override;
    void onAlphaModChange(float newValue) override;

private:

    std::shared_ptr<Label> m_message;

    TransientMessageState m_messageState;
    float m_fadeInTime;
    float m_visibleTime;
    float m_fadeOutTime;
};

}