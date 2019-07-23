#include "resultdialog.h"
#include "ui_resultdialog.h"

ResultDialog::ResultDialog(QWidget *parent, const std::vector<std::vector<int>>* results_ptr) :
    QDialog(parent),
    ui(new Ui::ResultDialog),
    pResults(results_ptr)
{
    if(pResults == nullptr) throw std::runtime_error("ERROR! results_ptr in resultdialog must not be empty!");
    ui->setupUi(this);
    SetFonts();
    ConstructIndividualResultList();
    ConstructOverallResultList();
}

ResultDialog::~ResultDialog()
{
    delete ui;
}

void ResultDialog::on_pushOK_clicked()
{
    QWidget::close();
}

void ResultDialog::SetFonts(){
    DefaultFont.setFamily("MS Shell Dlg 2");
    DefaultFont.setBold(true);
    DefaultFont.setPointSize(12);
}

void ResultDialog::MakeDefaultItem( QTableWidgetItem *input){
    input->setFlags(Qt::NoItemFlags);
    input->setFont(DefaultFont);
    input->setTextAlignment(Qt::AlignCenter);
}

void  ResultDialog::AddItem(const QString itemtext, const int counter){
    QTableWidgetItem *newitem = new QTableWidgetItem;
    MakeDefaultItem(newitem);
    newitem->setText(itemtext);
    ui->tableResult_deatil->setItem(counter, 0, newitem);
}

void ResultDialog::ConstructIndividualResultList(){

    ui->tableResult_deatil->setRowCount(static_cast<int>(pResults->size()));
    int rows_cnt = 0;

    for (auto questionresult : *pResults) {
        if(questionresult.size() == 1){
            if(questionresult.at(0) != 0){
                AddItem("+", rows_cnt);
                CorrectAnswers++;
            }
            else {
                AddItem("-", rows_cnt);
                WrongAnswers++;
            }
            rows_cnt++;
        }else {
            for(auto item:questionresult){
                //for multi-annswer questions, do not work yet
                int A = item;
                A++;
            }
        }
    }
}

void ResultDialog::ConstructOverallResultList(){
    QTableWidgetItem *CorrAmount = new QTableWidgetItem();
    QTableWidgetItem *WrongAmount = new QTableWidgetItem();
    QTableWidgetItem *CorrPercent = new QTableWidgetItem();
    QTableWidgetItem *WrongPercent = new QTableWidgetItem();
    MakeDefaultItem(CorrAmount);
    MakeDefaultItem(WrongAmount);
    MakeDefaultItem(CorrPercent);
    MakeDefaultItem(WrongPercent);

    CorrAmount->setText(QString::number(CorrectAnswers));
    WrongAmount->setText(QString::number(WrongAnswers));
    CorrPercent->setText(QString::number(100*CorrectAnswers/(CorrectAnswers + WrongAnswers)) + " %");
    WrongPercent->setText(QString::number(100*WrongAnswers/(CorrectAnswers + WrongAnswers)) + " %");
    ui->tableResult_total->setItem(0, 0, CorrAmount);
    ui->tableResult_total->setItem(0, 1, CorrPercent);
    ui->tableResult_total->setItem(1, 0, WrongAmount);
    ui->tableResult_total->setItem(1, 1, WrongPercent);


}

