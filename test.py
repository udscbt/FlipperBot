#!/usr/bin/env python
from flipperbot.board import *
g = game.Game(totems.totems.values(), debug=False, logging=False, displayex=False)
s = server.Server(g, dbgFlag=True)
#s.address = '192.168.1.1'
s.address = '0.0.0.0'

g.start()
s.start()
