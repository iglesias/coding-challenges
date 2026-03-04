// g++ -std=c++17 -I${ftxui}/include -L${ftxui}/build sinc.cpp -lftxui-component -lftxui-dom -lftxui-screen -o sinc -I{googletest}/include -L{googletest}/lib -lgtest
// https://stackoverflow.com/questions/78284124/configuring-a-project-with-ftxui-in-visual-studio-code

#include <cmath>
#include <cstring>
#include <thread>
#include <variant>
#include <vector>

#include <ftxui/component/component.hpp>
#include <ftxui/component/loop.hpp>
#include <ftxui/component/screen_interactive.hpp>
#include <ftxui/dom/elements.hpp>
#include <ftxui/screen/color.hpp>

#include <gtest/gtest.h>

using namespace ftxui;

enum class WaveSampleFormatEnum { F32, F64, }; // ¯\_(ツ)_/¯
using WaveSampleFormat = WaveSampleFormatEnum;
using variant_vector = std::variant<std::vector<float>, std::vector<double>>;

// C++23's std::unreachable backport for C++17:
#ifdef __GNUC__
    [[noreturn]] inline __attribute__((always_inline)) void unreachable()
    { __builtin_unreachable(); }
#elif defined(_MSC_VER)
    [[noreturn]] __forceinline void unreachable() { __assume(false); }
#else
    [[noreturn]] inline void unreachable() {}
#endif

auto make_samples_vector(WaveSampleFormat wsf) -> variant_vector {
  switch (wsf) {
    case WaveSampleFormat::F32:
      return std::vector<float>{};
    case WaveSampleFormat::F64:
      return std::vector<double>{};
    default:
      throw std::runtime_error("Unexpected sample format!");
  }
}

struct Wave {
  std::vector<std::byte> data;
  int wave_sample_size = 0;

  Wave(WaveSampleFormat wsf) {
    wave_sample_size = [wsf]() {
    switch (wsf) {
      case WaveSampleFormat::F32:
        return 4;
      case WaveSampleFormat::F64:
        return 8;
    }
    unreachable();
    }();
  }

  Wave(WaveSampleFormat wsf, const std::byte* bytes, int N) : Wave(wsf) {
    data = std::vector(bytes, bytes + N);
  }

  auto data_size() const {
    return data.size() * wave_sample_size;
  }
};

class WaveSamplesFormatTest :
  public ::testing::TestWithParam<WaveSampleFormat> {};

TEST_P(WaveSamplesFormatTest, DataSize) {
  auto samples = make_samples_vector(GetParam());
  const int N = 512;
  std::visit([](auto&& arg) {
    using T = typename std::decay_t<decltype(arg)>::value_type;
    arg.resize(N);
    for (int i = 0; i < N; i++) {
      arg[i] = std::sin(10 * 1.61803 * i / static_cast<T>(N));
    }
    Wave wave(GetParam(), reinterpret_cast<const std::byte*>(arg.data()), N);
    ASSERT_EQ(wave.data_size(), arg.size()*sizeof(T));
  }, samples);
}

INSTANTIATE_TEST_SUITE_P(
  WaveSamplesFormatTestInstantiation,
  WaveSamplesFormatTest,
  ::testing::Values(WaveSampleFormat::F32, WaveSampleFormat::F64)
);

int main(int argc, char** argv) {
  for (int i = 1; i < argc; i++) {
    if (std::string(argv[i]) == "--run_tests") {
      ::testing::InitGoogleTest(&argc, argv);
      return RUN_ALL_TESTS();
    }
  }

  auto screen = ScreenInteractive::Fullscreen();

  int iteration = 0;

  Wave wave(WaveSampleFormat::F64);

  // Periodic sinc pattern
  auto sinc = [&iteration, &wave](int width, int height) {
    std::vector<int> output(width);
    for (int i = 0; i < width; i++) {
      /*
        Scale in (i + iteration) instead of mapping to [-π, π] and work in a
        natural coordinate space where the sinc's side lobes are visible.

        After wrapping with modulo, shift values in the second half of each
        period to the negative side, so each sinc is centered.

        Parameters:
        - scale: lower values → wider sinc (fewer side lobes visible)
                 higher values → narrower sinc (more side lobes)
        - period
        - amplitude
       */

      const float scale = 0.35f;     // Adjust this to see more/fewer side lobes
      const float period = 40.0f;    // Distance between peaks
      const float amplitude = 0.69f; // Height of the peaks

      float t = ((i + iteration) * scale);

      // Periodicity is created with the modulo operation
      t = std::fmod(t, period);

      // Shift to center each period (fftshift?)
      if (t > period / 2.0f) t -= period;

      const float sinc_value = std::abs(t) < 0.01f ? 1.0f : std::sin(t) / t;

      float x = 0.1;
      x += amplitude * sinc_value;
      x *= height;

      std::vector<std::byte>& bytes = wave.data;
      const auto current_size = bytes.size();
      bytes.resize(current_size + sizeof(x));
      std::memcpy(bytes.data() + current_size, &x, sizeof(x));
      output[i] = static_cast<int>(x);
    }
    return output;
  };

  auto graph_renderer = Renderer([&sinc] {
    auto wave = hbox({graph(std::ref(sinc)) | color(Color::BlueLight)});

    return hbox({
               wave | flex,
           }) |
           flex;
  });

  Loop loop(&screen,
            Renderer(graph_renderer, [&] {
                     return vbox({
                         text("El Seno Cardinal") | bold | hcenter,
                         graph_renderer->Render(),
                     });
            }));

  const int FPS = 60;
  while (!loop.HasQuitted()) {
    iteration++;
    screen.RequestAnimationFrame();
    loop.RunOnce();
    std::this_thread::sleep_for(std::chrono::milliseconds(1000 / FPS));
  }
}
