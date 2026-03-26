#include "core/boid.hpp"
#include "math/vector_math.hpp"
#include <cmath>

Boid::Boid(float x, float y)
    : position{ x, y }
    , acceleration{ 0.0f, 0.0f }
{
    // 随机初始速度：角度随机，速率在 [2, MAX_SPEED] 之间
    float angle = static_cast<float>(GetRandomValue(0, 360)) * DEG2RAD;
    float speed = static_cast<float>(GetRandomValue(200, 400)) / 100.0f;
    velocity = { std::cos(angle) * speed, std::sin(angle) * speed };
    displayAngle = angle;  // 初始显示角度 = 初始速度方向
}

// 施加一个力（加速度累加）
// 模拟牛顿第二定律: F = ma，简化 m=1，所以 a = F
void Boid::applyForce(Vector2 force) {
    acceleration += force;
}

void Boid::update() {
    velocity += acceleration;
    velocity = vec2Limit(velocity, MAX_SPEED);
    position += velocity;
    acceleration = { 0.0f, 0.0f };

    // ============================================
    // 平滑角度插值 (Smooth Angle Lerp)
    //
    // 问题：直接用 atan2(vy, vx) 作为渲染角度会导致三角形
    //       在速度微小波动时剧烈抖动（视觉噪声）
    //
    // 解决：维护一个 displayAngle，每帧向目标角度
    //       插值一小步，而非直接跳到目标角度
    //
    // 数学：
    //   targetAngle = atan2(vy, vx)       // 速度方向的真实角度
    //   diff = targetAngle - displayAngle  // 角度差
    //
    //   关键：角度差必须归一化到 [-PI, PI] 范围！
    //   否则从 +170° 到 -170° 会走 340° 的大弯
    //   而不是走 20° 的捷径（穿过±180°边界）
    //
    //   最终: displayAngle += diff * ANGLE_LERP
    // ============================================
    float targetAngle = std::atan2(velocity.y, velocity.x);
    float diff = targetAngle - displayAngle;

    // 角度差归一化到 [-PI, PI]，确保走最短路径
    while (diff > PI)  diff -= 2.0f * PI;
    while (diff < -PI) diff += 2.0f * PI;

    displayAngle += diff * ANGLE_LERP;
}

void Boid::wrapEdges(float screenWidth, float screenHeight) {
    if (position.x < 0.0f)           position.x += screenWidth;
    if (position.x > screenWidth)    position.x -= screenWidth;
    if (position.y < 0.0f)           position.y += screenHeight;
    if (position.y > screenHeight)   position.y -= screenHeight;
}

void Boid::draw() const {
    // 更小更纤细的三角形
    constexpr float SIZE = 6.0f;
    constexpr float BACK_ANGLE = 145.0f * DEG2RAD;  // 后方展开角度
    constexpr float BACK_SCALE = 0.45f;              // 后方缩小比例

    // 使用平滑后的 displayAngle 而非瞬时速度角度
    float angle = displayAngle;

    // ============================================
    // 三角形三个顶点的计算：
    //   p1: 尖端（前方），沿 displayAngle 方向
    //   p2: 左后方，旋转 +BACK_ANGLE
    //   p3: 右后方，旋转 -BACK_ANGLE
    // ============================================
    Vector2 p1 = {
        position.x + std::cos(angle) * SIZE,
        position.y + std::sin(angle) * SIZE
    };
    Vector2 p2 = {
        position.x + std::cos(angle + BACK_ANGLE) * SIZE * BACK_SCALE,
        position.y + std::sin(angle + BACK_ANGLE) * SIZE * BACK_SCALE
    };
    Vector2 p3 = {
        position.x + std::cos(angle - BACK_ANGLE) * SIZE * BACK_SCALE,
        position.y + std::sin(angle - BACK_ANGLE) * SIZE * BACK_SCALE
    };

    DrawTriangle(p1, p3, p2, WHITE);  // Raylib 要求逆时针顶点顺序
}
