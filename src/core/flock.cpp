#include "core/flock.hpp"
#include "math/vector_math.hpp"

Flock::Flock(float w, float h, int count)
    : screenWidth(w), screenHeight(h)
{
    boids.reserve(count);
    for (int i = 0; i < count; ++i) {
        float x = static_cast<float>(GetRandomValue(0, static_cast<int>(w)));
        float y = static_cast<float>(GetRandomValue(0, static_cast<int>(h)));
        boids.emplace_back(x, y);
    }
}

void Flock::update() {
    // ============================================
    // O(N²) 核心循环：
    // 对于每只 Boid，遍历所有其他 Boid 计算三大规则
    // ============================================
    for (auto& boid : boids) {
        Vector2 separationForce = separation(boid);
        Vector2 alignmentForce  = alignment(boid);
        Vector2 cohesionForce   = cohesion(boid);

        // 施加加权后的力
        boid.applyForce(separationForce * SEPARATION_WEIGHT);
        boid.applyForce(alignmentForce  * ALIGNMENT_WEIGHT);
        boid.applyForce(cohesionForce   * COHESION_WEIGHT);

        boid.update();
        boid.wrapEdges(screenWidth, screenHeight);
    }
}

void Flock::draw() const {
    for (const auto& boid : boids) {
        boid.draw();
    }
}

// ============================================================
// 规则一：分离 (Separation) —— 防结晶的核心
//
// 使用 1/(d² + ε) 平方反比力 + epsilon 防爆
//
// 为什么需要 ε (epsilon = 0.01)?
//   当两只 boid 几乎重叠时，d ≈ 0，1/d² → 无穷大
//   这会产生"力爆炸"：一帧内加速度飙到极大值
//   加上 ε 后：1/(0² + 0.01) = 100（有界）
//   而正常距离下：1/(10² + 0.01) ≈ 0.01（几乎无影响）
// ============================================================
Vector2 Flock::separation(const Boid& boid) const {
    Vector2 steer = { 0.0f, 0.0f };
    int count = 0;

    for (const auto& other : boids) {
        float d = vec2Distance(boid.position, other.position);
        if (d > 0.0f && d < SEPARATION_RADIUS) {
            Vector2 diff = boid.position - other.position;
            // 平方反比 + epsilon 防爆：1/(d² + ε)
            // 确保分母永远不会趋近于零
            diff = vec2Normalize(diff) / (d * d + SEPARATION_EPSILON);
            steer += diff;
            count++;
        }
    }

    if (count > 0) {
        steer /= static_cast<float>(count);
        steer = vec2SetMagnitude(steer, Boid::MAX_SPEED);
        steer -= boid.velocity;
        steer = vec2Limit(steer, Boid::MAX_FORCE);
    }

    return steer;
}

// ============================================================
// 规则二：对齐 (Alignment)
//
// 目的：让 Boid 朝向邻居群体的平均飞行方向
// 直觉：如果周围的鸟都往北飞，我也想往北飞
// ============================================================
Vector2 Flock::alignment(const Boid& boid) const {
    Vector2 avgVelocity = { 0.0f, 0.0f };
    int count = 0;

    for (const auto& other : boids) {
        float d = vec2Distance(boid.position, other.position);
        if (d > 0.0f && d < PERCEPTION_RADIUS) {
            avgVelocity += other.velocity;
            count++;
        }
    }

    if (count > 0) {
        avgVelocity /= static_cast<float>(count);
        avgVelocity = vec2SetMagnitude(avgVelocity, Boid::MAX_SPEED);
        Vector2 steer = avgVelocity - boid.velocity;
        return vec2Limit(steer, Boid::MAX_FORCE);
    }

    return { 0.0f, 0.0f };
}

// ============================================================
// 规则三：凝聚 (Cohesion)
//
// 目的：让 Boid 朝向邻居群体的中心（重心）靠拢
// 直觉：飞向群体的中心，不要落单
// ============================================================
Vector2 Flock::cohesion(const Boid& boid) const {
    Vector2 centerOfMass = { 0.0f, 0.0f };
    int count = 0;

    for (const auto& other : boids) {
        float d = vec2Distance(boid.position, other.position);
        if (d > 0.0f && d < PERCEPTION_RADIUS) {
            centerOfMass += other.position;
            count++;
        }
    }

    if (count > 0) {
        centerOfMass /= static_cast<float>(count);
        Vector2 desired = centerOfMass - boid.position;
        desired = vec2SetMagnitude(desired, Boid::MAX_SPEED);
        Vector2 steer = desired - boid.velocity;
        return vec2Limit(steer, Boid::MAX_FORCE);
    }

    return { 0.0f, 0.0f };
}
