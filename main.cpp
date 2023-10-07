#include <xfilesystem_abi.h>
#define __std_fs_code_page() __std_code_page::_Utf8
#include <__msvc_all_public_headers.hpp>

int main(void)
{
    std::map<std::size_t, std::string> m;

    for (const auto &e : std::filesystem::recursive_directory_iterator(L".") |
                             std::views::filter([](const auto &e) {
                                 return e.is_regular_file();
                             })) {
        const auto &p = e.path();
        const auto &k = [&p, &e] {
            std::ifstream f(p, std::ios::binary);
            std::string s;

            f.exceptions(std::ios::failbit);
            s.resize_and_overwrite(e.file_size(), [&f](auto p, auto n) {
                return f.read(p, n).gcount();
            });

            return std::hash<std::string>{}(s);
        }();
        const auto &v = p.generic_string();

        if (m.contains(k))
            std::println(R"(ln -f "{}" "{}")", m[k], v);
        else
            m[k] = v;
    }

    return 0;
}
