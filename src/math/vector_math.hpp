#pragma once
#include "raylib.h"
#include <cmath>

// ============================================================
// Vector2 运算符重载
// 基于 Raylib 的 Vector2 结构体 (含 float x, y)
// 让向量数学运算可以像写数学公式一样自然
// ============================================================

// ---------- 加法 (向量 + 向量) ----------
inline Vector2 operator+(Vector2 a, Vector2 b) {
    return { a.x + b.x, a.y + b.y };
}

// ---------- 减法 (向量 - 向量) ----------
inline Vector2 operator-(Vector2 a, Vector2 b) {
    return { a.x - b.x, a.y - b.y };
}

// ---------- 标量乘法 (向量 * 标量) ----------
inline Vector2 operator*(Vector2 v, float s) {
    return { v.x * s, v.y * s };
}

// ---------- 标量乘法 (标量 * 向量) ----------
inline Vector2 operator*(float s, Vector2 v) {
    return { v.x * s, v.y * s };
}

// ---------- 标量除法 (向量 / 标量) ----------
inline Vector2 operator/(Vector2 v, float s) {
    return { v.x / s, v.y / s };
}

// ---------- 复合赋值 += ----------
inline Vector2& operator+=(Vector2& a, Vector2 b) {
    a.x += b.x;
    a.y += b.y;
    return a;
}

// ---------- 复合赋值 -= ----------
inline Vector2& operator-=(Vector2& a, Vector2 b) {
    a.x -= b.x;
    a.y -= b.y;
    return a;
}

// ---------- 复合赋值 *= (标量) ----------
inline Vector2& operator*=(Vector2& v, float s) {
    v.x *= s;
    v.y *= s;
    return v;
}

// ---------- 复合赋值 /= (标量) ----------
inline Vector2& operator/=(Vector2& v, float s) {
    v.x /= s;
    v.y /= s;
    return v;
}

// ============================================================
// 向量工具函数
// ============================================================

// 求向量的模（长度）: |v| = sqrt(x² + y²)
inline float vec2Length(Vector2 v) {
    return std::sqrt(v.x * v.x + v.y * v.y);
}

// 求两点之间的距离: dist = |a - b|
inline float vec2Distance(Vector2 a, Vector2 b) {
    return vec2Length(a - b);
}

// 归一化：将向量缩放为单位长度（模为1）
// 如果向量长度为0，返回零向量以避免除以零
inline Vector2 vec2Normalize(Vector2 v) {
    float len = vec2Length(v);
    if (len > 0.0f) {
        return v / len;
    }
    return { 0.0f, 0.0f };
}

// 将向量的模限制在 maxLen 以内
// 如果当前长度超过 maxLen，则归一化后乘以 maxLen
inline Vector2 vec2Limit(Vector2 v, float maxLen) {
    float len = vec2Length(v);
    if (len > maxLen) {
        return vec2Normalize(v) * maxLen;
    }
    return v;
}

// 设置向量的模为指定值，保持方向不变
inline Vector2 vec2SetMagnitude(Vector2 v, float mag) {
    return vec2Normalize(v) * mag;
}
