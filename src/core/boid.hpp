#pragma once
#include "raylib.h"

// ============================================================
// Boid 类 —— 单个鸟群个体（声明）
// 每只 Boid 拥有位置、速度、加速度三个向量，以及一个平滑显示角度
//
// 物理模型（牛顿运动学）:
//   加速度 → 改变速度 → 改变位置
//   每帧: velocity += acceleration
//          position += velocity
//          acceleration 清零（等待下一帧重新计算）
// ============================================================

class Boid {
public:
    Vector2 position;      // 当前位置
    Vector2 velocity;      // 当前速度（含方向和大小）
    Vector2 acceleration;  // 当前加速度（每帧由三大规则叠加得到）
    float displayAngle;    // 平滑显示角度（用于渲染，不直接跟随瞬时速度）

    // Boid 运动参数
    // ────────────────────────────────────────────────
    // MAX_FORCE 设为 0.15：
    //   过高会导致 boid 频繁超调（overshoot）产生抖动
    //   0.15 让转向更柔和，减少锯齿运动
    // ────────────────────────────────────────────────
    static constexpr float MAX_SPEED = 3.5f;
    static constexpr float MAX_FORCE = 0.15f;

    // ────────────────────────────────────────────────
    // 角度插值系数 (Lerp factor)
    //   0.1 表示每帧只移动 10% 的角度差
    //   约 10 帧追上目标角度
    // ────────────────────────────────────────────────
    static constexpr float ANGLE_LERP = 0.1f;

    Boid(float x, float y);

    void applyForce(Vector2 force);
    void update();
    void wrapEdges(float screenWidth, float screenHeight);
    void draw() const;
};
