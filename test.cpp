#include <iostream>

template <class T, class U>
long max(T a, U b)
{
	return max(a*1LL,b*1LL);
}


int main()
{
	std::cout << max(1,69LL) << " " << std::max(1,69LL);
}
