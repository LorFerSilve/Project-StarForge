#include <starforge/render/render.hpp>

#include <glad/gl.h>

#include <memory>
#include <stdexcept>
#include <string>

namespace starforge::render {
namespace {

[[nodiscard]] GLuint compile_shader(const GLenum type, const char* source) {
    const GLuint shader = glCreateShader(type);
    glShaderSource(shader, 1, &source, nullptr);
    glCompileShader(shader);

    GLint compiled = GL_FALSE;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);
    if (compiled == GL_TRUE) {
        return shader;
    }

    GLint length = 0;
    glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &length);
    std::string log(static_cast<std::size_t>(length > 1 ? length : 1), '\0');
    glGetShaderInfoLog(shader, length, nullptr, log.data());
    glDeleteShader(shader);
    throw std::runtime_error("OpenGL shader compilation failed: " + log);
}

[[nodiscard]] GLuint create_debug_program() {
    constexpr auto vertex_source = R"GLSL(#version 460 core
const vec2 positions[3] = vec2[3](
    vec2(-0.55, -0.45),
    vec2( 0.55, -0.45),
    vec2( 0.00,  0.55)
);
void main() {
    gl_Position = vec4(positions[gl_VertexID], 0.0, 1.0);
}
)GLSL";

    constexpr auto fragment_source = R"GLSL(#version 460 core
layout(location = 0) out vec4 out_color;
void main() {
    out_color = vec4(0.15, 0.72, 1.0, 1.0);
}
)GLSL";

    const GLuint vertex = compile_shader(GL_VERTEX_SHADER, vertex_source);
    const GLuint fragment = compile_shader(GL_FRAGMENT_SHADER, fragment_source);
    const GLuint program = glCreateProgram();
    glAttachShader(program, vertex);
    glAttachShader(program, fragment);
    glLinkProgram(program);
    glDeleteShader(vertex);
    glDeleteShader(fragment);

    GLint linked = GL_FALSE;
    glGetProgramiv(program, GL_LINK_STATUS, &linked);
    if (linked == GL_TRUE) {
        return program;
    }

    GLint length = 0;
    glGetProgramiv(program, GL_INFO_LOG_LENGTH, &length);
    std::string log(static_cast<std::size_t>(length > 1 ? length : 1), '\0');
    glGetProgramInfoLog(program, length, nullptr, log.data());
    glDeleteProgram(program);
    throw std::runtime_error("OpenGL program link failed: " + log);
}

class OpenGlRenderer final : public Renderer {
public:
    ~OpenGlRenderer() override { shutdown(); }

    void initialize(const GraphicsProcResolver resolver) override {
        if (resolver == nullptr) {
            throw std::invalid_argument("OpenGL renderer requires a graphics procedure resolver");
        }
        if (initialized_) {
            throw std::logic_error("OpenGL renderer was initialized twice");
        }

        const int version = gladLoadGL(resolver);
        if (version == 0) {
            throw std::runtime_error("glad failed to load the active OpenGL context");
        }

        capabilities_.api_major = static_cast<std::uint32_t>(GLAD_VERSION_MAJOR(version));
        capabilities_.api_minor = static_cast<std::uint32_t>(GLAD_VERSION_MINOR(version));
        if (capabilities_.api_major < 4U || (capabilities_.api_major == 4U && capabilities_.api_minor < 6U)) {
            throw std::runtime_error("Project StarForge requires an OpenGL 4.6 core context");
        }

        glCreateVertexArrays(1, &debug_vao_);
        debug_program_ = create_debug_program();
        glEnable(GL_DEPTH_TEST);
        initialized_ = true;
    }

    void render(const RenderFrame& frame) override {
        if (!initialized_) {
            throw std::logic_error("OpenGL renderer must be initialized before rendering");
        }
        if (!valid_frame(frame)) {
            return;
        }

        glViewport(0, 0, static_cast<GLsizei>(frame.framebuffer_width), static_cast<GLsizei>(frame.framebuffer_height));
        glClearColor(frame.clear_color.r, frame.clear_color.g, frame.clear_color.b, frame.clear_color.a);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glUseProgram(debug_program_);
        glBindVertexArray(debug_vao_);
        glDrawArrays(GL_TRIANGLES, 0, 3);
        glBindVertexArray(0);
        glUseProgram(0);
    }

    void shutdown() noexcept override {
        if (!initialized_) {
            return;
        }
        if (debug_program_ != 0U) {
            glDeleteProgram(debug_program_);
            debug_program_ = 0U;
        }
        if (debug_vao_ != 0U) {
            glDeleteVertexArrays(1, &debug_vao_);
            debug_vao_ = 0U;
        }
        initialized_ = false;
        capabilities_ = {};
    }

    [[nodiscard]] RendererCapabilities capabilities() const noexcept override {
        return capabilities_;
    }

private:
    bool initialized_{false};
    GLuint debug_vao_{0};
    GLuint debug_program_{0};
    RendererCapabilities capabilities_{};
};

} // namespace

std::unique_ptr<Renderer> create_renderer() {
    return std::make_unique<OpenGlRenderer>();
}

} // namespace starforge::render
