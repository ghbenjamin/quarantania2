#pragma once

#include <memory>

namespace Utils
{


template <typename Ret, typename Act, typename... Args>
std::shared_ptr<Ret> make_shared_with_type( Args... args )
{
    static_assert( std::is_base_of_v<Ret, Act> );
    auto ptr = std::make_shared<Act>( std::forward<Args>(args)... );
    return std::static_pointer_cast<Ret>(ptr);
}

}