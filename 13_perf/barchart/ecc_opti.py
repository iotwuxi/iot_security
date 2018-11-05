
import numpy as np
import matplotlib.pyplot as plt

max = (4448, 5512, 4908)
recommend = (1412, 1856, 1416)

fig, x = plt.subplots()

index = np.arange(3)
# 宽度
bar_width = 0.35

# 透明度
opacity = 1
# 颜色说明
# https://matplotlib.org/gallery/color/named_colors.html#sphx-glr-gallery-color-named-colors-py
# 推荐选择 darkgray gray dimgray

plt.bar(index, max, bar_width, alpha=opacity, color='dimgray', label=u"最优性能")
plt.bar(index + bar_width, recommend, bar_width, alpha=opacity, color='darkgray', label=u"推荐配置")

plt.ylabel('bytes')

plt.xticks(index + bar_width/2, ('ECDSA Sign', 'ECDSA Verify', 'ECDHE'))

# plt.legend(loc='upper left')
# plt.legend()

# 增加Y轴范围保证
plt.ylim()


for a, b in zip(index, max):
    plt.text(a, b + 0.1, '%.0f' % b, ha='center', va='bottom', fontsize='12')

for a, b in zip(index, recommend):
    plt.text(a + bar_width, b + 0.1, '%.0f' % b, ha='center', va='bottom', fontsize='12')


plt.tight_layout()
plt.show()
