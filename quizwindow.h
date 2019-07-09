#ifndef QUIZWINDOW_H
#define QUIZWINDOW_H

#include <QMainWindow>
#include <QDialog>
#include <QMessageBox>
#include "quiztest.h"
#include "resultdialog.h"

namespace Ui {
class QuizWindow;
}
//окна для нескольких вариантов ответа, картинок и ввода ответа вручную = сделать наследниками этого класса

class QuizWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit QuizWindow(QWidget *parent = nullptr);
    ~QuizWindow();
    void SetQuiz(const QuizTest* ActiveQuiz);

private slots:

    void on_pushButtonAnswer_1_clicked();

    void on_pushButtonAnswer_2_clicked();

    void on_pushButtonAnswer_3_clicked();

    void on_pushButtonAnswer_4_clicked();

private:
    Ui::QuizWindow *ui;

    void ShowQuestion() const;
    void ProcessButtonClick(unsigned int btn_number);
    void ShowAnswerResult(const bool param) const;
    void ShowTotalResult()  const;
    const QuizTest* currentQuiz;
    unsigned int CurrentQuestionNumber = 0;
    std::vector<bool> UserAnswersList;
};

#endif // QUIZWINDOW_H
