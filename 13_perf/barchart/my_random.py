
'''
  CTR_DRBG (NOPR)          :       1150 KiB/s,        153 cycles/byte
  CTR_DRBG (PR)            :        757 KiB/s,        232 cycles/byte
  HMAC_DRBG SHA-1 (NOPR)   :        232 KiB/s,        761 cycles/byte
  HMAC_DRBG SHA-1 (PR)     :        214 KiB/s,        827 cycles/byte
  HMAC_DRBG SHA-256 (NOPR) :        220 KiB/s,        803 cycles/byte
  HMAC_DRBG SHA-256 (PR)   :        192 KiB/s,        924 cycles/byte

'''
import numpy as np
import matplotlib.pyplot as plt


random = (1150,  232,  220)

fig, x = plt.subplots()

index = np.arange(3)
# 宽度
bar_width = 0.3

# 透明度
opacity = 1
# 颜色说明
# https://matplotlib.org/gallery/color/named_colors.html#sphx-glr-gallery-color-named-colors-py
# 推荐选择 darkgray gray dimgray

plt.bar(index, random, bar_width, alpha=opacity, color='darkgray')


plt.ylabel('KB/s')

plt.xticks(index, ('CTR_DRBG ', 'HMAC_DRBG SHA-1', 'HMAC_DRBG SHA-256'), rotation='0')
# plt.legend()

# 增加Y轴范围保证 SHA512位置
plt.ylim(0, 1300)


for a, b in zip(index, random):
    plt.text(a, b + 0.1, '%.0f' % b, ha='center', va='bottom', fontsize='12')


plt.tight_layout()
plt.show()
