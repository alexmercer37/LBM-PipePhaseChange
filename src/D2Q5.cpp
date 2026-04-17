#include "../inc/D2Q5.hpp"

void D2Q5::export_results(const string &filename)
{
    ofstream outFile(filename);
    outFile << "x_idx,y_idx,T,qx,qy\n";

    for (int i = 0; i < m; ++i)
    {

        for (int j = 0; j < n; ++j)
        {
            double sum_ex = 0.0;
            double sum_ey = 0.0;

            for (int k = 0; k < 5; ++k)
            {
                sum_ex = f[1][i][j] - f[3][i][j];
                sum_ey = f[2][i][j] - f[4][i][j];
            }

            double qx = sum_ex * (omega - 0.5) / omega;
            double qy = sum_ey * (omega - 0.5) / omega;

            outFile << i << "," << j << "," << T[i][j] << "," << qx << "," << qy << "\n";
        }
    }

    outFile.close();
    cout << "Successfully saved to: " << filename << std::endl;
}

void D2Q5::D2Q5_F()
{

    reset();

    for (int k1 = 0; k1 < nstep; ++k1)
    {

        for (int i = 0; i < 5; ++i)
        {

            for (int j = 0; j < m; ++j)
            {

                for (int k = 0; k < n; ++k)
                {
                    f[i][j][k] = (1.0 - omega) * f[i][j][k] + omega * w[i] * T[j][k];
                }
            }
        }

        for (int i = 0; i < 5; ++i)
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
            f[1][i][0] = -f[3][i][0] + 2.0 * w[1] * twall;
        }

        for (int i = 0; i < m; ++i)
        {
            f[3][i][n - 1] = -f[1][i][n - 1] + 2.0 * w[3] * 0.0;
        }

        for (int j = 0; j < n; ++j)
        {
            f[4][0][j] = f[2][0][j];
        } // 3. 上边界 (i=0): T = 0, 绝热

        for (int j = 0; j < n; ++j)
        {
            f[2][m - 1][j] = f[4][m - 1][j];
        } // 4. 下边界 (i=m-1): T = 0, 绝热

        for (int i = 0; i < m; ++i)
        {

            for (int j = 0; j < n; ++j)
            {

                T[i][j] = 0.0;

                for (int k = 0; k < 5; ++k)
                {
                    T[i][j] += f[k][i][j];
                }
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

    D2Q5::export_results("D2Q5.csv");
}

D2Q5::~D2Q5()
{

    std::cout << "Object destroyed: D1Q3 simulation resources have been released." << std::endl;
}
