#include <type_traits>
#include <iostream>
#include <typeinfo>

template <typename T>
struct FoundType : std::true_type
{
	typedef T foundType;
};

template <typename T, typename... Rest>
struct Contains
{
};

template <typename T, typename First, typename... Rest>
struct Contains<T, First, Rest...> :
	std::conditional<std::is_same<T, First>::value,
		FoundType<First>,
		Contains<T, Rest...>>::type
{
};

template <typename T>
struct Contains<T> : std::false_type
{
};

template <typename T, typename... Rest>
struct ContainsConvertible
{
};

template <typename T, typename First, typename... Rest>
struct ContainsConvertible<T, First, Rest...> :
	std::conditional<std::is_convertible<T, First>::value,
		FoundType<First>,
		ContainsConvertible<T, Rest...>>::type
{
};

template <typename T>
struct ContainsConvertible<T> : std::false_type
{
};

template <typename T, typename... Rest>
struct FindType :
	std::conditional<Contains<T, Rest...>::value,
		Contains<T, Rest...>,
		ContainsConvertible<T, Rest...>>::type
{
};

int main()
{
	static_assert(FindType<int, float, int, double>::value, "int is not in the parameter pack float, int, double");
	std::cout << "nearest type of int in the parameter pack std::string, float: " <<
		typeid(FindType<int, std::string, float>::foundType).name() << std::endl;
}
