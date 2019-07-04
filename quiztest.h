#ifndef QUIZTEST_H
#define QUIZTEST_H

#include <vector>
#include <map>

#include <QException>

#include <QString>
#include <QPixmap>


class QuizTest
{
public:
    QuizTest();

    void loadQuestion(const QString& QuestionString, const QString ImagesInfo = nullptr);
    inline std::pair<const QString&, const std::map<QPixmap*, QString*>&> showQuestion(const size_t QuestionNumber)  const;
    inline std::pair<const QString&, const std::map<QPixmap*, QString*>&> showAnswer(const size_t QuestionNumber, const size_t AnswerNumber)  const;
    inline bool checkAnswer(const size_t QuestionNumber, const size_t AnswerNumber) const;
    void getImagesForQuiz(std::map<QPixmap, QString>& LnkToMapImgs);
    void SetName(const QString Name);

private:
    void processImageInfo(const QString& ImagesInfo);
    struct Answer {
        QString  AnswerText;
        std::map<QPixmap*, QString*> AnswerImgs; //map<Img, Img options>
    };


    struct Question {
       //explicit Question(std::vector<QPixmap>& ImgVectorToMove);
       QString  QuestionText;
       std::map<QPixmap*, QString*> QuestionImgs; //map<Img, Img options>
       std::vector<Answer> AllAnswers;
       std::vector<bool> CorrectAnswers; //vector.size() == amount of Answers e.g for 6 Answers vector.size()==6
    };
    std::map<QPixmap, QString> AllImages;
    std::vector<Question> Questions;
    QString QuizName;
};

#endif // QUIZTEST_H
