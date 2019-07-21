#include "resultdialog.h"
#include "ui_resultdialog.h"

ResultDialog::ResultDialog(QWidget *parent, const std::vector<std::vector<unsigned int>>* results_ptr) :
    QDialog(parent),
    ui(new Ui::ResultDialog)
{
    ui->setupUi(this);
    if(results_ptr == nullptr) throw std::runtime_error("ERROR! results_ptr in resultdialog must not be empty!");
    for (auto item : *results_ptr) {
        //придумать обработку ответов, пока тут фигня
        if (item.at(0) != 0) {
            Correct_answers++;
        }
        else {
            Wrong_answers++;
        }
    }
    //ui->tableResults->
}

ResultDialog::~ResultDialog()
{
    delete ui;
}

void ResultDialog::on_pushOK_clicked()
{
    QWidget::close();
}
