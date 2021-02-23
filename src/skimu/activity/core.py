"""
Activity level classification based on accelerometer data

Lukas Adamowicz
Pfizer DMTI 2021
"""
from skimu.base import _BaseProcess

from skimu.activity.metrics import *

# ==========================================================
# Activity cutpoints
cutpoints = {}

cutpoints["esliger_lwrist_adult"] = {
    "metric": metric_enmo,
    "kwargs": {"take_abs": True},
    "sedentary": 217 / 80 / 60,  # paper at 80hz, summed for each minute long window
    "light": 644 / 80 / 60,
    "moderate": 1810 / 80 / 60
}

cutpoints["esliger_rwirst_adult"] = {
    "metric": metric_enmo,
    "kwargs": {"take_abs": True},
    "sedentary": 386 / 80 / 60,  # paper at 80hz, summed for each 1min window
    "light": 439 / 80 / 60,
    "moderate": 2098 / 80 / 60
}

cutpoints["esliger_lumbar_adult"] = {
    "metric": metric_enmo,
    "kwargs": {"take_abs": True},
    "sedentary": 77 / 80 / 60,  # paper at 80hz, summed for each 1min window
    "light": 219 / 80 / 60,
    "moderate": 2056 / 80 / 60
}

cutpoints["schaefer_ndomwrist_child6-11"] = {
    "metric": metric_bfen,
    "kwargs": {"low_cutoff": 0.2, "high_cutoff": 15, "trim_zero": False},
    "sedentary": 0.190,
    "light": 0.314,
    "moderate": 0.998
}

cutpoints["phillips_rwrist_child8-14"] = {
    "metric": metric_enmo,
    "kwargs": {"take_abs": True},
    "sedentary": 6 / 80,  # paper at 80hz, summed for each 1s window
    "light": 21 / 80,
    "moderate": 56 / 80
}

cutpoints["phillips_lwrist_child8-14"] = {
    "metric": metric_enmo,
    "kwargs": {"take_abs": True},
    "sedentary": 7 / 80,
    "light": 19 / 80,
    "moderate": 60 / 80
}

cutpoints["phillips_hip_child8-14"] = {
    "metric": metric_enmo,
    "kwargs": {"take_abs": True},
    "sedentary": 3 / 80,
    "light": 16 / 80,
    "moderate": 51 / 80
}

cutpoints["vaha-ypya_hip_adult"] = {
    "metric": metric_mad,
    "kwargs": {},
    "light": 0.091,  # originally presented in mg
    "moderate": 0.414
}

cutpoints["hildebrand_hip_adult_actigraph"] = {
    "metric": metric_enmo,
    "kwargs": {"take_abs": False, "trim_zero": True},
    "sedentary": 0.0474,
    "light": 0.0691,
    "moderate": 0.2587
}

cutpoints["hildebrand_hip_adult_geneactv"] = {
    "metric": metric_enmo,
    "kwargs": {"take_abs": False, "trim_zero": True},
    "sedentary": 0.0469,
    "light": 0.0687,
    "moderate": 0.2668
}

cutpoints["hildebrand_wrist_adult_actigraph"] = {
    "metric": metric_enmo,
    "kwargs": {"take_abs": False, "trim_zero": True},
    "sedentary": 0.0448,
    "light": 0.1006,
    "moderate": 0.4288
}

cutpoints["hildebrand_wrist_adult_geneactiv"] = {
    "metric": metric_enmo,
    "kwargs": {"take_abs": False, "trim_zero": True},
    "sedentary": 0.0458,
    "light": 0.0932,
    "moderate": 0.4183
}

cutpoints["hildebrand_hip_child7-11_actigraph"] = {
    "metric": metric_enmo,
    "kwargs": {"take_abs": False, "trim_zero": True},
    "sedentary": 0.0633,
    "light": 0.1426,
    "moderate": 0.4646
}

cutpoints["hildebrand_hip_child7-11_geneactiv"] = {
    "metric": metric_enmo,
    "kwargs": {"take_abs": False, "trim_zero": True},
    "sedentary": 0.0641,
    "light": 0.1528,
    "moderate": 0.5143
}

cutpoints["hildebrand_wrist_child7-11_actigraph"] = {
    "metric": metric_enmo,
    "kwargs": {"take_abs": False, "trim_zero": True},
    "sedentary": 0.0356,
    "light": 0.2014,
    "moderate": 0.707
}

cutpoints["hildebrand_wrist_child7-11_geneactiv"] = {
    "metric": metric_enmo,
    "kwargs": {"take_abs": False, "trim_zero": True},
    "sedentary": 0.0563,
    "light": 0.1916,
    "moderate": 0.6958
}


def get_available_cutpoints():
    """
    Print the available cutpoints for activity level segmentation.
    """
    print(cutpoints.keys())


class MVPActivityClassification(_BaseProcess):
    """
    Classify accelerometer data into different activity levels as a proxy for assessing physical
    activity energy expenditure (PAEE). Levels are sedentary, light, moderate, and vigorous.

    Parameters
    ----------

    """