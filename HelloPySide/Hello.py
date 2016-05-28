# -*- coding: utf-8 -*-

import sys

from PySide.QtGui import *
from PySide.QtCore import *
from helloui import Ui_Form



class Hello(QWidget, Ui_Form):
    def __init__(self, parent=None):
        super(Hello, self).__init__(parent)
        self.setupUi(self)

if __name__=='__main__':
    Program = QApplication(sys.argv)
    Window = Hello()
    Window.show()
    Program.exec_()
