#pragma once

#include <string_view>
#include <utility>

namespace morra {

auto handle_input(int argc, char** argv) -> std::pair<int, std::string_view>;

}
