#pragma once
#include "core/boid.hpp"
#include <vector>

// ============================================================
// Flock 类 —— 鸟群管理器（声明）
//
// 管理所有 Boid 个体，每帧对每只 Boid 应用三大规则：
//   1. 分离 (Separation): 避免与邻居太近
//   2. 对齐 (Alignment):  朝向邻居的平均速度方向
//   3. 凝聚 (Cohesion):   朝向邻居的平均位置（重心）
//
// 数据布局：std::vector<Boid>（扁平数组）
//   Boid 对象在内存中连续排列，遍历时 CPU Cache 命中率高
//   对比 vector<unique_ptr<Boid>>：每个指针指向堆上不同位置
//   遍历时频繁 cache miss，性能差数倍
//
// 时间复杂度: O(N²) —— 每只 Boid 需要遍历所有其他 Boid
// ============================================================

class Flock {
public:
    // 扁平数组：Boid 对象连续存储，cache 友好
    std::vector<Boid> boids;

    // ────────────────────────────────────────────────────────────
    // 感知半径设计：
    //   对齐/凝聚用大半径（100）→ 松散跟随群体大方向
    //   分离用小半径（30）      → 只对"太近"的邻居排斥
    // ────────────────────────────────────────────────────────────
    static constexpr float PERCEPTION_RADIUS = 100.0f;
    static constexpr float SEPARATION_RADIUS = 30.0f;

    // ────────────────────────────────────────────────────────────
    // 权重：sep >> ali > coh，分离力压倒性优势防止抱团结晶
    // ────────────────────────────────────────────────────────────
    static constexpr float SEPARATION_WEIGHT = 2.8f;
    static constexpr float ALIGNMENT_WEIGHT  = 0.5f;
    static constexpr float COHESION_WEIGHT   = 0.4f;

    Flock(float w, float h, int count = 150);

    void update();
    void draw() const;

private:
    float screenWidth;
    float screenHeight;

    // 分离力分母中的 epsilon，防止 d≈0 时力爆炸
    static constexpr float SEPARATION_EPSILON = 0.01f;

    Vector2 separation(const Boid& boid) const;
    Vector2 alignment(const Boid& boid) const;
    Vector2 cohesion(const Boid& boid) const;
};
