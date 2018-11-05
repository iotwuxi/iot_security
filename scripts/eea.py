import sys

n = int(sys.argv[1])
p = int(sys.argv[2])

def extended_euclid_algorithm(n, p):
    s, old_s = 0, 1
    t, old_t = 1, 0
    r, old_r = p, n

    while r != 0:
        quotient = old_r // r
        old_r, r = r, old_r - quotient * r
        old_s, s = s, old_s - quotient * s
        old_t, t = t, old_t - quotient * t

    return old_r, old_s, old_t

def inverse_of(n, p):
    gcd, x, y = extended_euclid_algorithm(n, p)
    assert (n * x + p * y) % p == gcd

    if gcd != 1:
        raise ValueError(
            '{} has no multiplicative inverse '
            'modulo {}'.format(n, p))
    else:
        return x % p

print(n, "^ -1 mod", p, "=", inverse_of(n, p))
