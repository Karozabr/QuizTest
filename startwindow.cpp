#include "startwindow.h"
#include "ui_startwindow.h"

StartWindow::StartWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::StartWindow)
{
    ui->setupUi(this);
    pQuizWindow =  new QuizWindow(nullptr, &UserAnswersHistoryForQuizes);
    connect(pQuizWindow, &QMainWindow::windowTitleChanged, this, &StartWindow::show);
    connect(ui->comboLang, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &StartWindow::loadLanguage);

    SetSupportedLanguages();
    processCFGfile();
    GetDefaultLanguage();
    connectMenu();
    loadAllQuizes();
    MakeComboQuiz();
  }

StartWindow::~StartWindow()
{
    delete ui;
}

// ------------------------ Configurating program ----------------------------- //
void StartWindow::SetSupportedLanguages(){
    //pushback order must be the same as Language combo box items order
    SupportedLanguages.push_back("en_gb");
    SupportedLanguages.push_back("ru_ru");
    SupportedLanguages.push_back("zh_cn");
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

int StartWindow::checkLocale (){
    // check if systems language is suported by program
    QString systemLocale = QLocale::system().name(); // e.g. "ru_RU"
    systemLocale = systemLocale.toLower(); // ru_RU => ru_ru
    for (size_t i = 0; i < SupportedLanguages.size(); i++) {
        if(systemLocale == SupportedLanguages.at(i)){
            cfg_DefaultLang = SupportedLanguages.at(i);
            return static_cast<int>(i);
        }
    }
    return 0;
}

void StartWindow::GetDefaultLanguage()
{

 int InitialLangIndex = checkLocale(); //==0 if locale lang is not supported, else == supported lang index
 if(cfg_DefaultLang.isEmpty() && InitialLangIndex == 0) cfg_DefaultLang = "en_gb";
 QString filename = cfg_DefaultLang + ".qm";
 ui->comboLang->setCurrentIndex(InitialLangIndex);
 switchTranslator(translator, filename);
}

void StartWindow::switchTranslator(QTranslator& translator, const QString& NewLanguageFileName)
{
 // remove the old translator
 qApp->removeTranslator(&translator);

 // load the new translator
 QString path = QApplication::applicationDirPath();
     path.append("/translations/");
  if(translator.load(path + NewLanguageFileName)) //Here Path and Filename has to be entered because the system didn't find the QM Files else
   qApp->installTranslator(&translator);

}

void StartWindow::loadLanguage(const int LanguageIndex)
{
 if(CurrentLang != LanguageIndex) {
     QString lang = SupportedLanguages.at(static_cast<size_t>(LanguageIndex));
     QLocale locale = QLocale(lang);
     QLocale::setDefault(locale);
     lang.append(".qm");
     switchTranslator(translator, lang);
     CurrentLang = LanguageIndex;
 }
}

void StartWindow::changeEvent(QEvent* event)
{
 if(nullptr != event) {
   switch(event->type()) {
   // this event is send if a translator is loaded
       case QEvent::LanguageChange: {
       ui->retranslateUi(this);
       break;
       }
   // this event is send, if the system, language changes
   case QEvent::LocaleChange: {
       loadLanguage(checkLocale());
       }
       break;
   default:
       break;
  }
 }
 QMainWindow::changeEvent(event);
}

// ------------------ Main Window actions ---------------------------- //

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
    //: This is About window tilte
    QString Title = tr("About...");
    //: This is program name inside About window
    QString ProgramName = tr("QuizTest");
    //: This is author name inside About window
    QString Author = tr("Maxim Potkalo");
    //: This is Date inside About window
    QString Date = tr("2019");
    //: This is program additional info inside About window
    QString Info = tr("ver. 1.0");
    QString FullAboutText = ProgramName + ("\n ") + Author + ("\n ") + Date + ("\n ") + Info;
    QMessageBox::about(this, Title, FullAboutText);
}

void StartWindow::buttonStart(){
    //: This is Error message on Buttont start in start window
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
            if(tmp == "ENDFILE") continue;
            tmp.chop(1); //droping endLine simbol
            newQuiz.push_back(tmp);
        }
        DataReadFromFile.push_back(newQuiz);
    }
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
