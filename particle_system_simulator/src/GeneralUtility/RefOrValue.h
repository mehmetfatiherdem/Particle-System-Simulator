#pragma once

#include <optional>
#include <functional>

template<typename T>
class RefOrValue
{
private:
	std::optional<std::reference_wrapper<T>> ref;
	std::optional<T> val;

public:

	RefOrValue() : ref(std::nullopt), val(std::nullopt) { }
	RefOrValue(T& object, bool isRef) : ref(std::nullopt), val(std::nullopt)
	{
		if (isRef)
		{
			ref = object;
		}
		else
		{
			val = object;
		}
	}

	T* get()
	{
		if (ref.has_value())
		{
			return &ref.value().get();
		} else if (val.has_value())
		{
			return &val.value();
		}

		return nullptr;
	}

	bool isEmpty()
	{
		return (!ref.has_value() && !val.has_value());
	}

	bool isReference()
	{
		return ref.has_value();
	}

	bool isValue()
	{
		return val.has_value();
	}

	void empty()
	{
		ref = std::nullopt;
		val = std::nullopt;
	}

	T& assign(T& object, bool isRef)
	{
		if (isRef)
		{
			val = std::nullopt;
			ref = object;
			return ref.value().get();
		}
		else
		{
			ref = std::nullopt;
			val = object;
			return val.value();
		}
	}

	T* operator*()
	{
		return get();
	}

	T* operator ->()
	{
		return get();
	}
};