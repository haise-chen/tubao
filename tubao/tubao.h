#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_tubao.h"
#include"qlineedit.h"
#include"qpainter.h"
#include"qlabel.h"
#include"qtextedit.h"
#include"qmenu.h"
#include"qmenubar.h"
#include"qpushbutton.h"
#include"QTime"
#include"qmessagebox.h"

class tubao : public QMainWindow
{
    Q_OBJECT

public:
    tubao(QWidget* parent = Q_NULLPTR);
    void paintEvent(QPaintEvent* e);

private slots:
    void startexe();
    void nextpoint();
    void drawresult();
    void restart();
    void automachine();

private:
    Ui::tubaoClass ui;
    QLineEdit* Pnum;
    QLineEdit* NumX;
    QLineEdit* NumY;
    QLabel* PNumL;
    QLabel* NumXL;
    QLabel* NumYL;
    QPushButton* InputNum;
    QPushButton* Next;
    QPushButton* drawtubao;
    QPushButton* machine;
    QTextEdit* view;
};
