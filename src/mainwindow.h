#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <iostream>
#include <string>

#include "creditcalc.h"

#ifdef __cplusplus
extern "C" {
#endif
#include "s21_smart_calc.h"
}
#endif

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
  Q_OBJECT

 public:
  MainWindow(QWidget *parent = nullptr);
  ~MainWindow();

 private slots:
  void input_line();
  void on_pushButton_AC_clicked();
  void operations();
  void on_pushButton_equals_clicked();
  void on_pushButton_graph_clicked();
  void on_pushButton_graph_2_clicked();

 private:
  Ui::MainWindow *ui;
  CreditCalc *credit;
  double xBegin, xEnd, h, X, Y;
  QVector<double> x, y;

  // signals:
  //     void signal(QString);
};
