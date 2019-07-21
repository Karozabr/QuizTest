#include "startwindow.h"
#include "ui_startwindow.h"

StartWindow::StartWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::StartWindow)
{
    ui->setupUi(this);
    pQuizWindow =  new QuizWindow();
    connect(pQuizWindow, &QMainWindow::windowTitleChanged, this, &StartWindow::show);
    connect(pQuizWindow, &QMainWindow::destroyed, this, &StartWindow::show);
    //добавить обработку события закрытия окна вопросов (или вообще отключить такую возможность)
    processCFGfile();
    //createLanguageCombo();
    //loadLanguage(m_currLang);
    connectMenu();
    loadAllQuizes();
    MakeComboQuiz();
  }

StartWindow::~StartWindow()
{
    delete ui;
}

void StartWindow::processCFGfile(){
    std::vector<QString> AllCFGNames =
        {"PATH", "DEFLANG"};
    size_t cNamesCnt = 0;
    QString cfgfilename = QApplication::applicationDirPath() + "/config.qtc";
    QFile config(cfgfilename);
    if (!config.open(QIODevice::ReadOnly | QIODevice::Text))
            throw std::runtime_error("ERROR! Cannot find config.qtc!");
    while (!config.atEnd()) {
        QString tmpline = config.readLine();
        if(tmpline == "ENDFILE") return;
        tmpline.chop(1);
        QString thisLine;
        for(auto c:tmpline){
            if(c != " "){
                thisLine.append(c);
            }
            else{
                if(thisLine == AllCFGNames.at(cNamesCnt)){
                thisLine.clear();
                continue;
                }else {throw std::runtime_error("ERROR! config.qtc format is WRONG!");}
            }

        }
        if(thisLine != "NONE"){ //if this line param is NONE => go to next line
            switch (cNamesCnt) {
            case 0: {
                cfg_PATH = thisLine; break;}
            case 1: {
                cfg_DefaultLang = thisLine; break;}
            default: break;
            }
        }
        cNamesCnt++;
    }

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

 QString m_langPath = QApplication::applicationDirPath();
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
 if(CurrentLang != rLanguage) {
  CurrentLang = rLanguage;
  QLocale locale = QLocale(CurrentLang);
  QLocale::setDefault(locale);
  QString languageName = QLocale::languageToString(locale.language());
  switchTranslator(AllTranslatinos, QString("TranslationExample_%1.qm").arg(rLanguage));
  switchTranslator(TranslatorQt, QString("qt_%1.qm").arg(rLanguage));
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


    connect(ui->actionExit, &QAction::triggered, this, &QWidget::close);
    connect(ui->actionAbout, &QAction::triggered, this, &StartWindow::menuAbout);
    connect(ui->actionHelp, &QAction::triggered, this, &StartWindow::menuHelp);
    connect(ui->pushStartTest, &QPushButton::clicked, this, &StartWindow::buttonStart);
    connect(ui->comboTestSelect, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &StartWindow::SelectQuizComboBox);
}

void StartWindow::menuHelp(){
    QDialog Help;
    Help.exec();
}

void StartWindow::menuAbout(){
QMessageBox::about(this, tr("About..."),
           tr("<b>QuizTest</b>" "Maxim Potkalo" "2019"));
}


// --------------- main field ---------------------- //

void StartWindow::buttonStart(){
    if(pSelectedQuiz == nullptr) {QMessageBox::about(this, tr("ERROR!"), tr("No Quiz to start!"));return;}
    pQuizWindow->SetQuiz(pSelectedQuiz);
    pQuizWindow->show();
    this->hide();
 }

// ---------------------------- Quiz processing ------------------------------ //

void StartWindow::loadAllQuizes(){

    std::vector<std::vector<QString>> DataReadFromFile;
    QString fQizPath;
    if(!cfg_PATH.isEmpty()){ //if PATH override in cfg file !=0;
        fQizPath = cfg_PATH;
    }
    else {
        fQizPath = QApplication::applicationDirPath();
        fQizPath.append("/data");
    }
    QDir dir(fQizPath);
    QStringList QuizfileTextNames = dir.entryList(QStringList("*.txt"));
    QStringList QuizfileImgNames = dir.entryList(QStringList("*.jpg"));
    fQizPath.append("/"); //addig "/" for correct full file name
    for (int f = 0; f < QuizfileTextNames.size(); f++) {
    //open each Quiz file
        QString file_path = fQizPath + QuizfileTextNames[f];
        QFile quiz(file_path);
        std::vector<QString> newQuiz;
        if (!quiz.open(QIODevice::ReadOnly | QIODevice::Text))
                return;
        while (!quiz.atEnd()){
            QString tmp = quiz.readLine();
            tmp.chop(1); //droping endLine simbol
            newQuiz.push_back(tmp);
        }
        DataReadFromFile.push_back(newQuiz);
    }
   //----For Test Area ---//
   // std::vector<QString> tmpQuizEn = {"Example Quiz EN GB@THISISPARAMETRS", "One_~_Ans1_C;_;_Ans2_;_Ans3______;_Ans4*_*_1","QuestionTwoTextHere_~_QuestionTwoAnswerOne_;_QuestionTwoAnswerTwo__;_QuestionTwoAnswerThree_;_QuestionTwoAnswerFour_C_*_4"};
   // std::vector<QString> tmpQuizRu = {"Образец вопросника Рус Рус@ЭТОПАРАМЕТРЫ", "ПЕрвый_~_Отв1!!_;_Отв2._;_Отв3__Пр____;_;_Отв44*_*_3","ВопросВторойТут_~_ОТветНаВтойРаз_;_ОтветНаВторойДва_Пр_;_ОТветНаВторойТри_;_ОТветНаВторойЧетыре_*_2"};
   // DataReadFromFile.push_back(tmpQuizEn);
   // DataReadFromFile.push_back(tmpQuizRu);
    //----For Test Area ENDS---//
    for (size_t i = 0; i < DataReadFromFile.size(); ++i) {
         QuizTest newQuiz;
         bool quizname = true;
         auto it = DataReadFromFile.at(i).begin();
         while (it != DataReadFromFile.at(i).end()) {
             QString line = *it++;
            if (quizname){
                quizname = false;
                newQuiz.SetNameAndParams(line);
                continue;
            }
            newQuiz.loadQuestionText(line);
    }

    AllQuizMap[newQuiz.GetName()] = newQuiz;
    }

//    int FOrBreakPOintOnly = 0;
}

void StartWindow::MakeComboQuiz(){

    for (auto it : AllQuizMap) {
        ui->comboTestSelect->addItem(it.first);
      }
}

void StartWindow::SelectQuizComboBox() {
    auto param = ui->comboTestSelect->currentText();
    if(param != nullptr)
    pSelectedQuiz = &AllQuizMap.at(param);
    else return;
}
// -------------------DELET THIS SECTION IN RELEASE-------------------------------------------------- //
void StartWindow::testFunc(){
    QMessageBox::about(this,"TESTFUNC","              ");
}
