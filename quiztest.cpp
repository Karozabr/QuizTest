#include "quiztest.h"

QuizTest::QuizTest(){}

void  QuizTest::processImageInfo(const QString& ImagesInfo){
    QString A =  ImagesInfo;
}

void QuizTest::SetNameAndParams(const QString& First_String){
    auto qsrt_it = First_String.begin();
    while (*qsrt_it != '@'){
    QuizName.push_back(*qsrt_it++);
    }
    qsrt_it++;
    while (qsrt_it != First_String.end()){
    QuizParams.push_back(*qsrt_it++);
    }

}
unsigned int QuizTest::GetQuizSize() const{
    return Questions.size();
}

QString QuizTest::GetName() const{
    return QuizName;
}

void QuizTest::loadQuestionText(const QString& QuestionString,  const QString ImagesInfo){
    Question newQ;
    if(!AllImages.empty()){
     if(ImagesInfo == nullptr) throw std::runtime_error("Images are present, ImageInfo should not be nullptr!");
        processImageInfo(ImagesInfo);
    }
    bool QuestionIsFinished = false;
    bool AnswersAreFinished = false;
    Answer QAnswer_tmp;
    auto it = QuestionString.begin();
    //int pos = 0;
    while(it !=QuestionString.end()){
        QChar posChar = *it;
        if(QuestionIsFinished && AnswersAreFinished){
           if(posChar == ',') {
           it++;
           continue;
           }
           int number = posChar.digitValue();
           if(number == -1){
               throw std::runtime_error("CorrectAnswer is not digit!");
           }
           else {
            newQ.CorrectAnswers.at(static_cast<size_t>(number-1)) = true;
            it++;
            continue;
           }
        }
        if(posChar == '_'){ //checks for separator
            if(*(it+1) == '~' && *(it+2) == '_'){
                            //separator for Question END found
                            QuestionIsFinished = true;
                            it+=3;
                            continue;
                            } else if (*(it+1) == ';' && *(it+2) == '_') {
                                    //separator for Answers found
                                    newQ.AllAnswers.push_back(QAnswer_tmp);
                                    QAnswer_tmp.AnswerText.clear();
                                    it+=3;
                                    continue;
                                    }else if (*(it+1) == '*' && *(it+2) == '_') {
                                           //separator for Answers END found
                                           AnswersAreFinished = true;
                                           newQ.AllAnswers.push_back(QAnswer_tmp);
                                           newQ.CorrectAnswers.resize(newQ.AllAnswers.size());
                                           it+=3;
                                           continue;
                                           }
        }
        if(!QuestionIsFinished) newQ.QuestionText.push_back(posChar);
        if(!AnswersAreFinished) {
            QAnswer_tmp.AnswerText.push_back(posChar);
        }
        it++;
    }
    Questions.push_back(newQ);
    QString a = "For Break Point";
}

std::pair<const QString&, const std::map<QPixmap*, QString*>&>
QuizTest::GetQuestion(const size_t QuestionNumber)  const{
//const QString& tmpSlnk =  Questions.at(Qnumber).QuestionText;
//const std::vector<QPixmap>& tmpVlnk =  Questions.at(Qnumber).QuestionImgs;
//    return std::make_pair(tmpSlnk, tmpVlnk);
    return std::make_pair(Questions.at(QuestionNumber).QuestionText, Questions.at(QuestionNumber).QuestionImgs);
}

std::pair<const QString&, const std::map<QPixmap*, QString*>&>
QuizTest::GetSingleAnswer(const size_t QuestionVectorItem, const size_t AnswerVectorItem)  const{
    return std::make_pair(Questions.at(QuestionVectorItem).AllAnswers.at(AnswerVectorItem).AnswerText, Questions.at(QuestionVectorItem).AllAnswers.at(AnswerVectorItem).AnswerImgs);
}

const std::vector<QString> QuizTest::GetAnswersText(const size_t QuestionVectorItem)  const{
    std::vector<QString> result;
    for (size_t i = 0; i < Questions.at(QuestionVectorItem).AllAnswers.size(); i++) {
        result.push_back(Questions.at(QuestionVectorItem).AllAnswers.at(i).AnswerText);
    }
    return result;
}

bool QuizTest::checkAnswer(const size_t QuestionVectorItem, const size_t AnswerVectorItem) const{
    if(Questions.at(QuestionVectorItem).CorrectAnswers.at(AnswerVectorItem) == true) {return true;}
    else {return false;}
}
