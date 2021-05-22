#include <ui/lib/TransientMessage.h>
#include <ui/lib/Manager.h>
#include <ui/lib/Controls.h>
#include <resource/ResourceManager.h>

using namespace UI;

TransientMessage::TransientMessage(Manager *manager, Element *parent,
                                   float fadeInTime, float visibleTime, float fadeOutTime)
    : Element(manager, parent), m_fadeInTime(fadeInTime),
      m_visibleTime(visibleTime), m_fadeOutTime(fadeOutTime),
      m_messageState(TransientMessageState::FadingIn)
{
    setDecorative(true);

    m_message = manager->createElement<Label>( this );
    m_message->setTextSize(28);
    m_message->setTextColour(Colour::White);

    auto const& patch = ResourceManager::get().getNinePatch( "simple-border" ).getPatch();
    setBackground( patch );
    setBorderWidth( patch.getBorderWidth() );

    setPadding(6, 6, 0, 6);

    setAlphaMod( 0.0f );
    setAlphaTransition( 0.0f, 1.0f, m_fadeInTime );
}

void TransientMessage::updateSelf(uint32_t ticks, InputInterface &iinter, RenderInterface &rInter)
{
    switch (m_messageState)
    {
        case TransientMessageState::FadingIn:
        {
            if (!hasActiveAlphaTransition())
            {
                setAlphaTransition( 1.f, 1.f, m_visibleTime );
                m_messageState = TransientMessageState::Waiting;
            }
            break;
        }

        case TransientMessageState::Waiting:
        {
            if (!hasActiveAlphaTransition())
            {
                setAlphaTransition( 1.f, 0.f, m_fadeOutTime );
                m_messageState = TransientMessageState::FadingOut;
            }
            break;
        }

        case TransientMessageState::FadingOut:
        {
            if (!hasActiveAlphaTransition())
            {
                deleteSelf();
            }
            break;
        }
    }
}

void TransientMessage::onAlphaModChange(float newValue)
{
    m_message->setAlphaMod(newValue);
}

std::shared_ptr<Label> TransientMessage::getLabel()
{
    return m_message;
}
