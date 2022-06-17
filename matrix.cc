#include "matrix.hh"

namespace mygl
{
    matrix4::matrix4()
    {
        mat = glm::mat4();
    }

    matrix4::matrix4(const float &a, const float &b, const float &c,
                     const float &d, const float &e, const float &f,
                     const float &g, const float &h, const float &i,
                     const float &j, const float &k, const float &l,
                     const float &m, const float &n, const float &o,
                     const float &p)
    {
        mat = glm::mat4(a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p);
    }

    void matrix4::frustum(const float &left, const float &right,
                          const float &bottom, const float &top,
                          const float &znear, const float &far)
    {
        auto A = (right + left) / (right - left);
        auto B = (top + bottom) / (top - bottom);
        auto C = -(far + znear) / (far - znear);
        auto D = -2 * far * znear / (far - znear);

        auto first = 2 * znear / (right - left);
        auto second = 2 * znear / (top - bottom);

        mat = glm::mat4(first, 0.0, 0.0, 0.0, 0.0, second, 0.0, 0.0, A, B, C,
                        -1, 0.0, 0.0, D, 0.0);
    }

    void matrix4::lookat(const float &eyeX, const float &eyeY,
                         const float &eyeZ, const float &centerX,
                         const float &centerY, const float &centerZ, float upX,
                         float upY, float upZ)
    {
        auto F = glm::vec3(centerX - eyeX, centerY - eyeY, centerZ - eyeZ);
        auto f = glm::normalize(F);
        auto up = glm::normalize(glm::vec3(upX, upY, upZ));

        auto s = glm::cross(f, up);
        auto u = glm::cross(s, f);

        mat = glm::mat4(s[0], u[0], -f[0], 0, s[1], u[1], -f[1], 0, s[2], u[2],
                        -f[2], 0, -eyeX, -eyeY, -eyeZ, 1);
    }

} // namespace mygl