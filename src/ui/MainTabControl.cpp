#include <ui/MainTabControl.h>
#include <ui/Manager.h>
#include <ui/EquippedItemsView.h>
#include <ui/ContainerView.h>
#include <utils/Assert.h>

using namespace UI;

MainTabControl::MainTabControl(Manager* manager, Element* parent)
    : Element(manager, parent), m_currentIdx(0)
{
    setPreferredOuterSize( {TOTAL_WIDTH, TOTAL_HEIGHT} );
//    setBackgroundSprite({"ui-frames", "main-ui-tab-control"});
    setLayout<UI::HorizontalLayout>( 0, UI::VAlignment::Top );
    setBackgroundColour( Colour::Magenta );

    m_buttonHolder = manager->createElement<Element>( this );
    m_buttonHolder->setLayout<UI::VerticalLayout>( 0, UI::HAlignment::Fill );
    m_buttonHolder->setPreferredOuterSize({TAB_BUTTONS_WIDTH, TOTAL_HEIGHT});
    m_buttonHolder->setBackgroundColour(Colour::Maroon);

    m_contentHolder = manager->createElement<Element>( this );
    m_contentHolder->setLayout<UI::VerticalLayout>( 0, UI::HAlignment::Left );
    m_contentHolder->setPreferredOuterSize({TOTAL_WIDTH - TAB_BUTTONS_WIDTH, TOTAL_HEIGHT});
    m_contentHolder->setBackgroundColour(Colour::Mint);

    // Inventory & equip

    ElementPtr tabInvEquip = addTab();

    auto equipView = manager->createElement<UI::EquippedItemsView>( tabInvEquip.get() );
    equipView->setPreferredOuterSize({20, 200});
    equipView->setId( "main-equip-view" );
    equipView->setBackgroundColour(Colour::Blue);

    auto inventoryView = manager->createElement<UI::ContainerView>( tabInvEquip.get() );
    inventoryView->setPreferredOuterSize({20, 200});
    inventoryView->setId( "main-inventory-view" );
    inventoryView->setBackgroundColour(Colour::Green);


    // Character stats

    ElementPtr tabCharStats = addTab();

    auto dummyElem = manager->createElement<UI::Element>( tabCharStats.get() );
    dummyElem->setPreferredOuterSize({20, 100});
    dummyElem->setBackgroundColour(Colour::Brown);


    setCurrentTab(0);
}

ElementPtr MainTabControl::addTab()
{
    int idx = m_tabContents.size();

    ElementPtr tabContainer = manager()->createElement<Element>( m_contentHolder.get() );
    tabContainer->setPreferredOuterSize({TOTAL_WIDTH - TAB_BUTTONS_WIDTH, TOTAL_HEIGHT});
    tabContainer->setLayout<UI::VerticalLayout>( 0, UI::HAlignment::Fill );
    tabContainer->setHidden(true);

    m_tabContents.push_back( tabContainer );

    ElementPtr tabButton = manager()->createElement<TabControlButton>( m_buttonHolder.get(), idx );
    tabButton->setPreferredOuterSize({0, TAB_BUTTONS_HEIGHT});
    tabButton->addEventCallback(UEventType::Click, [this, idx](UEvent& evt) {
        this->setCurrentTab( idx );
    });

    return tabContainer;
}

void MainTabControl::setCurrentTab( int idx )
{
    Assert( idx < m_tabContents.size() );

    m_tabContents[ m_currentIdx ]->setHidden( true );
    m_tabContents[ idx ]->setHidden( false );

    m_currentIdx = idx;
}

TabControlButton::TabControlButton(Manager *manager, Element *parent, int idx)
        : Element(manager, parent), m_index(idx)
{
    setBackgroundColour(Colour::Apricot);
    setBorder(2, Colour::Olive);
}

int TabControlButton::index() const
{
    return m_index;
}
