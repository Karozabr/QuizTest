#include "quizwindow.h"
#include "ui_quizwindow.h"


QuizWindow::QuizWindow(QWidget *parent, std::map<QString, std::vector<std::vector<int>>>* UserAnswersHistoryForQuizes) :
    QMainWindow(parent), ui(new Ui::QuizWindow)
{
    if(UserAnswersHistoryForQuizes == nullptr) throw std::runtime_error("ERROR! UserAnswersHistoryForQuizes in quizwindow must not be nullptr!");
    pUserAnswersHistory = UserAnswersHistoryForQuizes;
    ui->setupUi(this);
    setWindowFlags(Qt::Window
        | Qt::WindowMinimizeButtonHint
        | Qt::WindowMaximizeButtonHint);
}

QuizWindow::~QuizWindow()
{
    delete ui;
}


void QuizWindow::SetQuiz(const QuizTest* ActiveQuiz){
    currentQuiz = ActiveQuiz;
    CurrentQuizUserAnswersList.erase(CurrentQuizUserAnswersList.begin(),CurrentQuizUserAnswersList.end());
    CurrentQuizUserAnswersList.resize(currentQuiz->GetQuizSize());
    this->setWindowTitle(currentQuiz->GetName());
    ShowQuestion();
}

void QuizWindow::ShowQuestion() const{

    ui->labelQuestionText->setText(currentQuiz->GetQuestion(CurrentQuestionNumber).first);
    ui->pushButtonAnswer_1->setText(currentQuiz->GetAnswersText(CurrentQuestionNumber).at(0));
    ui->pushButtonAnswer_2->setText(currentQuiz->GetAnswersText(CurrentQuestionNumber).at(1));
    ui->pushButtonAnswer_3->setText(currentQuiz->GetAnswersText(CurrentQuestionNumber).at(2));
    ui->pushButtonAnswer_4->setText(currentQuiz->GetAnswersText(CurrentQuestionNumber).at(3));
}

void QuizWindow::ProcessButtonClick(unsigned int btn_number){
    bool result = currentQuiz->checkAnswer(CurrentQuestionNumber, btn_number - 1);
    if(result){
        CurrentQuizUserAnswersList.at(CurrentQuestionNumber).push_back(static_cast<int>(btn_number));

    }else {
        CurrentQuizUserAnswersList.at(CurrentQuestionNumber).push_back(0);

    }
    ShowAnswerResult(result);
    CurrentQuestionNumber++;
    if(CurrentQuestionNumber + 1 > currentQuiz->GetQuizSize()){
        ShowTotalResult();
        this->hide();
        this->setWindowTitle("HIDE");
        CurrentQuestionNumber = 0;
        return;
    }
    ShowQuestion();
}

void QuizWindow::ShowAnswerResult(const bool param) const{
    QString result;
    if(param){
        //: Correct answer notification
       result =tr("C o r r e c t !");
    }else {
        //: Wrong answer notification
       result = tr("W r o n g !");
    }
    QMessageBox R;
    R.setText(result);
    R.exec();
}

void QuizWindow::ShowTotalResult() const{

    pUserAnswersHistory->operator[](currentQuiz->GetName()) = CurrentQuizUserAnswersList;
    ResultDialog Result(nullptr, &CurrentQuizUserAnswersList);
    Result.exec();
}


void QuizWindow::on_pushButtonAnswer_1_clicked()
{
    ProcessButtonClick(1u);
}

void QuizWindow::on_pushButtonAnswer_2_clicked()
{
    ProcessButtonClick(2u);
}

void QuizWindow::on_pushButtonAnswer_3_clicked()
{
    ProcessButtonClick(3u);
}

void QuizWindow::on_pushButtonAnswer_4_clicked()
{
    ProcessButtonClick(4u);
}
