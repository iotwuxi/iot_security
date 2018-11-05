
import numpy as np
import matplotlib.pyplot as plt

gcm = (363, 381, 399)
ccm = (298, 333, 369)

fig, x = plt.subplots()

index = np.arange(3)
# 宽度
bar_width = 0.3

# 透明度
opacity = 1
# 颜色说明
# https://matplotlib.org/gallery/color/named_colors.html#sphx-glr-gallery-color-named-colors-py
# 推荐选择 darkgray gray dimgray

plt.bar(index, gcm, bar_width, alpha=opacity, color='dimgray', label='AES_GCM')
plt.bar(index + bar_width, ccm, bar_width, alpha=opacity, color='darkgray', label='AES_CCM')

plt.ylabel('cycles/bytes')

plt.xticks(index + bar_width/2, ('AES-GCM/CCM-128', 'AES-GCM/CCM-192', 'AES-GCM/CCM-256'))

plt.legend()

# 增加Y轴范围保证 SHA512位置
plt.ylim(0, 520)


for a, b in zip(index, gcm):
    plt.text(a, b + 0.1, '%.0f' % b, ha='center', va='bottom', fontsize='12')

for a, b in zip(index, ccm):
    plt.text(a + bar_width, b + 0.1, '%.0f' % b, ha='center', va='bottom', fontsize='12')


plt.tight_layout()
plt.show()
