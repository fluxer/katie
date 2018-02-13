#!/usr/bin/python2

import sys, KtCore, KtGui

a = KtGui.QApplication(0, None)
w = KtGui.QMainWindow()
w.setWindowTitle(KtCore.QString('Test'))
w.show()
sys.exit(a._exec())