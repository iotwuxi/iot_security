
import numpy as np
import matplotlib.pyplot as plt

n_groups = 2

no_crt = (98, 15)
crt = (261, 48)

fig, priv_x = plt.subplots(figsize=(5, 5))

index = np.arange(n_groups)
# 宽度
bar_width = 0.35

# 透明度
opacity = 0.5

plt.bar(index, no_crt, bar_width,
        alpha=opacity, color='b', label='RSA_NO_CRT')

plt.bar(index + bar_width, crt, bar_width,
        alpha=opacity, color='r', label='RSA_WITH_CRT')

# plt.xlabel('RSA')
plt.ylabel('private/s')
# plt.title('mbedtls RSA With CRT')

plt.xticks(index + bar_width / 2, ('RSA-2048', 'RSA-4096'))
plt.legend()

for a, b in zip(index, no_crt):
    plt.text(a, b + 0.1, '%.0f' % b, ha='center', va='bottom', fontsize='12')

for a, b in zip(index, crt):
    plt.text(a + bar_width, b + 0.1, '%.0f' % b, ha='center', va='bottom', fontsize='12')

plt.tight_layout()
plt.show()
