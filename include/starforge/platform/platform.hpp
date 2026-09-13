#pragma once

#include <cstdint>
#include <memory>
#include <string>

namespace starforge::platform {

using GraphicsProcAddress = void (*)();
using GraphicsProcResolver = GraphicsProcAddress (*)(const char* name);

enum class Key : std::uint8_t {
    escape,
    w,
    a,
    s,
    d,
    count,
};

struct InputSnapshot final {
    bool escape{false};
    bool w{false};
    bool a{false};
    bool s{false};
    bool d{false};
};

struct FramebufferSize final {
    std::uint32_t width{0};
    std::uint32_t height{0};
};

struct WindowConfig final {
    std::string title{"Project StarForge"};
    std::uint32_t width{1280};
    std::uint32_t height{720};
    bool visible{true};
    bool vsync{true};
};

class Window {
public:
    Window() = default;
    virtual ~Window() = default;

    Window(const Window&) = delete;
    Window& operator=(const Window&) = delete;
    Window(Window&&) = delete;
    Window& operator=(Window&&) = delete;

    virtual void poll_events() = 0;
    [[nodiscard]] virtual bool should_close() const noexcept = 0;
    virtual void request_close() noexcept = 0;
    virtual void swap_buffers() = 0;
    [[nodiscard]] virtual FramebufferSize framebuffer_size() const noexcept = 0;
    [[nodiscard]] virtual InputSnapshot input() const noexcept = 0;
    [[nodiscard]] virtual GraphicsProcResolver graphics_proc_resolver() const noexcept = 0;
};

// The concrete implementation is supplied by the linked platform adapter.
[[nodiscard]] std::unique_ptr<Window> create_window(const WindowConfig& config);

} // namespace starforge::platform
