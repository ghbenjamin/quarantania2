#include <game/Player.h>
#include <algorithm>

Player::Player(ImPlayerData &&data, EntityRef ref)
: m_imData(data), m_ref(ref)
{

}