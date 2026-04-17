import pandas as pd
import numpy as np
import matplotlib.pyplot as plt
import matplotlib.animation as animation
import glob
import re

# 1. 配置路径与文件搜索
data_path = '../data/'  # 你的数据存放文件夹
# 获取所有 frame_ 开头的文件，并按数字编号排序
files = sorted(glob.glob(f'{data_path}/frame_*.csv'), 
               key=lambda x: int(re.findall(r'\d+', x)[0]))

if not files:
    print("错误：未在指定目录找到 frame_*.csv 文件！")
    exit()

# 2. 初始化画布
fig, ax = plt.subplots(figsize=(10, 6), dpi=100)
# 固定 Colorbar 的范围 (0到1)，防止动画闪烁
norm = plt.Normalize(0, 1.0) 
cmap = 'RdYlBu_r'

# 3. 动画核心函数
def update(frame_idx):
    ax.clear()
    file = files[frame_idx]
    df = pd.read_csv(file)
    
    # 提取维度与矩阵
    m_max = df['x_idx'].max() + 1
    n_max = df['y_idx'].max() + 1
    T_matrix = df.pivot(index='x_idx', columns='y_idx', values='T').values
    
    # 创建网格
    X, Y = np.meshgrid(np.arange(n_max), np.arange(m_max))
    
    # A. 绘制云图
    contour = ax.contourf(X, Y, T_matrix, levels=50, cmap=cmap, norm=norm)
    
    # B. 叠加等温线 (可选)
    lines = ax.contour(X, Y, T_matrix, levels=10, colors='black', linewidths=0.5, alpha=0.3)
    
    # C. 修饰
    step_num = re.findall(r'\d+', file)[0]
    ax.set_title(f'LBM D2Q5 Thermal Evolution\nStep: {step_num}', fontsize=14)
    ax.set_xlabel('Column index (j)')
    ax.set_ylabel('Row index (i)')
    ax.invert_yaxis() # 保持索引习惯
    
    # 第一次运行添加 colorbar
    if not hasattr(update, "cbar"):
        update.cbar = fig.colorbar(plt.cm.ScalarMappable(norm=norm, cmap=cmap), ax=ax)
        update.cbar.set_label('Temperature ($T$)')

# 4. 创建并保存动画
print(f"检测到 {len(files)} 帧数据，正在合成动画...")

# interval: 帧间隔(毫秒)，数字越小速度越快
ani = animation.FuncAnimation(fig, update, frames=len(files), interval=100)

# 保存为 GIF (需要安装 pillow 库: pip install pillow)
output_name = 'LBM_Evolution.gif'
ani.save(output_name, writer='pillow', fps=10)

print(f"成功！动画已保存为: {output_name}")
plt.show()
