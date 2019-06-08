#include <game/Player.h>
#include <algorithm>

Player::Player(ImPlayerData &data)
: m_imData( std::move(data) )
{

}
