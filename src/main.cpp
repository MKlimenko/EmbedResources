#include "resource.h"
#include <Windows.h>

#include <string_view>
#include <vector>

namespace {
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

		auto GetResourceString() {
			std::string_view dst;
			if (p.ptr != nullptr)
				dst = std::string_view(reinterpret_cast<char*>(p.ptr), p.size_bytes);
			return dst;
		}
	};

	void GetFile() {
		Resource very_important(IDR_TEXT1, "TEXT");
		auto dst = very_important.GetResourceString();

		Resource more_data(IDR_TEXT2, "TEXT");
		auto dst_2 = more_data.GetResourceString();
	}
}

int main() {
	GetFile();

	return 0;
}