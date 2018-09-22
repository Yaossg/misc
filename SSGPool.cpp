#include<cstddef>
#include<cstring>
#include<vector>
struct SSGPool {
	using byte = unsigned char;
	using size_t = std::size_t;
	struct span {
		byte* ptr = nullptr;
		size_t size = 0;
		bool operator <(span const& other) const { return ptr < other.ptr; }
		bool operator ==(span const& other) const { return ptr == other.ptr; }
		byte* begin() const { return ptr; }
		byte* end() const { return ptr + size; }
	} meta;
	std::vector<span> alloc_info;
	SSGPool(size_t size) : meta{new byte[size], size}, alloc_info() {}
	~SSGPool() { delete meta.ptr; }
	bool isUnused() { return alloc_info.empty(); }
	void* allocate(size_t size) {
		if(isUnused()) {
			if(size > meta.size)
				return nullptr;
			return alloc_info.insert(alloc_info.end(), {meta.begin(), size})->ptr;
		}
		if(alloc_info.size() == 1) {
			if(size > meta.size - alloc_info[0].size) 
				return nullptr;
			return alloc_info.insert(alloc_info.end(), {alloc_info[0].end(), size})->ptr;
		}
		for(size_t i = 1; i < alloc_info.size(); ++i) {
			size_t diff = alloc_info[i].begin() - alloc_info[i - 1].end();
			if(diff >= size) {
				return alloc_info.insert(alloc_info.begin() + i - 1, {alloc_info[i - 1].end(), size})->ptr;
			}
		}
		if(alloc_info.rbegin()->end() + size <= meta.end()) {
			return alloc_info.insert(alloc_info.end(), {alloc_info.rbegin()->end(), size})->ptr;
		}
		return nullptr;
	}
	void deallocate(void* ptr) {
		for(auto it = alloc_info.begin(); it != alloc_info.end(); ++it) {
			if(it->ptr == ptr) {
				alloc_info.erase(it);
				return;
			}
		}
	}
	void* reallocate(void* ptr, size_t size) {
		for(auto it = alloc_info.begin(); it != alloc_info.end(); ++it) {
			if(it->ptr == ptr) {
				if(it->size == size) {
					return it->ptr;
				}
				if(it->size > size || it->ptr + size <= std::next(it)->ptr) {
					it->size = size;
					return ptr;
				}
				void* cpy = allocate(size);
				return cpy ? std::memcpy(cpy, it->ptr, it->size) : cpy;
			}
		}
		return allocate(size);
	}
	size_t get_size(void* ptr) {
		for(span entry : alloc_info) {
			if(entry.ptr == ptr) {
				return entry.size;
			}
		}
		return 0;
	}
}; 
