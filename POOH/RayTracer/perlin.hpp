#ifndef PERLIN_H
#define PERLIN_H

#include "ray_tracing_common.hpp"

class perlin {
  public:
    perlin() {
        ranvec = new vec3[point_count];
        // Allocate memory for the random vector array
        for (int i = 0; i < point_count; ++i) {
            // Loop over the random vector array
            ranvec[i] = unit_vector(vec3::random(-1,1));
            // Generate a random vector and store it in the array
        }

        perm_x = perlin_generate_perm();
        // Generate the permutation array for the x direction
        perm_y = perlin_generate_perm();
        // Generate the permutation array for the y direction
        perm_z = perlin_generate_perm();
        // Generate the permutation array for the z direction
    }

    ~perlin() {
        // Destructor for the perlin class
        delete[] ranvec;
        delete[] perm_x;
        delete[] perm_y;
        delete[] perm_z;
    }

    double noise(const point3& p) const {
        // Compute the noise value at a given point
        auto u = p.x() - floor(p.x());
        auto v = p.y() - floor(p.y());
        auto w = p.z() - floor(p.z());

        auto i = static_cast<int>(floor(p.x()));
        // Compute the integer part of the x coordinate
        auto j = static_cast<int>(floor(p.y()));
        // Compute the integer part of the y coordinate
        auto k = static_cast<int>(floor(p.z()));
        // Compute the integer part of the z coordinate
        vec3 c[2][2][2];
        // Initialize the corner values

        for (int di=0; di < 2; di++)
            for (int dj=0; dj < 2; dj++)
                for (int dk=0; dk < 2; dk++)
                    c[di][dj][dk] = ranvec[
                        // Compute the random vector index
                        perm_x[(i+di) & 255] ^
                        perm_y[(j+dj) & 255] ^
                        perm_z[(k+dk) & 255]
                    ];

        return perlin_interp(c, u, v, w);
        // Return the interpolated noise value
    }

    double turb(const point3& p, int depth=7) const {
        // Compute the turbulence value at a given point
        auto accum = 0.0;
        // Initialize the accumulator
        auto temp_p = p;
        // Initialize the temporary point
        auto weight = 1.0;
        // Initialize the weight

        for (int i = 0; i < depth; i++) {
            accum += weight*noise(temp_p);
            // Accumulate the noise value
            weight *= 0.5;
            // Update the weight
            temp_p *= 2;
            // Update the temporary point
        }

        return fabs(accum);
        // Return the absolute value of the accumulated noise
    }

  private:
    static const int point_count = 256;
    // Set the number of points in the perlin noise array
    vec3* ranvec;
    int* perm_x;
    // Permutation array for the x direction
    int* perm_y;
    // Permutation array for the y direction
    int* perm_z;
    // Permutation array for the z direction

    static int* perlin_generate_perm() {
        // Generate the permutation array for the perlin noise
        auto p = new int[point_count];
        // Allocate memory for the permutation array

        for (int i = 0; i < perlin::point_count; i++)
        // Loop over the permutation array
            p[i] = i;

        permute(p, point_count);
        // Permute the permutation array

        return p;
    }

    static void permute(int* p, int n) {
        // Permute the permutation array
        for (int i = n-1; i > 0; i--) {
            int target = random_int(0, i);
            int tmp = p[i];
            p[i] = p[target];
            p[target] = tmp;
        }
    }

    static double perlin_interp(vec3 c[2][2][2], double u, double v, double w) {
        // Interpolate the perlin noise
        auto uu = u*u*(3-2*u);
        // Compute the Hermite cubic interpolation for u
        auto vv = v*v*(3-2*v);
        // Compute the Hermite cubic interpolation for v
        auto ww = w*w*(3-2*w);
        // Compute the Hermite cubic interpolation for w
        auto accum = 0.0;
        // Initialize the accumulator

        for (int i=0; i < 2; i++)
            for (int j=0; j < 2; j++)
                for (int k=0; k < 2; k++) {
                    vec3 weight_v(u-i, v-j, w-k);
                    // Compute the weight vector
                    accum += (i*uu + (1-i)*(1-uu))
                           * (j*vv + (1-j)*(1-vv))
                           * (k*ww + (1-k)*(1-ww))
                           * dot(c[i][j][k], weight_v);
                    // Compute the dot product and accumulate
                }

        return accum;
    }
};

#endif