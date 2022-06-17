#pragma once

#include <glm/gtx/string_cast.hpp>
#include <glm/mat4x4.hpp>
#include <glm/matrix.hpp>
#include <iostream>

namespace mygl
{
    class matrix4
    {
    public:
        glm::mat4 mat;

        matrix4();
        matrix4(const float &a, const float &b, const float &c, const float &d,
                const float &e, const float &f, const float &g, const float &h,
                const float &i, const float &j, const float &k, const float &l,
                const float &m, const float &n, const float &o, const float &p);

        void operator*=(const matrix4 &rhs)
        {
            mat *= rhs.mat;
        }

        static matrix4 identity()
        {
            matrix4 res = matrix4();
            res.mat = glm::mat4(1.0);
            return res;
        }

        void frustum(const float &left, const float &right, const float &bottom,
                     const float &top, const float &znear, const float &far);

        void lookat(const float &eyeX, const float &eyeY, const float &eyeZ,
                    const float &centerX, const float &centerY,
                    const float &centerZ, float upX, float upY, float upZ);

    private:
    };

    inline std::ostream &operator<<(std::ostream &out, const matrix4 &m)
    {
        return out << "matrix =\n" << glm::to_string(m.mat) << std::endl;
    }

} // namespace mygl