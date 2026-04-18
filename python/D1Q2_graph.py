import pandas as pd
import matplotlib.pyplot as plt

# 1. 加载数据
filename = 'S_D1Q3.csv' # 确保文件名正确
try:
    df = pd.read_csv(filename)
except FileNotFoundError:
    print("未找到数据文件，请检查路径。")
    exit()

# 2. 创建画布
fig, ax1 = plt.subplots(figsize=(10, 6), dpi=100)

# --- 绘制温度分布 (左侧 Y 轴) ---
color_t = 'tab:red'
ax1.set_xlabel('Position ($x$)', fontsize=12)
ax1.set_ylabel('Temperature ($T$)', color=color_t, fontsize=12)
line1 = ax1.plot(df['x'], df['T'], color=color_t, linewidth=2, marker='o', 
                 markersize=4, label='Temperature')
ax1.tick_params(axis='y', labelcolor=color_t)
ax1.grid(True, linestyle='--', alpha=0.5)

# --- 绘制热通量分布 (右侧 Y 轴) ---
# 创建共享 x 轴的第二个 y 轴
ax2 = ax1.twinx() 
color_q = 'tab:blue'
ax2.set_ylabel('Heat Flux ($q$)', color=color_q, fontsize=12)
line2 = ax2.plot(df['x'], df['flux'], color=color_q, linewidth=1.5, 
                 linestyle='--', marker='s', markersize=4, label='Heat Flux')
ax2.tick_params(axis='y', labelcolor=color_q)

# 3. 设置范围 (防止 Flux 的微小波动被无限放大)
# 稳态下 Flux 应为常数，这里让 Y 轴范围覆盖平均值的上下 10%
flux_mean = df['flux'].mean()
if abs(flux_mean) > 1e-10:
    ax2.set_ylim(flux_mean * 0.5, flux_mean * 1.5)
else:
    ax2.set_ylim(-0.1, 0.1)

# 4. 合并图例
lines = line1 + line2
labels = [l.get_label() for l in lines]
ax1.legend(lines, labels, loc='upper center', frameon=True)

# 5. 标题与修饰
plt.title('D1Q3 LBM Heat Conduction Results', fontsize=14, pad=20)
fig.tight_layout()

# 6. 保存与显示
# plt.savefig('D1Q2_Plot.png', dpi=300)
plt.show()
