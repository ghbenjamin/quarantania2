#pragma once

#include <unordered_map>

#include <game/InputInterface.h>

template <typename T>
class Keymap
{
public:


private:
    static_assert( std::is_enum_v<T> );
    std::unordered_map<IEventType, T> m_keymap;

};