#ifndef RESULTDIALOG_H
#define RESULTDIALOG_H

#include <QDialog>
#include <QTableWidget>

namespace Ui {
class ResultDialog;
}

class ResultDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ResultDialog(QWidget *parent = nullptr, const std::vector<std::vector<int>>* results_ptr = nullptr);
    ~ResultDialog();

private slots:
    void on_pushOK_clicked();

private:
    Ui::ResultDialog *ui;
    void SetFonts();
    void MakeDefaultItem( QTableWidgetItem *newitem);
    void AddItem(const QString itemtext, const int counter);
    void ConstructIndividualResultList();
    void ConstructOverallResultList();

    QFont DefaultFont;
    const std::vector<std::vector<int>>* pResults;
    double CorrectAnswers = 0.0;
    double WrongAnswers = 0.0;
};

#endif // RESULTDIALOG_H
