#include <cmath>
#include <limits>
#include <print>
#include <utility>

struct vec2 {
    int x, y;

    constexpr vec2& operator+=(const vec2& that) noexcept {
        this->x += that.x;
        this->y += that.y;
        return *this;
    }

    constexpr void drag() noexcept {
        switch(std::bit_cast<int8_t>(this->x <=> 0)) {
            case -1:
                this->x -= 1;
                break;
            case  0:
                break;
            case +1:
                this->x += 1;
        }
    }
};

struct rect_area {
    vec2 top_left, bot_right;

    constexpr bool contains(const vec2& p) const noexcept {
        return top_left.x <= p.x and p.x <= bot_right.x and
            bot_right.y <= p.y and p.y <= top_left.y;
    }
};

constexpr auto solve(rect_area const& target_area) {
    std::pair<int, int> ans{std::numeric_limits<int>::min(), 0};
    for (int Y = -200; Y <= 1000; Y++) for (int X = -600; X <= 1000; X++) {
        const vec2 v0{X, Y};
        vec2 p{0, 0}, v{v0};
        int potential_max_y{std::numeric_limits<int>::min()}; 
        for (;;) {
            p += v;
            v.drag();
            v += {0, -1};
            potential_max_y = std::max(potential_max_y, p.y);
            if (target_area.contains(p)) {
                ans.first = std::max(ans.first, potential_max_y);
                ans.second++;
                break;
            }
            if (p.y <= target_area.bot_right.y) break;
        }
    }
    return ans;
}

int main() {
    constexpr rect_area target_area{{102, -90}, {157, -146}};
    //static_assert(solve(target_area) == std::make_pair(10585, 5247));
    const auto answer_pair = solve(target_area);
    std::print("Part one: {}\nPart two: {}\n", answer_pair.first, answer_pair.second); 
}
