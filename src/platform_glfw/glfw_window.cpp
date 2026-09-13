#include <starforge/platform/platform.hpp>

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#include <iostream>
#include <limits>
#include <memory>
#include <stdexcept>

namespace starforge::platform {
namespace {

void glfw_error_callback(const int error_code, const char* description) {
    std::cerr << "PLATFORM.GLFW_ERROR code=" << error_code << " message="
              << (description != nullptr ? description : "<none>") << '\n';
}

class GlfwWindow final : public Window {
public:
    explicit GlfwWindow(const WindowConfig& config) {
        // Validate inputs before touching GLFW process-global state so an
        // exception cannot strand a successful glfwInit() without a matching
        // glfwTerminate().
        const auto width = checked_dimension(config.width);
        const auto height = checked_dimension(config.height);

        glfwSetErrorCallback(&glfw_error_callback);
        if (glfwInit() != GLFW_TRUE) {
            throw std::runtime_error("PLATFORM.GLFW_INITIALIZATION_FAILED");
        }

        glfwDefaultWindowHints();
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GLFW_TRUE);
        glfwWindowHint(GLFW_VISIBLE, config.visible ? GLFW_TRUE : GLFW_FALSE);
        glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);

        window_ = glfwCreateWindow(width, height, config.title.c_str(), nullptr, nullptr);
        if (window_ == nullptr) {
            glfwTerminate();
            throw std::runtime_error("PLATFORM.GLFW_OPENGL_46_CORE_CONTEXT_FAILED");
        }

        glfwMakeContextCurrent(window_);
        glfwSwapInterval(config.vsync ? 1 : 0);
        glfwSetWindowUserPointer(window_, this);
        glfwSetKeyCallback(window_, &GlfwWindow::key_callback);
        refresh_input();
    }

    ~GlfwWindow() override {
        if (window_ != nullptr) {
            glfwDestroyWindow(window_);
            window_ = nullptr;
        }
        glfwTerminate();
    }

    void poll_events() override {
        glfwPollEvents();
        refresh_input();
    }

    [[nodiscard]] bool should_close() const noexcept override {
        return glfwWindowShouldClose(window_) == GLFW_TRUE;
    }

    void request_close() noexcept override {
        glfwSetWindowShouldClose(window_, GLFW_TRUE);
    }

    void swap_buffers() override {
        glfwSwapBuffers(window_);
    }

    [[nodiscard]] FramebufferSize framebuffer_size() const noexcept override {
        int width = 0;
        int height = 0;
        glfwGetFramebufferSize(window_, &width, &height);
        return FramebufferSize{
            .width = width > 0 ? static_cast<std::uint32_t>(width) : 0U,
            .height = height > 0 ? static_cast<std::uint32_t>(height) : 0U,
        };
    }

    [[nodiscard]] InputSnapshot input() const noexcept override {
        return input_;
    }

    [[nodiscard]] GraphicsProcResolver graphics_proc_resolver() const noexcept override {
        return &GlfwWindow::resolve_proc;
    }

private:
    static int checked_dimension(const std::uint32_t value) {
        constexpr auto max_int = static_cast<std::uint32_t>(std::numeric_limits<int>::max());
        if (value == 0 || value > max_int) {
            throw std::invalid_argument("PLATFORM.GLFW_INVALID_WINDOW_DIMENSION");
        }
        return static_cast<int>(value);
    }

    static GraphicsProcAddress resolve_proc(const char* name) {
        return glfwGetProcAddress(name);
    }

    static void key_callback(GLFWwindow* window, int, int, int, int) {
        auto* self = static_cast<GlfwWindow*>(glfwGetWindowUserPointer(window));
        if (self != nullptr) {
            self->refresh_input();
        }
    }

    void refresh_input() noexcept {
        input_.escape = glfwGetKey(window_, GLFW_KEY_ESCAPE) == GLFW_PRESS;
        input_.w = glfwGetKey(window_, GLFW_KEY_W) == GLFW_PRESS;
        input_.a = glfwGetKey(window_, GLFW_KEY_A) == GLFW_PRESS;
        input_.s = glfwGetKey(window_, GLFW_KEY_S) == GLFW_PRESS;
        input_.d = glfwGetKey(window_, GLFW_KEY_D) == GLFW_PRESS;
    }

    GLFWwindow* window_{nullptr};
    InputSnapshot input_{};
};

} // namespace

std::unique_ptr<Window> create_window(const WindowConfig& config) {
    return std::make_unique<GlfwWindow>(config);
}

} // namespace starforge::platform
