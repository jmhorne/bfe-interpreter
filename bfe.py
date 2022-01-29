#!/usr/bin/env python3
import sys
sys.path.insert(0, './src')

from Interpreter import Interpreter
from Memory import Memory

i = Interpreter(Memory())

program = "program2"

i.execute(Memory("programs/" + program + ".bfe"))