<<<<<<< HEAD
import numpy as np

from freq import fft


x = np.linspace(0, 4 * np.pi, 256)
y = np.sin(2 * np.pi * x) + 0.5 * np.cos(2 * np.pi * x/4) + 0.75 * np.random.rand(256)
nfft = 2 ** int(np.log2(x.size))

fF = fft(y, nfft)
=======
from scipy.misc import electrocardiogram

x1d = electrocardiogram()
x3d = x1d.reshape((1, -1, 1))


from sigent import fsignalentropy

for i in range(3):
    res = fsignalentropy(x3d)
    print(res)
>>>>>>> c58b4fb23624c66aa098d7bd6127a9e5dd012ffe
