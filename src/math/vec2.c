#include "math/math.h"

#include "math/math.h"

f32 vec2_length_2(vec2 v1)
{
    return (v1.x * v1.x + v1.y * v1.y);
}

f32 vec2_length(vec2 v)
{
    return sqrt((v.x * v.x) + (v.y * v.y));
}

vec2 vec2_add(vec2 v1, vec2 v2)
{
    return (vec2) { v1.x + v2.x, v1.y + v2.y};
}

vec2 vec2_subtract(vec2 v1, vec2 v2)
{
    return (vec2) { v1.x - v2.x, v1.y - v2.y};
}


vec2 vec2_multiply_f32(vec2 v, f32 s)
{
    return (vec2) { v.x* s, v.y* s};
}

f32 vec2_dot(vec2 v1, vec2 v2)
{
    return v1.x * v2.x + v1.y * v2.y;
}

vec2 vec2_normalize(vec2 v)
{
    f32 length = vec2_length(v);
    return (vec2) { v.x / length, v.y / length};
}

vec2 vec2_divide(vec2 v1, vec2 v2)
{
    return (vec2) { v1.x / v2.x, v1.y / v2.y};
}
vec2 vec2_divide_f32(vec2 v1, f32 f)
{
    return (vec2) { v1.x / f, v1.y / f};
}
vec2 vec2_multiply(vec2 v1, vec2 v2)
{
    return (vec2) { v1.x* v2.x, v1.y* v2.y};
}

vec2 vec2_lerp(vec2 v1, vec2 v2, f32 f)
{
    vec2 result;
    result.x = lerp(v1.x, v2.x, f);
    result.y = lerp(v1.y, v2.y, f);

    return result;
}
vec2 vec2_clamp(vec2 v, vec2 min, vec2 max)
{
    vec2 result = v;
    if (v.x < min.x)
        result.x = min.x;
    else  if (v.x > max.x)
        result.x = max.x;

    if (v.y < min.y)
        result.y = min.y;
    else if (v.y > max.y)
        result.y = max.y;

    return result;
}
vec2 vec2_negate(vec2 v)
{
    return (vec2) { -v.x, -v.y};
}
vec2 vec2_random_f32(f32 min, f32 max)
{
    return (vec2) { f32_random(min, max), f32_random(min, max)};
}

vec2 vec2_round(vec2 v)
{
    return (vec2){round(v.x),round(v.y)};
}


void vec2_print(vec2 vector)
{
    printf("V3: x:%f y:%f\n", vector.x, vector.y);
}
