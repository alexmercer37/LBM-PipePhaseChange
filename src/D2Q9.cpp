#include "../inc/D2Q9.hpp"

void D2Q9::export_results(const string &filename)
{
    ofstream outFile(filename);
    outFile << "x_idx,y_idx,T,qx,qy\n";

    for (int i = 0; i < m; ++i)
    {

        for (int j = 0; j < n; ++j)
        {
            double sum_ex = 0.0;
            double sum_ey = 0.0;

            for (int k = 0; k < 9; ++k)
            {
                sum_ex += (double)e[k][0] * f[k][i][j];
                sum_ey += (double)e[k][1] * f[k][i][j];
            }

            double qx = sum_ex * (omega - 0.5) / omega;
            double qy = sum_ey * (omega - 0.5) / omega;

            outFile << i << "," << j << "," << T[i][j] << "," << qx << "," << qy << "\n";
        }
    }

    outFile.close();
    cout << "Successfully saved to: " << filename << std::endl;
}

void D2Q9::D2Q9_F()
{
    reset();

    for (int k1 = 0; k1 < nstep; ++k1)
    {
        for (int i = 0; i < 9; ++i)
        {
            for (int j = 0; j < m; ++j)
            {

                for (int k = 0; k < n; ++k)
                {
                    f[i][j][k] = (1.0 - omega) * f[i][j][k] + omega * w[i] * T[j][k];
                }
            }
        }

        for (int i = 0; i < 9; ++i)
        {
            int di = e[i][0];
            int dj = e[i][1];

            for (int j = 0; j < m; ++j)
            {
                for (int k = 0; k < n; ++k)
                {
                    int ni = (j + di + m) % m;
                    int nj = (k + dj + n) % n;

                    f_new[i][ni][nj] = f[i][j][k];
                }
            }
        }

        swap(f, f_new);

        for (int i = 0; i < m; ++i)
        {
            for (int j = 0; j < n; ++j)
            {
                T[i][j] = 0.0;

                for (int k = 0; k < 9; ++k)
                {
                    T[i][j] += f[k][i][j];
                }
            }
        }

        for (int i = 0; i < m; ++i)
        {
            T[i][0] = twall;

            for (int k = 0; k < 9; ++k)
            {
                f[k][i][0] = w[k] * T[i][0] + f[k][i][1] - w[k] * T[i][1];
            }
        }

        for (int i = 0; i < m; ++i)
        {
            T[i][n - 1] = 0.0;

            for (int k = 0; k < 9; ++k)
            {
                f[k][i][n - 1] = w[k] * T[i][n - 1] + (f[k][i][n - 2] - w[k] * T[i][n - 2]);
            }
        }

        for (int j = 0; j < n; ++j)
        {
            T[0][j] = 0.0;

            for (int k = 0; k < 9; ++k)
            {
                f[k][0][j] = w[k] * T[0][j] + (f[k][1][j] - w[k] * T[1][j]);
            }
        }

        if (k1 % 1000 == 0)
        {
            string filename = "data/frame_" + to_string(k1) + ".csv";

            export_results(filename);
            cout << "Saved frame at step: " << k1 << endl;
        }
    }

    for (int i = 0; i < m; ++i)
    {
        Tm[i] = T[i][mid];
    }

    D2Q9::export_results("D2Q9.csv");
}

D2Q9::~D2Q9()
{

    std::cout << "Object destroyed: D1Q3 simulation resources have been released." << std::endl;
}