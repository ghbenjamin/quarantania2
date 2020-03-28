#include <ui/MainTabControl.h>
#include <ui/Manager.h>
#include <ui/EquippedItemsView.h>
#include <ui/ContainerView.h>

using namespace UI;

MainTabControl::MainTabControl(Manager* manager, Element* parent)
    : Element(manager, parent)
{
    setPreferredOuterSize( {TOTAL_WIDTH, TOTAL_HEIGHT} );
    setBackgroundSprite({"ui-frames", "main-ui-tab-control"});
    setLayout<UI::HorizontalLayout>( 0, UI::VAlignment::Top );
    setBackgroundColour( Colour::Magenta );

    m_buttonHolder = manager->createElement<Element>( this );
    m_buttonHolder->setLayout<UI::VerticalLayout>( 0, UI::HAlignment::Fill );
    m_buttonHolder->setPreferredOuterSize({TAB_BUTTONS_WIDTH, TOTAL_HEIGHT});
    m_buttonHolder->setBackgroundColour(Colour::Maroon);

    m_contentHolder = manager->createElement<Element>( this );
    m_contentHolder->setPreferredOuterSize({TOTAL_WIDTH - TAB_BUTTONS_WIDTH, TOTAL_HEIGHT});
    m_contentHolder->setBackgroundColour(Colour::Mint);


    // Inventory & equip

    ElementPtr tabInvEquip = addTab();

    auto equipView = manager->createElement<UI::EquippedItemsView>( tabInvEquip.get() );
    equipView->setPreferredOuterSize({200, 200});
    equipView->setId( "main-equip-view" );
    equipView->setBackgroundColour(Colour::Blue);

    auto inventoryView = manager->createElement<UI::ContainerView>( tabInvEquip.get() );
    inventoryView->setPreferredOuterSize({200, 200});
    inventoryView->setId( "main-inventory-view" );
    inventoryView->setBackgroundColour(Colour::Green);


    // Character stats

    // ElementPtr tabCharStats = addTab();
}

ElementPtr MainTabControl::addTab()
{
    ElementPtr tabContainer = manager()->createElement<Element>( m_contentHolder.get() );
    tabContainer->setPreferredOuterSize({TOTAL_WIDTH - TAB_BUTTONS_WIDTH, TOTAL_HEIGHT});
    tabContainer->setLayout<UI::VerticalLayout>( 0, UI::HAlignment::Fill );

    m_tabContents.push_back( tabContainer );

    ElementPtr tabButton = manager()->createElement<TabControlButton>( m_buttonHolder.get() );
    tabButton->setPreferredOuterSize({0, 80});

    return tabContainer;
}

void MainTabControl::setCurrentTab( int idx )
{

}

TabControlButton::TabControlButton(Manager *manager, Element *parent)
        : Element(manager, parent)
{
    setBackgroundColour(Colour::Apricot);
    setBorder(2, Colour::Olive);
}
