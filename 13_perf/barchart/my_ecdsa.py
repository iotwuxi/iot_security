
import numpy as np
import matplotlib.pyplot as plt

sign = (2.92, 4.73, 7.53, 11.17)
verify = (0.82, 1.32, 2.20, 3.23)

fig, x = plt.subplots()

index = np.arange(4)
# 宽度
bar_width = 0.35

# 透明度
opacity = 1
# 颜色说明
# https://matplotlib.org/gallery/color/named_colors.html#sphx-glr-gallery-color-named-colors-py
# 推荐选择 darkgray gray dimgray

plt.bar(index, sign, bar_width, alpha=opacity, color='dimgray', label='ECSDA Sign')
plt.bar(index + bar_width, verify, bar_width, alpha=opacity, color='darkgray', label='ECDSA Verify')

plt.ylabel('times/s')

plt.xticks(index + bar_width/2, ('secp521r1', 'secp384r1', 'secp256r1', 'secp224r1'))

plt.legend(loc='upper left')

# 增加Y轴范围保证
plt.ylim(0, 12)


for a, b in zip(index, sign):
    plt.text(a, b + 0.1, '%.2f' % b, ha='center', va='bottom', fontsize='12')

for a, b in zip(index, verify):
    plt.text(a + bar_width, b + 0.1, '%.2f' % b, ha='center', va='bottom', fontsize='12')


plt.tight_layout()
plt.show()
