#include "startwindow.h"
#include "ui_startwindow.h"

StartWindow::StartWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::StartWindow)
{
    ui->setupUi(this);
    //createLanguageCombo();
    //loadLanguage(m_currLang);
    connectMenu();
    loadAllQuizes();
  }

StartWindow::~StartWindow()
{
    delete ui;
}
// --------------------- Language processing ---------------------------------- //
void StartWindow::createLanguageCombo(void)
{
 QActionGroup* langGroup = new QActionGroup(ui->comboLang);
 langGroup->setExclusive(true);

 connect(langGroup, SIGNAL (triggered(QAction *)), this, SLOT (slotLanguageChanged(QAction *)));

 // format systems language
 QString defaultLocale = QLocale::system().name(); // e.g. "ru_RU"
 defaultLocale = defaultLocale.toLower(); // ru_RU => ru_ru
 //defaultLocale.truncate(defaultLocale.lastIndexOf('_')); // e.g. "ru"

 m_langPath = QApplication::applicationDirPath();
 m_langPath.append("/languages");
 QDir dir(m_langPath);
 QStringList fileNames = dir.entryList(QStringList("*.qm"));

 for (int i = 0; i < fileNames.size(); ++i) {
      // get locale extracted by filename
      QString locale;
      locale = fileNames[i]; // ru_ru.qm
      locale.truncate(locale.lastIndexOf('.')); // "ru_ru"
      //locale.remove(0, locale.indexOf('_') + 1); // ....

     QString lang = QLocale::languageToString(QLocale(locale).language());
     QIcon ico(QString("%1/%2.png").arg(m_langPath).arg(locale));

     QAction *action = new QAction(ico, lang, this);
     action->setCheckable(true);
     action->setData(locale);

     ui->comboLang->insertItem(i,ico,lang);
     //ui->comboLang->addAction(action)

     langGroup->addAction(action);

     // set default translators and language checked
     if (defaultLocale == locale)
     {
     action->setChecked(true);
     }
 }
}

// Called every time, when a menu entry of the language menu is called
void StartWindow::slotLanguageChanged(QAction* action)
{
 if(nullptr != action) {
  // load the language dependant on the action content
  loadLanguage(action->data().toString());
  setWindowIcon(action->icon());
 }
}

void switchTranslator(QTranslator& translator, const QString& filename)
{
 // remove the old translator
 qApp->removeTranslator(&translator);

 // load the new translator
QString path = QApplication::applicationDirPath();
    path.append("/languages/");
 if(translator.load(path + filename)) //Here Path and Filename has to be entered because the system didn't find the QM Files else
  qApp->installTranslator(&translator);
}

void StartWindow::loadLanguage(const QString& rLanguage)
{
 if(m_currLang != rLanguage) {
  m_currLang = rLanguage;
  QLocale locale = QLocale(m_currLang);
  QLocale::setDefault(locale);
  QString languageName = QLocale::languageToString(locale.language());
  switchTranslator(m_translator, QString("TranslationExample_%1.qm").arg(rLanguage));
  switchTranslator(m_translatorQt, QString("qt_%1.qm").arg(rLanguage));
  //ui.statusBar->showMessage(tr("Current Language changed to %1").arg(languageName));
 }
}

void StartWindow::changeEvent(QEvent* event)
{
 if(nullptr != event) {
   switch(event->type()) {
   // this event is send if a translator is loaded
   case QEvent::LanguageChange:
    //ui.retranslateUi(this);
    break;

   // this event is send, if the system, language changes
   case QEvent::LocaleChange:
   {
    QString locale = QLocale::system().name();
    locale.truncate(locale.lastIndexOf('_'));
    loadLanguage(locale);
   }
   break;
     default:
         break;
  }
 }
 QMainWindow::changeEvent(event);
}
// ------------------ menu ---------------------------- //

void StartWindow::connectMenu(){

    connect(ui->actionReLoad_Quizes, &QAction::triggered, this, &StartWindow::menuReload);
    connect(ui->actionExit, &QAction::triggered, this, &QWidget::close);
    connect(ui->actionAbout, &QAction::triggered, this, &StartWindow::menuAbout);
    connect(ui->actionHelp, &QAction::triggered, this, &StartWindow::menuHelp);

}

void StartWindow::menuReload(){
    QMessageBox::about(this, tr("Reloading..."),tr("..."));
}

void StartWindow::menuHelp(){
    QMessageBox::about(this, tr("Helping..."),tr("..."));
}

void StartWindow::menuAbout(){
QMessageBox::about(this, tr("About..."),
           tr("<b>QuizTest</b>" "Maxim Potkalo" "2019"));
}



// --------------- main field ---------------------- //

void StartWindow::buttonStart(){
   QMessageBox::about(this, tr("START"),tr("!!!"));
}
void StartWindow::comboLang(){

}
void StartWindow::comboQuiz(){

}


// ---------------------------- Quiz processing ------------------------------ //

void StartWindow::loadAllQuizes(){
    std::vector<QString> tmpQuiz = {"Example Quiz EN GB", "One_~_Ans1_C;_;_Ans2_;_Ans3______;_Ans4*_*_01","QuestionTwoTextHere_~_QuestionTwoAnswerOne_;_QuestionTwoAnswerTwo_C_;_QuestionTwoAnswerThree_;_QuestionTwoAnswerFour_C_*_02,04"};
    m_QizPath = QApplication::applicationDirPath();
    m_QizPath.append("/data");
    QDir dir(m_QizPath);
    QStringList QuizfileTextNames = dir.entryList(QStringList("*.txt"));
    QStringList QuizfileImgNames = dir.entryList(QStringList("*.jpg"));
    std::vector<QuizTest> AllQuizes;
    QDir::setCurrent("/data");
    //int END = QuizfileTextNames.size();
    size_t END = 1;
    for (size_t i = 0; i < END; ++i) {
         //open each Quiz file
        QuizTest newQuiz;
         QFile quiz(QuizfileTextNames[i]);

         if (!quiz.open(QIODevice::ReadWrite | QIODevice::Text))
                 return;

         bool quizname = true;
         auto it = tmpQuiz.begin();
         while (it != tmpQuiz.end()) {

            //QByteArray line = quiz.readLine();
             QString line = *it++;
            if (quizname){
                quizname = false;
                newQuiz.SetName(line);
                continue;
            }
            newQuiz.loadQuestion(line);

            }
    }
int a = 0;
}
