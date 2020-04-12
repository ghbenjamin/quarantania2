#include <game/Player.h>
#include <algorithm>

Player::Player(PlayerData const& data, EntityRef ref)
: m_imData(data), m_ref(ref)
{

}

EntityRef Player::ref()
{
    return m_ref;
}