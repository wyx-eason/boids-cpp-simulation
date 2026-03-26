#include "raylib.h"
#include "core/flock.hpp"
#include <memory>

/**
 * GameWindow class - Manages Raylib window lifecycle using RAII
 * Automatically initializes window in constructor and closes in destructor
 */
class GameWindow {
private:
    int width;
    int height;
    const char* title;
    bool initialized;

public:
    explicit GameWindow(int w = 1280, int h = 720, const char* t = "Boids Simulator")
        : width(w), height(h), title(t), initialized(false) {
        InitWindow(width, height, title);
        SetTargetFPS(60);
        initialized = true;
    }

    ~GameWindow() {
        if (initialized) {
            CloseWindow();
            initialized = false;
        }
    }

    GameWindow(const GameWindow&) = delete;
    GameWindow& operator=(const GameWindow&) = delete;

    GameWindow(GameWindow&& other) noexcept
        : width(other.width), height(other.height), title(other.title), initialized(other.initialized) {
        other.initialized = false;
    }

    GameWindow& operator=(GameWindow&& other) noexcept {
        if (this != &other) {
            if (initialized) CloseWindow();
            width = other.width;
            height = other.height;
            title = other.title;
            initialized = other.initialized;
            other.initialized = false;
        }
        return *this;
    }

    bool isOpen() const { return !WindowShouldClose(); }
    int getWidth() const { return width; }
    int getHeight() const { return height; }
    void beginDrawing() const { BeginDrawing(); }
    void endDrawing() const { EndDrawing(); }
    void clear(Color color = BLACK) const { ClearBackground(color); }
};

/**
 * Application - 主程序类
 * 管理窗口和鸟群，驱动更新与渲染循环
 */
class Application {
private:
    std::unique_ptr<GameWindow> window;
    std::unique_ptr<Flock> flock;

public:
    Application()
        : window(std::make_unique<GameWindow>(1280, 720, "Boids Simulator - Phase 3"))
    {
        flock = std::make_unique<Flock>(
            static_cast<float>(window->getWidth()),
            static_cast<float>(window->getHeight()),
            150
        );
    }

    void run() {
        while (window->isOpen()) {
            update();
            render();
        }
    }

private:
    void update() {
        flock->update();
    }

    void render() {
        window->beginDrawing();
        window->clear(BLACK);

        flock->draw();

        DrawText(TextFormat("Boids: %d", static_cast<int>(flock->boids.size())), 10, 10, 18, GREEN);
        DrawText(TextFormat("FPS: %d", GetFPS()), 10, 34, 16, GRAY);
        DrawText("Press ESC to exit", 10, window->getHeight() - 26, 16, GRAY);

        window->endDrawing();
    }
};

int main() {
    try {
        Application app;
        app.run();
    } catch (const std::exception& e) {
        return 1;
    }
    return 0;
}
