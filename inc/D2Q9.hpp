#ifndef _D2Q9_H

#include "main.hpp"

class D2Q9
{
private:
    int Lx, Ly, m, n, nstep, mid;

    double dx, dt, cc, cs2, twall, omega, alpha;

    vector<double> x, Tm;
    vector<double> w = {4.0 / 9.0, 1.0 / 9.0, 1.0 / 9.0, 1.0 / 9.0, 1.0 / 9.0, 1.0 / 36.0, 1.0 / 36.0, 1.0 / 36.0, 1.0 / 36.0};

    vector<vector<int>> e = {{0, 0}, {1, 0}, {0, 1}, {-1, 0}, {0, -1}, {1, 1}, {-1, 1}, {-1, -1}, {1, -1}};
    vector<vector<double>> T;
    vector<vector<vector<double>>> f, f_new;

    void reset()
    {

        x.assign(m, 0.0);
        Tm.assign(m, 0.0);

        T.assign(m, vector<double>(n, 0.0));
        f.assign(9, vector<vector<double>>(m, vector<double>(n, 0.0)));

        f_new = f;

        for (int i = 0; i < m - 1; ++i)
        {
            x[i + 1] = x[i] + dx;
        }

        for (int j = 0; j < m; ++j)
        {

            for (int k = 0; k < n; ++k)
            {
                T[j][k] = twall * (1.0 - (double)k / (n - 1));
            }
        }

        for (int i = 0; i < 9; ++i)
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
    D2Q9(int Lx_, int Ly_, double dx_, double dt_, int nstep_, double cs2_, double twall_, double alpha_) : Lx(Lx_), Ly(Ly_), dx(dx_), dt(dt_), nstep(nstep_), cs2(cs2_), twall(twall_), alpha(alpha_)
    {

        cc = dx / dt;
        m = Lx / dx + 1;
        n = Ly / dx + 1;

        omega = 1 / (0.5 + 3 * alpha * dt / (dx * dx));
        mid = (n - 1) / 2;

        reset();
    }

    void D2Q9_F();
    void export_results(const string &filename);

    ~D2Q9();
};

#endif