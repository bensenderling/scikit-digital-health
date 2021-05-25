"""
Utility Functions (:mod:`skdh.utility`)
================================

.. currentmodule:: skdh.utility

Misc. Math Functions
--------------------

.. autosummary::
    :toctree: generated/

    math.moving_mean
    math.moving_sd
    math.moving_skewness
    math.moving_kurtosis
    math.moving_median

Orientation Functions
---------------------

.. autosummary::
    :toctree: generated/

    orientation.correct_accelerometer_orientation


Windowing Functions
-------------------

.. autosummary::
    :toctree: generated/

    windowing.compute_window_samples
    windowing.get_windowed_view
"""

from skdh.utility.math import *
from skdh.utility import math
from skdh.utility.orientation import correct_accelerometer_orientation
from skdh.utility import orientation
from skdh.utility.windowing import compute_window_samples, get_windowed_view
from skdh.utility import windowing


__all__ = (
    ["math", "windowing", "orientation"]
    + math.__all__
    + windowing.__all__
    + orientation.__all__
)