import numpy as np

def fade(t):
    return t * t * t * (t * (t * 6 - 15) + 10)

def lerp(t, a, b):
    return a + t * (b - a)

def grad(hash, x, y):
    h = hash & 15
    u = x if h < 8 else y
    v = y if h < 4 else (x if h == 12 or h == 14 else 0)
    return (u if (h & 1) == 0 else -u) + (v if (h & 2) == 0 else -v)

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

noise = Noise()

def generate_terrain(size, scale=50.0, octaves=6, persistence=0.5, lacunarity=2.0):
    heightmap = np.zeros((size, size), dtype='f4')

    for z in range(size):
        for x in range(size):
            nx = x / size - 0.5
            nz = z / size - 0.5
            height = 0
            frequency = 1
            amplitude = 1
            for _ in range(octaves):
                height += noise.snoise2(nx * scale * frequency, nz * scale * frequency) * amplitude
                frequency *= lacunarity
                amplitude *= persistence
            heightmap[z, x] = height * 0.5 + 0.5

    return heightmap
perm = np.arange(256, dtype=int)
np.random.shuffle(perm)
perm = np.concatenate([perm, perm])

