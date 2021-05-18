from tempfile import TemporaryDirectory
from pathlib import Path

from numpy import array, allclose

from skimu.base import _BaseProcess


class Test_BaseProcess:
    def test_str_repr(self):
        bp = _BaseProcess(kw1=1, kw2="2")

        assert str(bp) == "_BaseProcess"
        assert repr(bp) == "_BaseProcess(kw1=1, kw2='2')"

    def test_eq(self, testprocess, testprocess2):
        tp1_a = testprocess(kw1=1)
        tp1_b = testprocess(kw1=2)
        tp2_a = testprocess2(kwa=1)
        tp2_b = testprocess2(kwa=2)

        assert tp1_a == tp1_a
        assert all([tp1_a != i for i in [tp1_b, tp2_a, tp2_b]])

        assert tp1_b == tp1_b
        assert all([tp1_b != i for i in [tp1_a, tp2_a, tp2_b]])

        assert tp2_a == tp2_a
        assert all([tp2_a != i for i in [tp1_a, tp1_b, tp2_b]])

        assert tp2_b == tp2_b
        assert all([tp2_b != i for i in [tp1_a, tp1_b, tp2_a]])

    @staticmethod
    def setup_lgr():
        class Lgr:
            msgs = []

            def info(self, msg):
                self.msgs.append(msg)

        return Lgr()

    def test__check_if_idx_none(self):
        bp = _BaseProcess()
        bp.logger = self.setup_lgr()  # overwrite the logger

        x = array([[0, 10], [15, 20]])

        s, e = bp._check_if_idx_none(x, "none msg", None, None)

        assert allclose(s, [0, 15])
        assert allclose(e, [10, 20])

        s, e = bp._check_if_idx_none(None, "none msg", 0, 10)
        sn, en = bp._check_if_idx_none(None, "none msg", None, 10)

        assert "none msg" in bp.logger.msgs
        assert s == 0
        assert e == 10

        assert sn is None
        assert en is None

    def test_predict(self):
        bp = _BaseProcess()
        bp.logger = self.setup_lgr()

        bp.predict(
            expect_days=True,
            expect_wear=True,
            accel=array([[1, 2, 3], [4, 5, 6]])
        )

        assert bp._file_name == ""
        assert "Entering _BaseProcess processing with call _BaseProcess()" in bp.logger.msgs
        assert "[_BaseProcess] Day indices [(-1, -1)] not found. No day split used." in bp.logger.msgs

    def test_save_results(self):
        bp = _BaseProcess()

        bp.predict(expect_wear=False, expect_days=False, file="test_file.infile")

        with TemporaryDirectory() as tdir:
            tdir = Path(tdir)

            fname = tdir / "{file}__{name}.out"

            bp.save_results({"a": [1, 2, 3]}, str(fname))

            files = [i.name for i in tdir.glob("*")]

        assert "test_file___BaseProcess.out" in files
