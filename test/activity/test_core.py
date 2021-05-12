import datetime as dt

from numpy import array, allclose

from skimu.activity.core import (
    _check_if_none,
    _update_date_results,
    ActivityLevelClassification,
    get_activity_bouts,
    get_intensity_gradient,
)


class Test_check_if_none:
    @staticmethod
    def setup_lgr():
        class Lgr:
            msgs = []

            def info(self, msg):
                self.msgs.append(msg)

        return Lgr()

    def test(self):
        lgr = self.setup_lgr()

        x = array([[0, 10], [15, 20]])

        s, e = _check_if_none(x, lgr, "none msg", None, None)

        assert allclose(s, [0, 15])
        assert allclose(e, [10, 20])

    def test_none(self):
        lgr = self.setup_lgr()

        s, e = _check_if_none(None, lgr, "none msg", 0, 10)
        sn, en = _check_if_none(None, lgr, "none msg", None, 10)

        assert "none msg" in lgr.msgs
        assert s == 0
        assert e == 10

        assert sn is None
        assert en is None


class Test_update_date_results:
    @staticmethod
    def setup_results():
        k = ["Date", "Weekday", "Day N", "N hours"]
        r = {i: [0] for i in k}

        return r

    def test(self):
        res = self.setup_results()

        date = dt.datetime(
            year=2021,
            month=5,
            day=10,
            hour=23,
            minute=59,
            second=57,
            tzinfo=dt.timezone.utc
        )
        epoch_ts = date.timestamp()

        time = [epoch_ts + i * 3600 for i in range(10)]

        _update_date_results(res, time, 0, 0, 5, 0)

        assert res["Date"][0] == "2021-05-11"  # next day is when it starts
        assert res["Weekday"][0] == "Tuesday"
        assert res["Day N"][0] == 1
        assert res["N hours"][0] == 4.0

    def test_late_day_start(self):
        res = self.setup_results()

        date = dt.datetime(
            year=2021,
            month=5,
            day=11,
            hour=10,
            minute=32,
            second=13,
            tzinfo=dt.timezone.utc
        )
        epoch_ts = date.timestamp()

        time = [epoch_ts + i * 3600 for i in range(10)]

        _update_date_results(res, time, 0, 0, 5, 12)

        # previous day is the actual window start
        assert res["Date"][0] == "2021-05-10"
        assert res["Weekday"][0] == "Monday"
        assert res["Day N"][0] == 1
        assert res["N hours"][0] == 4.0
