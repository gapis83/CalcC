#include "mainwindow.h"

#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow) {
  ui->setupUi(this);
  this->setFixedSize(1170, 610);

  connect(ui->pushButton_0, SIGNAL(clicked()), this, SLOT(input_line()));
  connect(ui->pushButton_1, SIGNAL(clicked()), this, SLOT(input_line()));
  connect(ui->pushButton_2, SIGNAL(clicked()), this, SLOT(input_line()));
  connect(ui->pushButton_3, SIGNAL(clicked()), this, SLOT(input_line()));
  connect(ui->pushButton_4, SIGNAL(clicked()), this, SLOT(input_line()));
  connect(ui->pushButton_5, SIGNAL(clicked()), this, SLOT(input_line()));
  connect(ui->pushButton_6, SIGNAL(clicked()), this, SLOT(input_line()));
  connect(ui->pushButton_7, SIGNAL(clicked()), this, SLOT(input_line()));
  connect(ui->pushButton_8, SIGNAL(clicked()), this, SLOT(input_line()));
  connect(ui->pushButton_9, SIGNAL(clicked()), this, SLOT(input_line()));
  connect(ui->pushButton_X, SIGNAL(clicked()), this, SLOT(input_line()));
  connect(ui->pushButton_plus, SIGNAL(clicked()), this, SLOT(input_line()));
  connect(ui->pushButton_minus, SIGNAL(clicked()), this, SLOT(input_line()));
  connect(ui->pushButton_divide, SIGNAL(clicked()), this, SLOT(input_line()));
  connect(ui->pushButton_multiplication, SIGNAL(clicked()), this,
          SLOT(input_line()));
  connect(ui->pushButton_bracket_1, SIGNAL(clicked()), this,
          SLOT(input_line()));
  connect(ui->pushButton_bracket_2, SIGNAL(clicked()), this,
          SLOT(input_line()));
  connect(ui->pushButton_pow, SIGNAL(clicked()), this, SLOT(input_line()));
  connect(ui->pushButton_dot, SIGNAL(clicked()), this, SLOT(input_line()));
  connect(ui->pushButton_mod, SIGNAL(clicked()), this, SLOT(input_line()));
  connect(ui->pushButton_sin, SIGNAL(clicked()), this, SLOT(operations()));
  connect(ui->pushButton_cos, SIGNAL(clicked()), this, SLOT(operations()));
  connect(ui->pushButton_tan, SIGNAL(clicked()), this, SLOT(operations()));
  connect(ui->pushButton_Asin, SIGNAL(clicked()), this, SLOT(operations()));
  connect(ui->pushButton_Acos, SIGNAL(clicked()), this, SLOT(operations()));
  connect(ui->pushButton_Atan, SIGNAL(clicked()), this, SLOT(operations()));
  connect(ui->pushButton_ln, SIGNAL(clicked()), this, SLOT(operations()));
  connect(ui->pushButton_log, SIGNAL(clicked()), this, SLOT(operations()));
  connect(ui->pushButton_sqrt, SIGNAL(clicked()), this, SLOT(operations()));
}

MainWindow::~MainWindow() { delete ui; }

void MainWindow::input_line() {
  QPushButton *button = (QPushButton *)sender();
  QString result_show = NULL;

  ui->result_show->setText(ui->result_show->text() + button->text());
}

void MainWindow::operations() {
  QPushButton *button = (QPushButton *)sender();
  ui->result_show->setText(ui->result_show->text() + button->text() + "(");
}

void MainWindow::on_pushButton_AC_clicked() {
  ui->result_show->setText("please enter data");
  ui->result_show->setText(NULL);
  ui->lineEdit_x->setText("0");
  ui->l_l->setText(NULL);
}

void MainWindow::on_pushButton_equals_clicked() {
  ui->result_show->setText(ui->result_show->text() + "=");
  QByteArray line = NULL;
  QString new_label = NULL;
  QString x_line = NULL;
  double x;

  new_label = ui->result_show->text();
  ui->result_show->setText(new_label);
  x = ui->lineEdit_x->text().toDouble();

  if (new_label.indexOf('X') < 0) {
    line = new_label.toLocal8Bit();
  } else {
    x_line = new_label.replace("X", "(" + (QString::number(x)) + ")");
    line = x_line.toLocal8Bit();
  }

  ui->l_l->setText(line);
  const char *str = line.data();
  char byt[255] = "";
  strncpy(byt, str, 255);

  if (*str) {
    int num_err = s21_correct_num_act(byt);
    int verify_err = s21_verifying(byt);
    if (num_err == 0 && verify_err == 0) {
      double res = 0.0;
      res = smart_calc(byt);
      QString new_label;
      new_label = QString::number(res, 'g', 14);
      ui->result_show->setText(new_label);
    } else {
      ui->result_show->setText("ERROR. Incorrect input");
    }
  }
}

void MainWindow::on_pushButton_graph_clicked() {
  ui->widget->clearGraphs();
  ui->l_l->setText(NULL);
  QString new_label = NULL;
  QString check_label = NULL;
  QString x_line = NULL;
  QByteArray line = NULL;
  double x_max, y_max, x_min, y_min;

  xBegin = ui->lineEdit_x_min->text().toDouble();
  xEnd = ui->lineEdit_x_max->text().toDouble();
  x_min = xBegin;
  x_max = xEnd;
  y_min = ui->lineEdit_y_min->text().toDouble();
  y_max = ui->lineEdit_y_max->text().toDouble();
  if (x_min < x_max && y_min < y_max) {
    double h = (abs(x_max) + abs(x_min)) / 1000;
    int N = (xEnd - xBegin) / h + 2;
    QVector<double> x(N), y(N);
    ui->widget->xAxis->setRange(x_min, x_max);
    ui->widget->yAxis->setRange(y_min, y_max);
    int i = 0;
    for (X = xBegin; X <= xEnd; X += h) {
      if (X < 0.001 && X > -0.001) X = 0;
      x[i] = (X);
      new_label = ui->result_show->text() + "=";
      if (new_label.indexOf('X') < 0) {
        line = new_label.toLocal8Bit();
      } else {
        x_line = new_label.replace("X", "(" + (QString::number(X)) + ")");
        line = x_line.toLocal8Bit();
      }
      const char *str = line.data();

      char byt[255] = "";
      strncpy(byt, str, 255);
      if (*str) {
        Y = smart_calc(byt);
        y[i] = Y;
      }
      i++;
    }
    ui->widget->addGraph();
    ui->widget->graph(0)->setData(x, y);
    ui->widget->graph(0)->setPen(QColor(127, 127, 255, 127));
    ui->widget->graph(0)->setLineStyle(QCPGraph::lsNone);
    ui->widget->graph(0)->setScatterStyle(
        QCPScatterStyle(QCPScatterStyle::ssCircle, 2));
    ui->widget->addGraph();
    ui->widget->graph(0)->addData(x, y);
    ui->widget->xAxis->setRange(xBegin, xEnd);
    ui->widget->yAxis->setRange(y_min - 0.5, y_max + 0.5);
    ui->widget->replot();
    x.clear();
    y.clear();
  }
}

void MainWindow::on_pushButton_graph_2_clicked() {
  credit = new CreditCalc(this);
  credit->show();
}
