#include "creditcalc.h"

#include <cmath>

#include "ui_creditcalc.h"

CreditCalc::CreditCalc(QWidget *parent)
    : QDialog(parent), ui(new Ui::CreditCalc) {
  ui->setupUi(this);
}

CreditCalc::~CreditCalc() { delete ui; }

void CreditCalc::on_pushButton_start_clicked() {
  ui->label_mounhly_payment_2->setText(NULL);
  ui->label_overpayment_2->setText(NULL);
  ui->label_total_payment_2->setText(NULL);
  ui->label_overpayment_2->setText(NULL);
  ui->label_diff_m_payment->setText(NULL);
  ui->textEdit_diff->setText(NULL);
  QByteArray line = NULL;
  QString new_label1 = NULL;
  QString x_line = NULL;
  QString new_label2 = NULL;
  QString new_label3 = NULL;
  QString dif_payment_1;
  QString dif_payment_2;
  QString counter = NULL;
  double a, c, d;
  int b;
  a = ui->lineEdit_amount_credit->text().toDouble(); /* сумма кредита */
  b = ui->lineEdit_term->text().toInt();             /* срок */
  c = ui->lineEdit_interest_rate->text().toDouble(); /* процентная ставка */
  double amount = a;
  double mounhly_payment = 0.0;
  double overpayment = 0.0;
  double total_payment = 0.0;
  if (a < 1 || b < 1 || c < 1) {
    ui->label_diff_m_payment->setText("ERROR. Incorrect input");
  } else {
    if (ui->radioButton_annuity->isChecked()) {
      d = c / 1200; /* ставка по кредиту в месяц */
      new_label1 = "(D/(1-(1+D)^(-B)))*A=";
      x_line = new_label1.replace("A", (QString::number(a, 'g', 10)));
      x_line = new_label1.replace("B", (QString::number(b, 'g', 10)));
      x_line = new_label1.replace("D", (QString::number(d, 'g', 10)));
      line = x_line.toLocal8Bit();

      const char *str = line.data();
      char byt[255] = "";
      strncpy(byt, str, 255);
      if (*str) {
        int num_err = s21_correct_num_act(byt);
        int verify_err = s21_verifying(byt);
        if (num_err == 0 && verify_err == 0) {
          mounhly_payment = smart_calc(byt);
          total_payment = mounhly_payment * b;

          new_label1 = QString::number(mounhly_payment, 'f', 2);

        } else {
          ui->label_diff_m_payment->setText("ERROR. Incorrect input");
        }
      }
    } else if (ui->radioButton_diff->isChecked()) {
      //          QString label_diff_mo_payment = " ";
      double month = a / b; /* ежемесячная плата основного долга */
      double i_payment = 30 * c / 36500;
      double first_payment = 0.0;
      double last_payment = 0.0;
      for (int x = 0; x < b; x++) {
        mounhly_payment = i_payment * a + month;
        total_payment = total_payment + mounhly_payment;
        if (a == amount) {
          first_payment = mounhly_payment;
        }
        last_payment = mounhly_payment;
        a = a - month;
        QString new_label4 = QString::number(mounhly_payment, 'f', 2);
        counter = QString::number(x + 1, 'g', 3);
        ui->label_diff_m_payment->setText(ui->label_diff_m_payment->text() +
                                          "Платёж №" + counter + "=" +
                                          new_label4 + "\n");
      }

      new_label1 = QString::number(month, 'f', 2) + "+\%";
      dif_payment_1 = QString::number(first_payment, 'f', 2);
      dif_payment_2 = QString::number(last_payment, 'f', 2);
    } else {
      ui->label_diff_m_payment->setText("Выберите тип расчётов");
    }
    if (b > 24) {
      ui->textEdit_diff->setText(ui->label_diff_m_payment->text());
    }
    new_label2 = QString::number(total_payment, 'f', 2);
    overpayment = total_payment - amount;
    new_label3 = QString::number(overpayment, 'f', 2);
    ui->label_mounhly_payment_2->setText(new_label1);
    ui->label_total_payment_2->setText(new_label2);
    ui->label_overpayment_2->setText(new_label3);
    ui->label_first_month->setText(dif_payment_1);
    ui->label_last_month->setText(dif_payment_2);
  }
}
