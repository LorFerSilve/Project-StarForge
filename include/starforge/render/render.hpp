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

class RenderSnapshot final {
public:
    constexpr RenderSnapshot() noexcept = default;

    constexpr RenderSnapshot(
        const std::uint64_t tick_index,
        const std::uint64_t scene_generation,
        const std::uint64_t origin_epoch,
        const std::uint64_t state_revision) noexcept
        : tick_index_(tick_index),
          scene_generation_(scene_generation),
          origin_epoch_(origin_epoch),
          state_revision_(state_revision) {}

    [[nodiscard]] constexpr std::uint64_t tick_index() const noexcept { return tick_index_; }
    [[nodiscard]] constexpr std::uint64_t scene_generation() const noexcept { return scene_generation_; }
    [[nodiscard]] constexpr std::uint64_t origin_epoch() const noexcept { return origin_epoch_; }
    [[nodiscard]] constexpr std::uint64_t state_revision() const noexcept { return state_revision_; }

private:
    std::uint64_t tick_index_{0};
    std::uint64_t scene_generation_{0};
    std::uint64_t origin_epoch_{0};
    std::uint64_t state_revision_{0};
};

struct RenderFrame final {
    std::uint32_t framebuffer_width{0};
    std::uint32_t framebuffer_height{0};
    Color clear_color{0.015F, 0.02F, 0.04F, 1.0F};
    std::shared_ptr<const RenderSnapshot> snapshot{};
};

struct RendererCapabilities final {
    std::uint32_t api_major{0};
    std::uint32_t api_minor{0};
    bool core_profile{false};
    bool debug_output{false};
    bool reversed_z{false};
};

struct RendererDiagnostics final {
    bool initialized{false};
    bool debug_callback_active{false};
    bool reversed_z_active{false};
    bool last_frame_clean{true};
    std::uint32_t live_gpu_objects{0};
    std::uint64_t rendered_frames{0};
    std::uint64_t debug_messages{0};
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
    [[nodiscard]] virtual RendererDiagnostics diagnostics() const noexcept = 0;
};

// The concrete implementation is supplied by the linked renderer adapter.
[[nodiscard]] std::unique_ptr<Renderer> create_renderer();

} // namespace starforge::render