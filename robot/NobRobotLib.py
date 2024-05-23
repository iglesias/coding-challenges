import nobuild.nob as nob
import os
import time

class NobRobotLib:
    def __init__(self) -> None:
        pass

    def build_executable(self):
        nob.make_and_run_cmd(["g++", "template.cc"])

    def get_executable_time(self):
        return os.path.getmtime('a.out')

    def get_current_time(self):
        return time.time()
