import os

from CHIFEngineMain.Utilities import Logger

log_level = Logger.INFO  # Logger.DEBUG, Logger.MINOR_INFO, Logger.INFO, Logger.WARNING, Logger.ERROR
logger = Logger.getLogger(level=log_level)

from .Command import COMMAND, get_command_name, CustomPipe, CustomQueue
from .Constants import *
