#ifndef RESULTDIALOG_H
#define RESULTDIALOG_H

#include <QDialog>

namespace Ui {
class ResultDialog;
}

class ResultDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ResultDialog(QWidget *parent = nullptr, const std::vector<std::vector<unsigned int>>* results_ptr = nullptr);
    ~ResultDialog();

private slots:
    void on_pushOK_clicked();

private:
    Ui::ResultDialog *ui;

    int Correct_answers = 0;
    int Wrong_answers = 0;
};

#endif // RESULTDIALOG_H
