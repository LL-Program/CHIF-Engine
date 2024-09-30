import numpy as np
class Noise:
    def __init__(self):
        self.p = np.arange(256, dtype=int)
        np.random.shuffle(self.p)
        self.p = np.concatenate([self.p, self.p])

    def snoise2(self, x, y):
        X = int(x) & 255
        Y = int(y) & 255
        x -= int(x)
        y -= int(y)
        u = fade(x)
        v = fade(y)
        A = self.p[X] + Y
        B = self.p[X + 1] + Y
        return lerp(v, lerp(u, grad(self.p[A], x, y),
                               grad(self.p[B], x - 1, y)),
                       lerp(u, grad(self.p[A + 1], x, y - 1),
                               grad(self.p[B + 1], x - 1, y - 1)))

def fade(t):
    return t * t * t * (t * (t * 6 - 15) + 10)

def lerp(t, a, b):
    return a + t * (b - a)

def grad(hash, x, y):
    h = hash & 15
    u = x if h < 8 else y
    v = y if h < 4 else (x if h == 12 or h == 14 else 0)
    return (u if (h & 1) == 0 else -u) + (v if (h & 2) == 0 else -v)


def normal_noise(noise, x, y):
    return 0.5 + 0.5 * noise.snoise2(x, y)

def cursive(noise, x, y, steps=13, sigma=0.7):
    gamma = (1.0 / sigma) ** steps
    displace_x, displace_y = 0.0, 0.0
    for _ in range(steps):
        noise_x = normal_noise(noise, x * gamma + displace_x, y * gamma + displace_y)
        noise_y = normal_noise(noise, y * gamma + displace_x, x * gamma + displace_y)
        displace_x += 1.5 * noise_x
        displace_y += 1.5 * noise_y
        gamma *= sigma
    return normal_noise(noise, x * gamma + displace_x, y * gamma + displace_y)

def smooth_heightmap(heightmap, kernel_size=3):
    from scipy.ndimage import gaussian_filter
    return gaussian_filter(heightmap, sigma=kernel_size)

def generate_terrain_with_cursive(size, noise, scale=50.0, steps=13, sigma=0.7, smooth=True):
    heightmap = np.zeros((size, size), dtype='f4')
    for z in range(size):
        for x in range(size):
            nx = x / size - 0.5
            nz = z / size - 0.5
            height = cursive(noise, nx * scale, nz * scale, steps, sigma)
            heightmap[z, x] = height
    if smooth:
        heightmap = smooth_heightmap(heightmap)
    
    return heightmap

size = 100
noise = Noise()
terrain = generate_terrain_with_cursive(size, noise, scale=50.0, steps=13, sigma=0.7, smooth=True)