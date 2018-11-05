'''
对比CRT对RSA的影响
'''

import numpy as np
import matplotlib.pyplot as plt

n_groups = 2
# 宽度
bar_width = 0.35
# 透明度
opacity = 0.4

# 公钥操作速度结果
public_no_crt = (12415, 12189)
public_crt = (3622, 3595)

# 私钥操作速度结果
private_no_crt = (98, 15)
private_crt = (261, 48)

plt.subplot(1, 2, 1)
index = np.arange(n_groups)

plt.bar(index, public_no_crt, bar_width,
        alpha=0.5, color='black', label='RSA_NO_CRT')

plt.bar(index + bar_width, public_crt, bar_width,
        alpha=0.8, color='black', label='RSA_WITH_CRT')

# plt.xlabel('RSA')
plt.ylabel('public/s')
# plt.title('mbedtls RSA With CRT')
# 增加Y轴范围保证 legend位置
plt.ylim(0, 15000)
plt.xticks(index + bar_width / 2, ('RSA-2048', 'RSA-4096'))
plt.legend()

for a, b in zip(index, public_no_crt):
    plt.text(a, b + 0.1, '%.0f' % b, ha='center', va='bottom')

for a, b in zip(index, public_crt):
    plt.text(a + bar_width, b + 0.1, '%.0f' % b, ha='center', va='bottom')

plt.subplot(1, 2, 2)
index = np.arange(n_groups)

plt.bar(index, private_no_crt, bar_width,
        alpha=0.5, color='black', label='RSA_NO_CRT')

plt.bar(index + bar_width, private_crt, bar_width,
        alpha=0.8, color='black', label='RSA_WITH_CRT')

# plt.xlabel('RSA')
plt.ylabel('private/s')
# plt.title('mbedtls RSA With CRT')

plt.xticks(index + bar_width / 2, ('RSA-2048', 'RSA-4096'))
for a, b in zip(index, private_no_crt):
    plt.text(a, b + 0.1, '%.0f' % b, ha='center', va='bottom')

for a, b in zip(index, private_crt):
    plt.text(a + bar_width, b + 0.1, '%.0f' % b, ha='center', va='bottom')

plt.legend()

plt.tight_layout()
plt.show()
