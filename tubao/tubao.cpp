#include "tubao.h"
#include"ConvexHull.h"


QString tips = QStringLiteral("使用说明:\r输入点数后点击Start，可自行选择输入坐标（点击Next输入下一个坐标），点击Auto自动生成坐标点，点击Finiish自动生成凸包");

tubao::tubao(QWidget* parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);

	PNumL = new QLabel(this);
	PNumL->setObjectName("PnumL");
	PNumL->setGeometry(QRect(1100, 100, 50, 30));
	PNumL->setText("Num:");
	Pnum = new QLineEdit(this);
	Pnum->setObjectName("Pnum");
	Pnum->setGeometry(QRect(1160, 100, 100, 30));

	NumXL = new QLabel(this);
	NumXL->setObjectName("NumXL");
	NumXL->setGeometry(QRect(1120, 140, 50, 30));
	NumXL->setText("X:");
	NumX = new QLineEdit(this);
	NumX->setObjectName("NumX");
	NumX->setGeometry(QRect(1160, 140, 100, 30));
	NumX->setReadOnly(true);

	NumYL = new QLabel(this);
	NumYL->setObjectName("NumYL");
	NumYL->setGeometry(QRect(1120, 180, 50, 30));
	NumYL->setText("Y:");
	NumY = new QLineEdit(this);
	NumY->setObjectName("NumY");
	NumY->setGeometry(QRect(1160, 180, 100, 30));
	NumY->setReadOnly(true);

	InputNum = new QPushButton(this);
	InputNum->setText("Start");
	InputNum->setObjectName(QStringLiteral("InputNum"));
	InputNum->setGeometry(QRect(1120, 230, 150, 30));

	Next = new QPushButton(this);
	Next->setText("Next");
	Next->setObjectName(QStringLiteral("Next"));
	Next->setGeometry(QRect(1120, 280, 150, 30));
	Next->setEnabled(false);

	machine = new QPushButton(this);
	machine->setText("auto");
	machine->setObjectName(QStringLiteral("machine"));
	machine->setGeometry(QRect(1120, 330, 150, 30));
	machine->setEnabled(false);

	drawtubao = new QPushButton(this);
	drawtubao->setText("Finish");
	drawtubao->setObjectName(QStringLiteral("drawtubao"));
	drawtubao->setGeometry(QRect(1120, 380, 150, 30));
	drawtubao->setEnabled(false);

	view = new QTextEdit(this);
	view->setReadOnly(true);
	view->setObjectName("view");
	view->setGeometry(QRect(1110, 440, 170, 600));
	view->setText(tips);

	ui.menuBar->addAction("ReStart", this, SLOT(restart()));
	connect(InputNum, SIGNAL(clicked()), this, SLOT(startexe()));
	connect(Next, SIGNAL(clicked()), this, SLOT(nextpoint()));
	connect(machine, SIGNAL(clicked()), this, SLOT(automachine()));
	connect(drawtubao, SIGNAL(clicked()), this, SLOT(drawresult()));
}

int i = 0;//记录输入点数
QString str;
QString resultnum;
bool isfinish = false;

void tubao::paintEvent(QPaintEvent* e)
{
	//绘制坐标系
	QPainter painter(this);
	QBrush brush;
	QPen pen;
	painter.translate(515, 545);
	painter.drawLine(-515, 0, 515, 0);
	painter.drawLine(0, -515, 0, 515);
	painter.drawLine(515, 515, 515, -515);

	for (int i = -500; i <= 500; )
	{
		QString a = QString::number(i / 50);
		painter.drawLine(i, 0, i, -3);
		painter.drawText(QPoint(i - 10, -5), a);
		painter.drawLine(0, i, 3, i);
		if (i != 0)
			painter.drawText(QPoint(10, i + 10), a);
		i = i + 50;
	}

	//输入后显示点
	for (int j = 0; j < i; j++)
	{
		painter.setBrush(QBrush(Qt::black));
		painter.drawEllipse(rawpoint[j].x * 50 - 5, 50 * rawpoint[j].y - 5, 10, 10);
		painter.setPen(QPen(Qt::blue));
		QString a = QString::number(j + 1);
		painter.drawText(QPoint(rawpoint[j].x * 50 + 5, 50 * rawpoint[j].y + 10), a);
	}

	//完成后画出凸包
	if (isfinish)
	{
		painter.setPen(QPen(Qt::red, 5));
		ConPnt fristpnt = resultpoint.top();
		int size = resultpoint.size();
		for (int i = 0; i < size - 1; i++)
		{
			ConPnt point1 = resultpoint.top();
			for (int j = 0; j < Ptotal; j++)
			{
				if (point1.x == rawpoint[j].x && point1.y == rawpoint[j].y)
				{
					QString status = QString("%1").arg(j + 1);
					resultnum = resultnum + status + "\r";
				}
			}
			resultpoint.pop();
			ConPnt  point2 = resultpoint.top();
			painter.drawLine(point1.x * 50, point1.y * 50, point2.x * 50, point2.y * 50);
		}
		ConPnt final = resultpoint.top();
		painter.drawLine(final.x * 50, final.y * 50, fristpnt.x * 50, fristpnt.y * 50);
		isfinish = false;
	}
}

//输入点数
void tubao::startexe() {

	QString Text = Pnum->text();
	int size = Text.toInt();
	if (size < 3)
	{
		QMessageBox message(QMessageBox::Warning, "Warning", "Please enter an integer greater than 2", QMessageBox::Yes, NULL);
		if (message.exec() == QMessageBox::Yes)
		{
			return;
		}
	}
	Ptotal = size;
	finalpoints = new ConPnt[Ptotal];
	rawpoint = new ConPnt[Ptotal];
	Pnum->setReadOnly(true);
	NumX->setReadOnly(false);
	NumY->setReadOnly(false);
	InputNum->setEnabled(false);
	Next->setEnabled(true);
	machine->setEnabled(true);
}

//输入坐标并显示
void tubao::nextpoint() {
	machine->setEnabled(false);
	QString Text1 = NumX->text();
	QString Text2 = NumY->text();
	int X = Text1.toInt();
	int Y = Text2.toInt();
	ConPnt now(X, Y);
	if (abs(X) > 10 || abs(Y) > 10)
	{
		QMessageBox message(QMessageBox::Warning, "Warning", "Please enter an integer with absolute value less than 10", QMessageBox::Yes, NULL);
		if (message.exec() == QMessageBox::Yes)
		{
			return;
		}
	}
	if (twice(now, rawpoint, i))
	{
		QMessageBox message(QMessageBox::Warning, "Warning", "The point already exists", QMessageBox::Yes, NULL);
		if (message.exec() == QMessageBox::Yes)
		{
			return;
		}
	}
	rawpoint[i] = now;
	i++;
	QString status = QString("%1:(%2,%3)")
		.arg(i).arg(X).arg(Y);
	str = str + status + "\r";
	view->setText(str);
	NumX->clear();
	NumY->clear();
	if (i == Ptotal)
	{
		NumX->setReadOnly(true);
		NumY->setReadOnly(true);
		drawtubao->setEnabled(true);
		Next->setEnabled(false);
	}
	repaint();
}

//随机生成点
void tubao::automachine()
{
	qsrand(QTime(0, 0, 0).secsTo(QTime::currentTime()));//当前时间为种子
	for (int m = 0; m < Ptotal; m++) {
		int xzf = qrand() % 2;//x值正负
		int yzf = qrand() % 2;//y值正负
		int X = qrand() % 11;
		int Y = qrand() % 11;
		if (xzf == 1)
		{
			X = -X;
		};
		if (yzf == 1) {
			Y = -Y;
		}
		ConPnt now(X, Y);
		if (twice(now, rawpoint, i))
		{
			m--;
			continue;
		}
		rawpoint[i] = now;
		i++;
		QString status = QString("%1:(%2,%3)")
			.arg(i).arg(X).arg(Y);
		str = str + status + "\r";
	}
	repaint();
	view->setText(str);
	InputNum->setEnabled(false);
	Next->setEnabled(false);
	machine->setEnabled(false);
	drawtubao->setEnabled(true);
}

//画图
void tubao::drawresult() {
	copyConPnt();
	int YM = SortY(finalpoints);//按Y排列
	Calculate(finalpoints, YM);//计算cos
	sortpoint(finalpoints);//按cos排列
	Graham(finalpoints);//扫描
	isfinish = true;
	repaint();
	//在绘图函数中，其余点位置均已输入resultnum，但起点（栈底）未弹出，需额外添加
	ConPnt last = resultpoint.top();
	for (int j = 0; j < Ptotal; j++)
	{
		if (last.x == rawpoint[j].x && last.y == rawpoint[j].y)
		{
			QString status = QString("%1").arg(j + 1);
			resultnum = resultnum + status + "\r";
		}
	}
	view->setText(resultnum);
	drawtubao->setEnabled(false);
}

//重新开始
void tubao::restart() {
	str = "";
	resultnum = "";
	i = 0;
	clearall();
	InputNum->setEnabled(true);
	Next->setEnabled(false);
	machine->setEnabled(false);
	drawtubao->setEnabled(false);
	view->setText(tips);
	Pnum->clear();
	Pnum->setReadOnly(false);
	NumX->setReadOnly(true);
	NumY->setReadOnly(true);
	repaint();
}