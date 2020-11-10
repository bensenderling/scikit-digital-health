"""
Base classes, functions, etc for the skimu library

Lukas Adamowicz
Pfizer DMTI 2020
"""
from datetime import date as dt_date
import logging

from pandas import DataFrame


class _BaseProcess:
    # names of the variables that are passed to predict
    # CHANGE IF predict/_predict call changes!
    _file = 'file'
    _time = 'time'
    _acc = 'accel'
    _gyro = 'gyro'
    _mag = 'magnet'
    _temp = 'temperature'
    _days = 'day_ends'

    def __str__(self):
        return self._name

    def __repr__(self):
        ret = f"{self._name}("
        for k in self._kw:
            ret += f"{k}={self._kw[k]!r}, "
        ret = ret[:-2] + ")"

        return ret

    def __init__(self, **kwargs):
        """
        Intended to be subclassed

        Parameters
        ----------
        return_result : bool, optional
            Return the result part of predict, or the input/output dictionary
        kwargs
            Key-word arguments which are passed to the sub-class
        """
        self._name = self.__class__.__name__
        self._in_pipeline = False  # initialize to false.  Will be set by the pipeline

        self._kw = kwargs

        self.logger = logging.getLogger(__name__)
        self.logger.setLevel('INFO')  # set the level for the logger
        self.logger.addHandler(logging.StreamHandler())
        self.logger.disabled = True

    def predict(self, *args, **kwargs):
        """
        Intended to be overwritten in the subclass. Should still be called with super though
        """
        if self.logger is not None:
            self.logger.info(f"Entering {self._name} processing with call {self!r}")

    def disable_logging(self):
        """
        Disable all logging for the process.
        """
        self.logger.disabled = True

    def enable_logging(self):
        """
        Enable all logging for the process.
        """
        self.logger.disabled = False

    def add_logging_file_handler(self, filename=None):
        """
        Add a file handler for logging, which will output the logs to the specified file.

        Parameters
        ----------
        filename : {None, str}, optional
            Filename for the logger. Default is None, which will use "{name}_logs.log"
        """
        if filename is None:
            filename = f"{self._name}_logs.log"
        self.logger.addHandler(logging.FileHandler(filename=filename))

    def save_results(self, results, file_name):
        """
        Save the results of the processing pipeline to a csv file

        Parameters
        ----------
        results : dict
            Dictionary of results from the output of predict
        file_name : str
            File name. Can be optionally formatted (see Notes)

        Notes
        -----
        Available format variables available:

        - date: todays date expressed in yyyymmdd format
        - name: process name
        """
        date = dt_date.today().strftime('%Y%m%d')
        name = self._name

        file_name = file_name.format(date=date, name=name)

        DataFrame(results).to_csv(file_name, index=False)
