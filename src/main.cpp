#include "resource.h"
#include <Windows.h>

#include <string>
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
		HRSRC hResource = nullptr;
		HGLOBAL hMemory = nullptr;

		Parameters p;

	public:
		Resource(int resource_id, const std::string &resource_class) {
			hResource = FindResourceA(nullptr, MAKEINTRESOURCEA(resource_id), resource_class.c_str());
			hMemory = LoadResource(nullptr, hResource);

			p.size_bytes = SizeofResource(nullptr, hResource);
			p.ptr = LockResource(hMemory);
		}

		auto& GetResource() const {
			return p;
		}

		auto GetResourceString() const {
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