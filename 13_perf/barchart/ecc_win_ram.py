
import numpy as np
import matplotlib.pyplot as plt

handshake = (1.37, 1.57, 1.60, 1.60, 1.60)
ram_consume = (1376, 1500, 2328, 2368, 2360)

fig, x = plt.subplots()
index = np.arange(5)
# 宽度
bar_width = 0.35

# 透明度
opacity = 1
# 颜色说明
# https://matplotlib.org/gallery/color/named_colors.html#sphx-glr-gallery-color-named-colors-py
# 推荐选择 darkgray gray dimgray

plt.bar(index, ram_consume, bar_width, alpha=opacity, color='darkgray')

plt.ylabel('bytes')

plt.xticks(index, ('win=2', 'win=3', 'win=4', 'win=5', 'win=6'))

# plt.legend(loc='upper left')
# plt.legend()

# 增加Y轴范围保证
plt.ylim(0, 2600)

for a, b in zip(index, ram_consume):
    plt.text(a, b + 0.1, '%.0f' % b, ha='center', va='bottom', fontsize='12')

plt.tight_layout()
plt.show()