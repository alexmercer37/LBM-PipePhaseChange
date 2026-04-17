import pandas as pd
import numpy as np
import matplotlib.pyplot as plt
from scipy.interpolate import griddata

# 1. 加载数据
filename = '../LBM_data/D2Q9.csv'
df = pd.read_csv(filename)


# 2. 提取维度信息
m_max = df['x_idx'].max() + 1
n_max = df['y_idx'].max() + 1

# 3. 数据重构 (Pivot)
# 将长格式数据转换为二维矩阵
T_matrix = df.pivot(index='x_idx', columns='y_idx', values='T').values
qx_matrix = df.pivot(index='x_idx', columns='y_idx', values='qx').values
qy_matrix = df.pivot(index='x_idx', columns='y_idx', values='qy').values

# 4. 创建坐标网格
X, Y = np.meshgrid(np.arange(n_max), np.arange(m_max))

# 5. 绘图
plt.figure(figsize=(12, 7), dpi=100)

# --- A. 绘制温度云图 ---
# cmap='RdYlBu_r' 红色代表热，蓝色代表冷
contour = plt.contourf(X, Y, T_matrix, levels=50, cmap='RdYlBu_r')
cbar = plt.colorbar(contour)
cbar.set_label('Temperature ($T$)', rotation=270, labelpad=15)

# --- B. 叠加等温线 ---
#lines = plt.contour(X, Y, T_matrix, levels=10, colors='black', linewidths=0.5, alpha=0.5)
#plt.clabel(lines, inline=True, fontsize=8)

# --- C. 叠加热流矢量图 (Quiver) ---
# 为了防止箭头太密，进行采样（每隔 skip 个点画一个箭头）
#skip = max(1, m_max // 15) 
#plt.quiver(X[::skip, ::skip], Y[::skip, ::skip], 
#           qy_matrix[::skip, ::skip], -qx_matrix[::skip, ::skip], 
#           color='white', alpha=0.7, pivot='mid', scale=None)

# 6. 图表修饰
plt.title(f'LBM D2Q9 Heat Conduction Simulation\nSteady State (Grid: {int(m_max)}x{int(n_max)})', fontsize=14)
plt.xlabel('Grid Column index ($j$)', fontsize=12)
plt.ylabel('Grid Row index ($i$)', fontsize=12)

# 反转 Y 轴，使 i=0 位于顶部，符合矩阵索引习惯
plt.gca().invert_yaxis()

plt.tight_layout()
plt.show()