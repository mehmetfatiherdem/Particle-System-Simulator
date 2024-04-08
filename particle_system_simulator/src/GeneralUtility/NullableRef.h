#pragma once

#include <optional>
#include <functional>

template<class T>
using NullableRef = std::optional<std::reference_wrapper<T>>;