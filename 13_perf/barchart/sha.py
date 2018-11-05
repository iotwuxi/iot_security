
import numpy as np
import matplotlib.pyplot as plt

sha = (46, 89, 249)

fig, x = plt.subplots()

index = np.arange(3)
# 宽度
bar_width = 0.35

# 透明度
opacity = 1
# 颜色说明
# https://matplotlib.org/gallery/color/named_colors.html#sphx-glr-gallery-color-named-colors-py
# 推荐选择 darkgray gray dimgray

plt.bar(index, sha, bar_width, alpha=opacity, color='darkgray')
# plt.bar(index + bar_width, sha, bar_width, alpha=opacity, color='dimgray')

plt.ylabel('cycles/bytes')

plt.xticks(index, ('SHA-1', 'SHA-256', 'SHA-512'))
# plt.legend()

# 增加Y轴范围保证 SHA512位置
plt.ylim(0, 280)


for a, b in zip(index, sha):
    plt.text(a, b + 0.1, '%.0f' % b, ha='center', va='bottom', fontsize='12')


plt.tight_layout()
plt.show()
