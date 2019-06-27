#include "startwindow.h"
#include "ui_startwindow.h"

StartWindow::StartWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::StartWindow)
{
    ui->setupUi(this);
    createLanguageCombo();
    //loadLanguage();
  }

StartWindow::~StartWindow()
{
    delete ui;
}

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
