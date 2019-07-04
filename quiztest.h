#ifndef QUIZTEST_H
#define QUIZTEST_H

#include <vector>

#include <QString>
#include <QPixmap>


class QuizTest
{
public:
    QuizTest();

    void loadQuestion();
    void showQuestion(const int Qnumber)  const;
    void checkAnswer(const int Qnumber) const;

private:

    struct Answer {
        QString  AnswerText;
        QPixmap AnswerImg;
    };


    struct Question {
       QString  QuestionText;
       QPixmap QuestionImg;
       std::vector<Answer> AllAnswers;
       std::vector<int> CorrectAnswers;
    };

    std::vector<Question> Questions;
};

#endif // QUIZTEST_H
