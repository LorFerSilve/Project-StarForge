#include <starforge/render/render.hpp>

#include <glad/gl.h>

#include <array>
#include <cstddef>
#include <cstdint>
#include <memory>
#include <stdexcept>
#include <string>
#include <utility>

namespace starforge::render {
namespace {

enum class GlObjectKind : std::uint8_t {
    shader,
    program,
    vertex_array,
    buffer,
};

class UniqueGlObject final {
public:
    explicit UniqueGlObject(const GlObjectKind kind) noexcept : kind_(kind) {}
    UniqueGlObject(const GlObjectKind kind, const GLuint id) noexcept : kind_(kind), id_(id) {}
    ~UniqueGlObject() { reset(); }

    UniqueGlObject(const UniqueGlObject&) = delete;
    UniqueGlObject& operator=(const UniqueGlObject&) = delete;

    UniqueGlObject(UniqueGlObject&& other) noexcept : kind_(other.kind_), id_(std::exchange(other.id_, 0U)) {}

    UniqueGlObject& operator=(UniqueGlObject&& other) noexcept {
        if (this != &other) {
            reset();
            kind_ = other.kind_;
            id_ = std::exchange(other.id_, 0U);
        }
        return *this;
    }

    [[nodiscard]] GLuint get() const noexcept { return id_; }

    void assign(const GLuint id) {
        reset();
        id_ = id;
    }

    void reset() noexcept {
        if (id_ == 0U) {
            return;
        }

        switch (kind_) {
        case GlObjectKind::shader:
            glDeleteShader(id_);
            break;
        case GlObjectKind::program:
            glDeleteProgram(id_);
            break;
        case GlObjectKind::vertex_array:
            glDeleteVertexArrays(1, &id_);
            break;
        case GlObjectKind::buffer:
            glDeleteBuffers(1, &id_);
            break;
        }
        id_ = 0U;
    }

private:
    GlObjectKind kind_;
    GLuint id_{0U};
};

[[nodiscard]] UniqueGlObject compile_shader(const GLenum type, const char* source) {
    UniqueGlObject shader{GlObjectKind::shader, glCreateShader(type)};
    if (shader.get() == 0U) {
        throw std::runtime_error("RENDER.OPENGL_SHADER_CREATE_FAILED");
    }

    const GLuint shader_id = shader.get();
    glShaderSource(shader_id, 1, &source, nullptr);
    glCompileShader(shader_id);

    GLint compiled = GL_FALSE;
    glGetShaderiv(shader_id, GL_COMPILE_STATUS, &compiled);
    if (compiled == GL_TRUE) {
        return shader;
    }

    GLint length = 0;
    glGetShaderiv(shader_id, GL_INFO_LOG_LENGTH, &length);
    std::string log(static_cast<std::size_t>(length > 1 ? length : 1), '\0');
    glGetShaderInfoLog(shader_id, length, nullptr, log.data());
    throw std::runtime_error("RENDER.OPENGL_SHADER_COMPILE_FAILED: " + log);
}

[[nodiscard]] UniqueGlObject create_debug_program() {
    constexpr auto vertex_source = R"GLSL(#version 460 core
layout(location = 0) in vec2 in_position;
void main() {
    gl_Position = vec4(in_position, 0.5, 1.0);
}
)GLSL";

    constexpr auto fragment_source = R"GLSL(#version 460 core
layout(location = 0) out vec4 out_color;
void main() {
    out_color = vec4(0.15, 0.72, 1.0, 1.0);
}
)GLSL";

    auto vertex = compile_shader(GL_VERTEX_SHADER, vertex_source);
    auto fragment = compile_shader(GL_FRAGMENT_SHADER, fragment_source);
    UniqueGlObject program{GlObjectKind::program, glCreateProgram()};
    if (program.get() == 0U) {
        throw std::runtime_error("RENDER.OPENGL_PROGRAM_CREATE_FAILED");
    }

    glAttachShader(program.get(), vertex.get());
    glAttachShader(program.get(), fragment.get());
    glLinkProgram(program.get());

    GLint linked = GL_FALSE;
    glGetProgramiv(program.get(), GL_LINK_STATUS, &linked);
    if (linked == GL_TRUE) {
        return program;
    }

    GLint length = 0;
    glGetProgramiv(program.get(), GL_INFO_LOG_LENGTH, &length);
    std::string log(static_cast<std::size_t>(length > 1 ? length : 1), '\0');
    glGetProgramInfoLog(program.get(), length, nullptr, log.data());
    throw std::runtime_error("RENDER.OPENGL_PROGRAM_LINK_FAILED: " + log);
}

[[nodiscard]] bool drain_gl_errors() noexcept {
    bool clean = true;
    while (glGetError() != GL_NO_ERROR) {
        clean = false;
    }
    return clean;
}

class OpenGlRenderer final : public Renderer {
public:
    OpenGlRenderer()
        : debug_program_(GlObjectKind::program),
          debug_vao_(GlObjectKind::vertex_array),
          debug_vertex_buffer_(GlObjectKind::buffer) {}

    ~OpenGlRenderer() override { shutdown(); }

    void initialize(const GraphicsProcResolver resolver) override {
        if (resolver == nullptr) {
            throw std::invalid_argument("RENDER.OPENGL_NULL_PROC_RESOLVER");
        }
        if (initialized_) {
            throw std::logic_error("RENDER.OPENGL_DOUBLE_INITIALIZE");
        }

        const int version = gladLoadGL(resolver);
        if (version == 0) {
            throw std::runtime_error("RENDER.OPENGL_GLAD_LOAD_FAILED");
        }
        gl_loaded_ = true;

        try {
            capabilities_.api_major = static_cast<std::uint32_t>(GLAD_VERSION_MAJOR(version));
            capabilities_.api_minor = static_cast<std::uint32_t>(GLAD_VERSION_MINOR(version));
            if (capabilities_.api_major < 4U ||
                (capabilities_.api_major == 4U && capabilities_.api_minor < 6U)) {
                throw std::runtime_error("RENDER.OPENGL_46_REQUIRED");
            }

            GLint profile_mask = 0;
            glGetIntegerv(GL_CONTEXT_PROFILE_MASK, &profile_mask);
            capabilities_.core_profile = (profile_mask & GL_CONTEXT_CORE_PROFILE_BIT) != 0;
            if (!capabilities_.core_profile) {
                throw std::runtime_error("RENDER.OPENGL_CORE_PROFILE_REQUIRED");
            }

            static_cast<void>(drain_gl_errors());

            glEnable(GL_DEBUG_OUTPUT);
            glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
            glDebugMessageCallback(&OpenGlRenderer::debug_callback, this);
            glDebugMessageControl(
                GL_DONT_CARE,
                GL_DONT_CARE,
                GL_DEBUG_SEVERITY_NOTIFICATION,
                0,
                nullptr,
                GL_FALSE);
            debug_callback_registered_ = true;
            capabilities_.debug_output = true;

            glClipControl(GL_LOWER_LEFT, GL_ZERO_TO_ONE);
            glEnable(GL_DEPTH_TEST);
            glDepthFunc(GL_GREATER);
            glClearDepth(0.0);
            capabilities_.reversed_z = true;

            GLuint vao = 0U;
            glCreateVertexArrays(1, &vao);
            if (vao == 0U) {
                throw std::runtime_error("RENDER.OPENGL_VAO_CREATE_FAILED");
            }
            debug_vao_.assign(vao);

            constexpr std::array<float, 6> positions{
                -0.55F, -0.45F,
                0.55F, -0.45F,
                0.0F, 0.55F,
            };

            GLuint vertex_buffer = 0U;
            glCreateBuffers(1, &vertex_buffer);
            if (vertex_buffer == 0U) {
                throw std::runtime_error("RENDER.OPENGL_BUFFER_CREATE_FAILED");
            }
            debug_vertex_buffer_.assign(vertex_buffer);
            glNamedBufferStorage(
                debug_vertex_buffer_.get(),
                static_cast<GLsizeiptr>(sizeof(positions)),
                positions.data(),
                0);

            glVertexArrayVertexBuffer(
                debug_vao_.get(),
                0,
                debug_vertex_buffer_.get(),
                0,
                static_cast<GLsizei>(2U * sizeof(float)));
            glEnableVertexArrayAttrib(debug_vao_.get(), 0);
            glVertexArrayAttribFormat(debug_vao_.get(), 0, 2, GL_FLOAT, GL_FALSE, 0);
            glVertexArrayAttribBinding(debug_vao_.get(), 0, 0);

            debug_program_ = create_debug_program();

            if (!drain_gl_errors()) {
                throw std::runtime_error("RENDER.OPENGL_INITIALIZATION_ERROR");
            }

            initialized_ = true;
            diagnostics_.initialized = true;
            diagnostics_.debug_callback_active = true;
            diagnostics_.reversed_z_active = true;
            diagnostics_.live_gpu_objects = 3U;
        } catch (...) {
            shutdown();
            throw;
        }
    }

    void render(const RenderFrame& frame) override {
        if (!initialized_) {
            throw std::logic_error("RENDER.OPENGL_RENDER_BEFORE_INITIALIZE");
        }
        if (!valid_frame(frame)) {
            return;
        }

        static_cast<void>(drain_gl_errors());

        glViewport(0, 0, static_cast<GLsizei>(frame.framebuffer_width), static_cast<GLsizei>(frame.framebuffer_height));
        glClearColor(frame.clear_color.r, frame.clear_color.g, frame.clear_color.b, frame.clear_color.a);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glUseProgram(debug_program_.get());
        glBindVertexArray(debug_vao_.get());
        glDrawArrays(GL_TRIANGLES, 0, 3);
        glBindVertexArray(0);
        glUseProgram(0);

        diagnostics_.last_frame_clean = drain_gl_errors();
        ++diagnostics_.rendered_frames;
        if (!diagnostics_.last_frame_clean) {
            throw std::runtime_error("RENDER.OPENGL_FRAME_ERROR");
        }
    }

    void shutdown() noexcept override {
        if (!gl_loaded_) {
            return;
        }

        if (debug_callback_registered_) {
            glDebugMessageCallback(nullptr, nullptr);
            debug_callback_registered_ = false;
        }

        debug_program_.reset();
        debug_vao_.reset();
        debug_vertex_buffer_.reset();

        initialized_ = false;
        diagnostics_.initialized = false;
        diagnostics_.debug_callback_active = false;
        diagnostics_.reversed_z_active = false;
        diagnostics_.live_gpu_objects = 0U;
        capabilities_ = {};
        gl_loaded_ = false;
    }

    [[nodiscard]] RendererCapabilities capabilities() const noexcept override {
        return capabilities_;
    }

    [[nodiscard]] RendererDiagnostics diagnostics() const noexcept override {
        return diagnostics_;
    }

private:
    static void GLAPIENTRY debug_callback(
        GLenum,
        GLenum,
        GLuint,
        GLenum severity,
        GLsizei,
        const GLchar*,
        const void* user_data) {
        if (severity == GL_DEBUG_SEVERITY_NOTIFICATION || user_data == nullptr) {
            return;
        }
        auto* self = static_cast<OpenGlRenderer*>(const_cast<void*>(user_data));
        ++self->diagnostics_.debug_messages;
    }

    bool gl_loaded_{false};
    bool initialized_{false};
    bool debug_callback_registered_{false};
    UniqueGlObject debug_program_;
    UniqueGlObject debug_vao_;
    UniqueGlObject debug_vertex_buffer_;
    RendererCapabilities capabilities_{};
    RendererDiagnostics diagnostics_{};
};

} // namespace

std::unique_ptr<Renderer> create_renderer() {
    return std::make_unique<OpenGlRenderer>();
}

} // namespace starforge::render