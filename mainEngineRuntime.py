
import sys
import os
import time
from multiprocessing import Process

import OpenGL
OpenGL.ERROR_CHECKING = True
OpenGL.ERROR_LOGGING = True
OpenGL.FULL_LOGGING = False
OpenGL.ALLOW_NUMPY_SCALARS = True

from CHIFEngineMain.Common import CustomQueue, CustomPipe
from CHIFEngineMain.src import CoreManager
from CHIFEngineMain.Utilities import AutoEnum, Config


class GUIEditor(AutoEnum):
    CLIENT_MODE = ()
    QT = ()
    TKINTER = ()


def run(editor=GUIEditor.QT, project_filename=""):
    appCmdQueue = None
    uiCmdQueue = None
    pipe1, pipe2 = None, None
    editor_process = None

    config = Config("config.ini")
    if "" == project_filename and config.hasValue('Project', 'recent'):
        last_project = config.getValue('Project', 'recent')
        if os.path.exists(last_project):
            project_filename = last_project
    if editor != GUIEditor.CLIENT_MODE:
        appCmdQueue = CustomQueue()
        uiCmdQueue = CustomQueue()
        pipe1, pipe2 = CustomPipe()

        if editor == GUIEditor.QT:
            from CHIFEngineMain.UI.QT.MainWindow import run_editor
        elif editor == GUIEditor.TKINTER:
            from CHIFEngineMain.UI.TKInter.MainWindow import run_editor
        editor_process = Process(target=run_editor, args=(project_filename, uiCmdQueue, appCmdQueue, pipe2))
        editor_process.start()

    coreManager = CoreManager.instance()
    result = coreManager.initialize(appCmdQueue, uiCmdQueue, pipe1, project_filename)
    if result:
        coreManager.run()
        next_next_open_project_filename = coreManager.get_next_open_project_filename()
    else:
        next_next_open_project_filename = ""


    if editor_process:
        editor_process.join()

    return next_next_open_project_filename 


if __name__ == "__main__":
    editor = GUIEditor.TKINTER

    project_filename = sys.argv[1] if len(sys.argv) > 1 else ""
    next_open_project_filename = run(editor, project_filename)
    if os.path.exists(next_open_project_filename):
        executable = sys.executable
        args = sys.argv[:]
        if len(args) > 1:
            args[1] = next_open_project_filename
        else:
            args.append(next_open_project_filename)
        args.insert(0, sys.executable)
        time.sleep(1)
        os.execvp(executable, args)
