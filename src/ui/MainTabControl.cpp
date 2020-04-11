#include <ui/MainTabControl.h>
#include <ui/Manager.h>
#include <ui/EquippedItemsView.h>
#include <ui/ContainerView.h>
#include <utils/Assert.h>
#include <resource/ResourceManager.h>

using namespace UI;

MainTabControl::MainTabControl(Manager* manager, Element* parent)
    : Element(manager, parent), m_currentIdx(0)
{
    setPreferredOuterSize( {TOTAL_WIDTH, TOTAL_HEIGHT} );
    setLayout<UI::HorizontalLayout>( 0, UI::VAlignment::Top );

    m_buttonHolder = manager->createElement<Element>( this );
    m_buttonHolder->setLayout<UI::VerticalLayout>( 0, UI::HAlignment::Fill );
    m_buttonHolder->setPreferredOuterSize({TAB_BUTTONS_WIDTH, TOTAL_HEIGHT});

    m_contentHolder = manager->createElement<Element>( this );
    m_contentHolder->setLayout<UI::VerticalLayout>( 0, UI::HAlignment::Left );
    m_contentHolder->setPreferredOuterSize({TOTAL_WIDTH - TAB_BUTTONS_WIDTH, TOTAL_HEIGHT});

    // Character stats

    ElementPtr tabCharStats = addTab();
    tabCharStats->setBackgroundSprite( ResourceManager::get().getSprite( "char-stats-tab" ) );

    // Inventory & equip

    ElementPtr tabInvEquip = addTab();
    tabInvEquip->setBackgroundSprite( ResourceManager::get().getSprite( "equip-inventory-tab" ) );

    auto equipView = manager->createElement<UI::EquippedItemsView>( tabInvEquip.get() );
    equipView->setPreferredOuterSize({20, 300});
    equipView->setId( "main-equip-view" );

    auto inventoryView = manager->createElement<UI::ContainerView>( tabInvEquip.get() );
    inventoryView->setPreferredOuterSize({20, 200});
    inventoryView->setId( "main-inventory-view" );



    ElementPtr dummyTab2 = addTab();
    ElementPtr dummyTab3 = addTab();
    ElementPtr dummyTab4 = addTab();

    setCurrentTab(1);
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
    setBackgroundSprite( ResourceManager::get().getSprite( "main-tab-button" ) );
}

int TabControlButton::index() const
{
    return m_index;
}
