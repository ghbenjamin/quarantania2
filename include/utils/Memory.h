#pragma once

#include <memory>

namespace utils
{

template <typename OldT, typename NewT>
std::unique_ptr<NewT> unique_ptr_cast( std::unique_ptr<OldT>&& ptr )
{
    auto casted = static_cast<NewT*>( ptr.release() );
    return std::unique_ptr<NewT>( casted );
}

template <typename Ret, typename Act, typename... Args>
std::shared_ptr<Ret> make_shared_with_type( Args... args )
{
    static_assert( std::is_base_of_v<Ret, Act> );
    auto ptr = std::make_shared<Act>( std::forward<Args>(args)... );
    return std::static_pointer_cast<Ret>(ptr);
}

template <typename Ret, typename Act, typename... Args>
std::unique_ptr<Ret> make_unique_with_type( Args... args )
{
    return unique_ptr_cast<Act, Ret>( std::make_unique<Act>( std::forward<Args>(args)... ) );
}

}