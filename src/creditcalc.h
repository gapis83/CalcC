#ifndef CREDITCALC_H
#define CREDITCALC_H

#include <QDialog>
#ifdef __cplusplus
extern "C" {
#endif
#include "s21_smart_calc.h"
}
#endif

namespace Ui {
class CreditCalc;
}

class CreditCalc : public QDialog {
  Q_OBJECT

 public:
  explicit CreditCalc(QWidget *parent = nullptr);
  ~CreditCalc();

 private slots:
  void on_pushButton_start_clicked();

 private:
  Ui::CreditCalc *ui;
};
