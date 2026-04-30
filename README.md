# 🔬 Pipe Phase Change LBM – 管道相变流动的格子 Boltzmann 模拟

[![License](https://img.shields.io/badge/License-MIT-blue.svg)](LICENSE)
[![C++](https://img.shields.io/badge/C++-17-blue.svg)](https://isocpp.org/)
[![LBM](https://img.shields.io/badge/LBM-BGK-orange.svg)]()

二维管道内气液相变（蒸发）的格子 Boltzmann 方法（LBM）模拟代码。采用 BGK 碰撞模型，耦合流体流动、温度场和相场，模拟液体受热蒸发为气体的过程。

## 📖 目录

- [物理模型](#-物理模型)
- [数值方法](#-数值方法)
- [边界条件](#-边界条件)
- [代码结构](#-代码结构)
- [参数说明](#-参数说明)
- [运行与输出](#-运行与输出)
- [可视化示例](#-可视化示例)
- [全局质量演化](#-全局质量演化)
- [参考文献](#-参考文献)

## 🧠 物理模型

### 1. 平衡分布函数

使用 D2Q9 离散速度模型，三个场的平衡分布函数形式相同（仅变量不同）：

**密度场**
$$
f_i^{eq} = \omega_i \rho \left(1 + 3\,\mathbf{e}_i\cdot\mathbf{u} + \frac{9}{2}(\mathbf{e}_i\cdot\mathbf{u})^2 - \frac{3}{2}u^2\right)
$$

**温度场**
$$
g_i^{eq} = \omega_i T \left(1 + 3\,\mathbf{e}_i\cdot\mathbf{u} + \frac{9}{2}(\mathbf{e}_i\cdot\mathbf{u})^2 - \frac{3}{2}u^2\right)
$$

**相场**
$$
h_i^{eq} = \omega_i \phi \left(1 + 3\,\mathbf{e}_i\cdot\mathbf{u} + \frac{9}{2}(\mathbf{e}_i\cdot\mathbf{u})^2 - \frac{3}{2}u^2\right)
$$

- $\omega_i$：D2Q9 权系数  
- $\mathbf{e}_i$：离散速度向量  
- $\rho$：混合密度，由相场线性插值  
  $$
  \rho = \phi\,\rho_l + (1-\phi)\,\rho_g
  $$
- $\phi$：相场，$\phi=1$ 纯液体，$\phi=0$ 纯气体  
- $T$：温度

### 2. 相变模型（蒸发）

蒸发速率（局部质量源项）：
$$
\dot{m} = K_{phase} \cdot \max(T - T_{sat},\,0) \cdot \phi
$$

- $K_{phase}$：相变系数  
- $T_{sat}$：饱和温度  
- 仅当 $T > T_{sat}$ 且存在液体（$\phi>0$）时发生蒸发

能量方程源项（吸热）：
$$
S_T = -\frac{L}{c_p}\,\dot{m}
$$

相场方程源项（液体减少）：
$$
S_\phi = -\dot{m}
$$

## 🧮 数值方法

### 碰撞步

$$
f_i^{new} = f_i - \frac{\Delta t}{\tau_f}(f_i - f_i^{eq})
$$
$$
g_i^{new} = g_i - \frac{\Delta t}{\tau_t}(g_i - g_i^{eq}) + \Delta t\,\omega_i S_T
$$
$$
h_i^{new} = h_i - \frac{\Delta t}{\tau_\phi}(h_i - h_i^{eq}) + \Delta t\,\omega_i S_\phi
$$

### 迁移步

$$
f_i(\mathbf{x}+\mathbf{e}_i\Delta t,\,t+\Delta t) = f_i^{new}(\mathbf{x},t)
$$
（对 $g_i, h_i$ 同理）

### 宏观量恢复

$$
\rho = \sum_i f_i,\quad \mathbf{j} = \sum_i f_i\mathbf{e}_i,\quad \mathbf{u} = \mathbf{j}/\rho
$$
$$
T = \sum_i g_i,\quad \phi = \sum_i h_i
$$
$$
\rho = \phi \rho_l + (1-\phi)\rho_g
$$

## 🧱 边界条件

| 边界 | 条件 |
|------|------|
| **入口 (x=0)** | Dirichlet：固定速度 \(u_{in}\)，温度 \(T_{in}\)，相场 \(\phi=1\)（纯液体） |
| **出口 (x=nx-1)** | 零梯度外推（速度、温度、相场、密度） – 非平衡外推分布函数 |
| **上下壁面 (y=0, y=ny-1)** | 无滑移（速度=0），固定壁温 \(T_{wall}\)，相场零梯度（绝热无质量通量） |

## 📂 代码结构

### 类 `PipePhaseChangeLBM`

| 成员 / 方法 | 说明 |
|------------|------|
| `PipePhaseChangeLBM()` | 构造函数，分配内存，设置初始参数 |
| `Initialize()` | 初始场：静止、均匀温度、全液相 |
| `ComputeMdot()` | 计算局部蒸发率 \(\dot{m}\) |
| `CollisionF/T/Phi()` | 碰撞步，更新分布函数 |
| `Streaming()` | 迁移步 |
| `Macroscopic()` | 计算宏观量（\(\rho,\mathbf{u},T,\phi\)） |
| `ApplyBoundary()` | 施加边界条件 |
| `export_results()` | 输出 CSV 数据（当前输出 φ 和 mdot） |
| `Run()` | 主循环，每 10 步输出一次结果 |

> 如需输出更多场（如速度、温度），可修改 `export_results()` 中写入的列。

## ⚙️ 参数说明（构造函数中可调）

| 参数 | 默认值 | 物理意义 |
|------|--------|----------|
| `nx, ny` | 用户定义 | 网格数 |
| `nstep` | 用户定义 | 总时间步数 |
| `dt` | 用户定义 | 时间步长 |
| `tau_f` | 1.0 | 流动松弛时间 |
| `tau_t` | 1.2 | 温度松弛时间 |
| `tau_phi` | 1.2 | 相场松弛时间 |
| `u_in` | 0.01 | 入口速度 |
| `T_in` | 1.7 | 入口温度 |
| `Twall` | 2.2 | 壁面温度 |
| `rho_l` | 5.0 | 液体密度 |
| `rho_g` | 0.4 | 气体密度 |
| `Tsat` | 0.5 | 饱和温度 |
| `Kphase` | 0.1 | 相变系数 |
| `latent` | 2.0 | 汽化潜热 |
| `cp` | 1.0 | 比热容 |

> **提示**：增大 `Kphase` 可加快蒸发速率；降低 `Tsat` 或提高 `Twall` 可增强过热度。

## 🚀 运行与输出

### 编译运行示例

```cpp
#include "LBM.hpp"

int main() {
    int nx = 100, ny = 30;
    int nstep = 50000;
    double dt = 1.0;

    PipePhaseChangeLBM sim(nx, ny, nstep, dt);
    sim.Run();

    return 0;
}
```
###  输出文件
程序每 10 个时间步在 data/ 文件夹生成 CSV 文件，如 results_0.csv, results_10.csv, …
当前输出列：x, y, rho, phi, mdot（可修改 export_results 添加更多场）。

##  📊 可视化示例（Python）


使用提供的 mdot.py 或手动绘图，例如生成 T 云图：
```python
#include "LBM.hpp"

import pandas as pd
import matplotlib.pyplot as plt
import matplotlib.tri as tri

df = pd.read_csv("data/results_1000.csv")
triang = tri.Triangulation(df['x'], df['y'])
plt.tripcolor(triang, df['T'], shading='gouraud', cmap='hot')
plt.colorbar(label='Evaporation rate (T)')
plt.xlabel('x')
plt.ylabel('y')
plt.title('T field')
plt.show()
```
你还可以绘制密度场、相场 φ、速度场的演化动画（参考仓库中的 Rho_graph.py, P_graph.py, V_graph.py）。

## 📈 全局质量演化

<div align="center">

![液相与气相质量演化曲线](data/mass_evolution.png)
![出口蒸汽质量流量曲线](data/outlet_vapor_flow.png)

*上：液相、气相及总质量随迭代步数的演化；下：出口蒸汽质量流量随迭代步数的变化*

</div>

## 🎬 物理场动画

<div align="center">

| 温度场 | 密度场 |
|:---:|:---:|
| ![温度场演化](data/temperature_evolution.gif) | ![密度场演化](data/density_evolution.gif) |

| 速度场（流线） | 相场（φ） |
|:---:|:---:|
| ![流场演化](data/velocity_streamlines.gif) | ![相场演化](data/phase_field_evolution.gif) |

</div>




## 📚 参考文献
Guo, Z., & Shu, C. (2013). Lattice Boltzmann Method and Its Applications in Engineering.

Sukop, M. C., & Thorne, D. T. (2006). Lattice Boltzmann Modeling.

相变模型参考焓法 LBM 相关文献。
## 📜 许可证
本项目采用 MIT 许可证。   
  
作者：alexmercer37
日期：2025-04-30
项目地址：https://github.com/alexmercer37/LBM-PipePhaseChange



