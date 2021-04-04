#include <ui/lib/Dialogs.h>
#include <ui/lib/Manager.h>
#include <resource/ResourceManager.h>
#include <ui/lib/Controls.h>

UI::Dialog::Dialog(Manager* manager, Element* parent, std::string const &title, int maxWidth )
: Element(manager, parent), m_maxWidth(maxWidth)
{
    
    setMaximumOuterSize({maxWidth, 1000});
    setLayout<VerticalLayout>( 1, HAlignment::Centre );
    
    auto const& patch = ResourceManager::get().getNinePatch( "simple-border-solid" ).getPatch();
    setBackground( patch );
    setBorderWidth( patch.getBorderWidth() );
    
    auto titleBar = manager->createElement<Element>( this );
    titleBar->setLayout<HorizontalLayout>( 0, VAlignment::Centre );
    titleBar->setPadding(4);
    
    auto titleText = manager->createElement<Label>(titleBar.get(), title, Colour::White, 18 );
    
    m_contentHolder = manager->createElement<Element>( this );
    m_contentHolder->setLayout<VerticalLayout>( 0, HAlignment::Left );
    m_contentHolder->setPadding(4);

    m_buttonHolder = manager->createElement<Element>( this );
    m_buttonHolder->setLayout<HorizontalLayout>( 4, VAlignment::Centre );
    m_buttonHolder->setPadding(8);
}

UI::MessageBoxButtonInfo::MessageBoxButtonInfo( std::string const &message, std::function<void()> const &callback )
        : message(message), callback(callback) {}

UI::MsgBoxDialog::MsgBoxDialog(UI::Manager *manager, UI::Element *parent, std::string const &title, int maxWidth,
                               std::string const &message, std::vector<MessageBoxButtonInfo> const& buttonData)
       : Dialog(manager, parent, title, maxWidth), m_buttonData(std::move(buttonData))
{
    auto textContent = manager->createElement<Label>(m_contentHolder.get(), message, Colour::White, 14);
    
    for ( auto const& item : m_buttonData )
    {
        auto btn =  manager->createElement<Button>( m_buttonHolder.get(), item.message, [=](){
            item.callback();
            manager->deleteElement( shared_from_this() );
        });
        m_buttons.push_back(btn);
    }
    
    int maxW = 0;
    int maxH = 0;
    for ( auto& btn : m_buttons )
    {
        auto size = btn->innerBounds();
        maxW = std::max(maxW, size.w());
        maxH = std::max(maxH, size.h());
    }
    
    Vector2i preferred = {maxW, maxH};
    for ( auto& btn : m_buttons )
    {
        btn->setPreferredContentSize(preferred);
    }

    doLayout();
}


