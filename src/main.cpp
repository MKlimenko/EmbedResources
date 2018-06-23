#include "resource.h"
#include <Windows.h>


#include <string_view>
#include <vector>

class Resource {
public:
	struct Parameters {
		std::size_t size_bytes = 0;
		void* ptr = nullptr;
	};
private:
	HMODULE hModule = nullptr;
	HRSRC hResource = nullptr;
	HGLOBAL hMemory = nullptr;

	Parameters p;
public:
	Resource(int resource_id, std::string_view resource_class) {
		hModule = GetModuleHandle(NULL);
		hResource = FindResource(hModule, MAKEINTRESOURCE(resource_id), resource_class.data());
		hMemory = LoadResource(hModule, hResource);

		p.size_bytes = SizeofResource(hModule, hResource);
		p.ptr = LockResource(hMemory);
	}

	~Resource() {
		FreeResource(hMemory);
	}

	auto& GetResource() {
		return p;
	}

	template <typename T>
	std::vector<T> GetResource() {
		std::vector<T> dst;
		if (p.ptr != nullptr)
			dst = std::vector<T>(reinterpret_cast<T*>(p.ptr), reinterpret_cast<T*>(p.ptr) + p.size_bytes / sizeof(T));
		return dst;
	}
};

void GetFile() {
	Resource very_important(IDR_TEXT1, "TEXT");

	auto dst = very_important.GetResource<char>();
	auto str = std::string(dst.begin(), dst.end());

	Resource more_data(IDR_TEXT2, "TEXT");
	auto dst_2 = more_data.GetResource<char>();
	auto str_2 = std::string(dst_2.begin(), dst_2.end());
}

int main() {
	GetFile();

	return 0;
}