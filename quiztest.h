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

    std::pair<const QString&, const std::map<QPixmap*, QString*>&>  GetQuestion(const size_t QuestionNumber)  const;
    void                                                            loadQuestionText(const QString& QuestionString);
    std::pair<const QString&, const std::map<QPixmap*, QString*>&>  GetSingleAnswer(const size_t QuestionVectorItem, const size_t AnswerVectorItem)  const;
    const std::vector<QString>                                      GetAnswersText(const size_t QuestionVectorItem)  const;
    bool                                                            checkAnswer(const size_t QuestionVectorItem, const size_t AnswerVectorItem) const;
    void                                                            getImagesForQuiz(std::map<QPixmap, QString>& LnkToMapImgs);
    void                                                            SetNameAndParams(const QString& First_String);
    QString                                                         GetName() const;
    size_t                                                          GetQuizSize() const;

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
    QString QuizParams;
};

#endif // QUIZTEST_H
