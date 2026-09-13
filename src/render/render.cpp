#include <starforge/render/render.hpp>

namespace starforge::render {

bool valid_frame(const RenderFrame& frame) noexcept {
    return frame.framebuffer_width > 0U && frame.framebuffer_height > 0U;
}

bool valid_texture_desc(const TextureDesc& desc) noexcept {
    return desc.width > 0U && desc.height > 0U;
}

} // namespace starforge::render
