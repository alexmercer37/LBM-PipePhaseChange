#ifndef _D1Q3_H

#include "main.hpp"

#define D1Q3_F
#define D1Q3_T
#define D1Q3_Alpha
#define D1Q3_S
#define D1Q3_V

class D1Q3

{
private:
    int m, nstep;

    double L, dx, dt, alpha, omega, Tw_right, Tw_left, S;

    vector<double> w = {4.0 / 6.0, 1.0 / 6.0, 1.0 / 6.0};
    vector<double> e = {1, -1, 0};
    vector<double> T;
    vector<vector<double>> f;
    vector<double> x;

    void reset()
    {

        T.assign(m, 0.5);
        x.assign(m, 0.0);

        for (int i = 0; i < m; ++i)
        {
            x[i] = i * dx;
        }

        f.assign(3, vector<double>(m, 0.0));

        for (int q = 0; q < 3; ++q)
        {
            for (int i = 0; i < m; ++i)
                f[q][i] = w[q] * T[i];
        }
    }

public:
    D1Q3(double L_, int m_, double S_, int nstep_, double dt_, double alpha_, double tl, double tr) : L(L_), m(m_), S(S_), nstep(nstep_), dt(dt_), alpha(alpha_), Tw_left(tl), Tw_right(tr), f(3, vector<double>(m_, 0.0))
    {

        dx = L / (m - 1);
        omega = 1.0 / (3.0 * alpha * dt / ((L / (m - 1)) * (L / (m - 1))) + 0.5);

        x.resize(m);
        T.resize(m);

        reset();
    }

    void D1Q3_f();
    void D1Q3_t();
    void D1Q3_alpha();
    void D1Q3_s();
    void D1Q3_v();
    void export_results(const std::string &filename);
    void export_results(const string &filename, const std::vector<double> &omega);

    ~D1Q3();
};
#endif