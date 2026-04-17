#ifndef _D2Q5_H

#include "main.hpp"

class D2Q5
{
private:
    int Lx, Ly, nstep, m, n, mid;

    double dx, dt, cc, cs2, alpha, omega, twall;

    vector<double> w = {2.0 / 6.0, 1.0 / 6.0, 1.0 / 6.0, 1.0 / 6.0, 1.0 / 6.0};
    vector<double> x, Tm;

    vector<vector<int>> e = {{0, 0}, {1, 0}, {0, 1}, {-1, 0}, {0, -1}};
    vector<vector<double>> T;
    vector<vector<vector<double>>> f, f_new;

    void reset()
    {

        x.assign(m, 0.0);
        Tm.assign(m, 0.0);

        T.assign(m, vector<double>(n, 0.0));
        f.assign(5, vector<vector<double>>(m, vector<double>(n, 0.0)));

        f_new = f;

        for (int i = 0; i < m - 1; ++i)
        {
            x[i + 1] = x[i] + dx;
        }

        for (int i = 0; i < m; ++i)
        {
            for (int j = 0; j < n; ++j)
            {
                T[i][j] = twall * (1.0 - (double)j / (n - 1));
            }
        }

        for (int i = 0; i < 5; ++i)
        {

            for (int j = 0; j < m; ++j)
            {

                for (int k = 0; k < n; ++k)
                {
                    f[i][j][k] = w[i] * T[j][k];
                    f_new[i][j][k] = f[i][j][k];
                }
            }
        }
    }

public:
    D2Q5(int Lx_, int Ly_, double dx_, double dt_, int nstep_, double cs2_, double twall_, double alpha_) : Lx(Lx_), Ly(Ly_), dx(dx_), dt(dt_), nstep(nstep_), cs2(cs2_), twall(twall_), alpha(alpha_)
    {

        cc = dx / dt;
        m = Lx / dx + 1;
        n = Ly / dx + 1;

        omega = 1 / (0.5 + 3 * alpha * dt / (dx * dx));
        mid = (n - 1) / 2;

        reset();
    }

    void D2Q5_F();
    void export_results(const string &filename);

    ~D2Q5();
};

#endif