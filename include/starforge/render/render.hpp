#pragma once

#include <cstdint>
#include <memory>

namespace starforge::render {

using GraphicsProcAddress = void (*)();
using GraphicsProcResolver = GraphicsProcAddress (*)(const char* name);

struct Color final {
    float r{0.0F};
    float g{0.0F};
    float b{0.0F};
    float a{1.0F};
};

struct RenderFrame final {
    std::uint32_t framebuffer_width{0};
    std::uint32_t framebuffer_height{0};
    Color clear_color{0.015F, 0.02F, 0.04F, 1.0F};
};

struct RendererCapabilities final {
    std::uint32_t api_major{0};
    std::uint32_t api_minor{0};
};

struct TextureHandle final {
    std::uint32_t value{0};
    [[nodiscard]] explicit operator bool() const noexcept { return value != 0; }
};

struct SamplerHandle final {
    std::uint32_t value{0};
    [[nodiscard]] explicit operator bool() const noexcept { return value != 0; }
};

enum class TextureFormat : std::uint8_t {
    rgba8_unorm,
    rgba8_srgb,
    depth24_stencil8,
};

struct TextureDesc final {
    std::uint32_t width{1};
    std::uint32_t height{1};
    TextureFormat format{TextureFormat::rgba8_unorm};
};

struct SamplerDesc final {
    bool linear_filtering{true};
    bool repeat_u{true};
    bool repeat_v{true};
};

[[nodiscard]] bool valid_frame(const RenderFrame& frame) noexcept;
[[nodiscard]] bool valid_texture_desc(const TextureDesc& desc) noexcept;

class Renderer {
public:
    Renderer() = default;
    virtual ~Renderer() = default;

    Renderer(const Renderer&) = delete;
    Renderer& operator=(const Renderer&) = delete;
    Renderer(Renderer&&) = delete;
    Renderer& operator=(Renderer&&) = delete;

    virtual void initialize(GraphicsProcResolver resolver) = 0;
    virtual void render(const RenderFrame& frame) = 0;
    virtual void shutdown() noexcept = 0;
    [[nodiscard]] virtual RendererCapabilities capabilities() const noexcept = 0;
};

// The concrete implementation is supplied by the linked renderer adapter.
[[nodiscard]] std::unique_ptr<Renderer> create_renderer();

} // namespace starforge::render
