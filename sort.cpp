#include <algorithm>

template<typename ForwardIt>
void bubble_sort(ForwardIt first, ForwardIt last)
{
	for(ForwardIt it = first; it != last; ++it) { 
		bool sorted = true;
		for(ForwardIt it2 = first; next(it2) != last; ++it2)
			if(*next(it2) < *it2) {
				std::iter_swap(it2, next(it2));
				sorted = false;
			}
		if(sorted) break;
	} 
}
template<typename ForwardIt, typename Compare>
void bubble_sort(ForwardIt first, ForwardIt last, Compare comp)
{
	for(ForwardIt it = first; it != last; ++it) { 
		bool sorted = true;
		for(ForwardIt it2 = first; next(it2) != last; ++it2)
			if(comp(*next(it2), *it2)) {
				std::iter_swap(it2, next(it2));
				sorted = false;
			}
		if(sorted) break;
	}
}
template<typename ForwardIt>
void selection_sort(ForwardIt first, ForwardIt last)
{
	for(ForwardIt it = first; it != last; ++it)
		std::iter_swap(it, std::min_element(it, last));
}
template<typename ForwardIt, typename Compare>
void selection_sort(ForwardIt first, ForwardIt last, Compare comp)
{
	for(ForwardIt it = first; it != last; ++it)
		std::iter_swap(it, std::min_element(it, last, comp));
}
template<typename ForwardIt>
void insertion_sort(ForwardIt first, ForwardIt last)
{
	for (auto it = first; it != last; ++it)
		std::rotate(std::upper_bound(first, it, *it), it, std::next(it));
}
template<typename ForwardIt, typename Compare>
void insertion_sort(ForwardIt first, ForwardIt last, Compare comp)
{
	for (auto it = first; it != last; ++it)
		std::rotate(std::upper_bound(first, it, *it, comp), it, std::next(it));
}
template<typename ForwardIt>
void heap_sort(ForwardIt first, ForwardIt last)
{
	std::make_heap(first, last);
	std::sort_heap(first, last);
}
template<typename ForwardIt, typename Compare>
void heap_sort(ForwardIt first, ForwardIt last, Compare comp)
{
	std::make_heap(first, last, comp);
	std::sort_heap(first, last, comp);
}
template<typename ForwardIt>
void quick_sort(ForwardIt first, ForwardIt last)
{
	if(first == last) return;
	auto pivot = *std::next(first, std::distance(first,last) / 2);
	ForwardIt middle = std::partition(first, last, 
		[&] (auto&& arg) { return arg < pivot; }
	);
	quick_sort(first, middle);
	quick_sort(
		std::partition(middle, last, 
			[&] (auto&& arg) { return !(pivot < arg); }
		), last
	);
}
template<typename ForwardIt, typename Compare>
void quick_sort(ForwardIt first, ForwardIt last, Compare comp)
{
	if(first == last) return;
	auto pivot = *std::next(first, std::distance(first,last) / 2);
	ForwardIt middle = std::partition(first, last, 
		[&] (auto&& arg) { return comp(arg, pivot); }
	);
	quick_sort(first, middle, comp);
	quick_sort(
		std::partition(middle, last, 
			[&] (auto&& arg) { return !comp(pivot, arg); }
		), last, comp
	);
}
template<typename ForwardIt>
void merge_sort(ForwardIt first, ForwardIt last)
{
	if(std::distance(first, last) > 1) {
		ForwardIt middle = next(first, std::distance(first, last) / 2);
		merge_sort(first, middle);
		merge_sort(middle, last);
		std::inplace_merge(first, middle, last);
	}
}
template<typename ForwardIt, typename Compare>
void merge_sort(ForwardIt first, ForwardIt last, Compare comp)
{
	if(std::distance(first, last) > 1) {
		ForwardIt middle = next(first, std::distance(first, last) / 2);
		merge_sort(first, middle, comp);
		merge_sort(middle, last, comp);
		std::inplace_merge(first, middle, last, comp);
	}
}
