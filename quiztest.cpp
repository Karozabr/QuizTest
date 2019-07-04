#include "quiztest.h"

QuizTest::QuizTest()
{

}
//QuizTest::Question::Question(std::vector<QPixmap>& ImgVectorToMove):QuestionImgs(ImgVectorToMove){}
void  QuizTest::processImageInfo(const QString& ImagesInfo){

}

void QuizTest::SetName(const QString QName){
    QuizName = QName;
}

void QuizTest::loadQuestion(const QString& QuestionString,  const QString ImagesInfo){
    Question newQ;
    if(!AllImages.empty()){
     if(ImagesInfo == nullptr) throw std::runtime_error("Images are present, ImageInfo should not be nullptr!");
        processImageInfo(ImagesInfo);
    }
    bool QuestionIsFinished = false;
    bool AnswersAreFinished = false;
    Answer QAnswer_tmp;
    QString correcttmp;
    for (int pos = 0; pos < QuestionString.size() + 1; pos++){
        QChar posChar = QuestionString.at(pos);
        if(QuestionIsFinished && AnswersAreFinished){
           if(posChar == ',' || pos == QuestionString.size()) {
               newQ.CorrectAnswers.at(correcttmp.toUInt()) = true;
               continue;
           }
           correcttmp.push_back(posChar);
           continue;
        }
        if(posChar == '_'){ //checks for separator
            if(QuestionString.at(pos+1) == '~' && QuestionString.at(pos+2) == '_'){
                            //separator for Question END found
                            QuestionIsFinished = true;
                            pos+=2;
                            continue;
                            } else if (QuestionString.at(pos+1) == ';' && QuestionString.at(pos+2) == '_') {
                                    //separator for Answers found
                                    newQ.AllAnswers.push_back(QAnswer_tmp);
                                    pos+=2;
                                    continue;
                                    }else if (QuestionString.at(pos+1) == '*' && QuestionString.at(pos+2) == '_') {
                                           //separator for Answers END found
                                           AnswersAreFinished = true;
                                           newQ.AllAnswers.push_back(QAnswer_tmp);
                                           newQ.CorrectAnswers.resize(newQ.AllAnswers.size());
                                           pos+=2;
                                           continue;
                                           }
        }
        if(!QuestionIsFinished) newQ.QuestionText.push_back(posChar);
        if(!AnswersAreFinished) {
            QAnswer_tmp.AnswerText.push_back(posChar);
        }


    }
    Questions.push_back(newQ);
    int a =1;
}








inline std::pair<const QString&, const std::map<QPixmap*, QString*>&> QuizTest::showQuestion(const size_t QuestionNumber)  const{
//const QString& tmpSlnk =  Questions.at(Qnumber).QuestionText;
//const std::vector<QPixmap>& tmpVlnk =  Questions.at(Qnumber).QuestionImgs;
//    return std::make_pair(tmpSlnk, tmpVlnk);
    return std::make_pair(Questions.at(QuestionNumber).QuestionText, Questions.at(QuestionNumber).QuestionImgs);
}

inline std::pair<const QString&, const std::map<QPixmap*, QString*>&> QuizTest::showAnswer(const size_t QuestionNumber, const size_t AnswerNumber)  const{
    return std::make_pair(Questions.at(QuestionNumber).AllAnswers.at(AnswerNumber).AnswerText, Questions.at(QuestionNumber).AllAnswers.at(AnswerNumber).AnswerImgs);
}

inline bool QuizTest::checkAnswer(const size_t QuestionNumber, const size_t AnswerNumber) const{
    if(Questions.at(QuestionNumber).CorrectAnswers.at(AnswerNumber) == true) {return true;}
    else {return false;}
}
