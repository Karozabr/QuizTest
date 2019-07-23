#ifndef STARTWINDOW_H
#define STARTWINDOW_H

#include <QMainWindow>
#include <QDir>
#include <QTranslator>
#include <QMessageBox>


#include "quiztest.h"
#include "quizwindow.h"

namespace Ui {
class StartWindow;
}

class StartWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit StartWindow(QWidget *parent = nullptr);
    ~StartWindow();

protected:
 void changeEvent(QEvent* event);

protected slots:
 void connectMenu(); // this slot is called connect all the main menu actions
 void menuHelp();  // this slot is called by the Help menu
 void menuAbout();  // this slot is called by the About menu
 void buttonStart();   // this slot is called by the Strat button action

private:
  Ui::StartWindow *ui;
  QString cfg_PATH;
  QString cfg_DefaultLang;
  std::vector<QString> SupportedLanguages;
  void SetSupportedLanguages();
  int checkLocale ();
  void processCFGfile(); //getting and processing data from config.qtc
  void switchTranslator(QTranslator& translator, const QString& NewLanguageName);
  void loadLanguage(const int LanguageIndex); // loads a language by the given language index
  void GetDefaultLanguage(); // gets language form en_gb.qm files
  int CurrentLang; // contains the currently loaded language index
  QTranslator translator;

  void loadAllQuizes(); //loading all Quizes from files into memory
  void MakeComboQuiz(); //making comboBOX from memory data
  void SelectQuizComboBox(); //set pSelectedQuiz form selected Quiz in ComboBox

  QuizWindow* pQuizWindow = nullptr; //pointer to window showing Quiz
  QuizTest* pSelectedQuiz = nullptr; //pointer to selected Quiz in comboBOX
  std::map<QString, QuizTest> AllQuizMap; //map <name,content>

  std::map<QString, std::vector<std::vector<int>>> UserAnswersHistoryForQuizes;
};

#endif // STARTWINDOW_H
