#ifndef STARTWINDOW_H
#define STARTWINDOW_H

#include <QMainWindow>
#include <QTranslator>
#include <QDir>

#include <QAction>
#include <QMessageBox>

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
  // this event is called, when a new translator is loaded or the system language is changed
  void changeEvent(QEvent*);

 protected slots:
  // this slot is called by the language menu actions
  void slotLanguageChanged(QAction* action);

  void connectMenu();

  void menuReload();
  void menuHelp();
  void menuAbout();

  void buttonStart();
  void comboLang();
  void comboQuiz();

 private:
  // loads a language by the given language shortcur (e.g. de, en)
  void loadLanguage(const QString& rLanguage);

  // creates the language menu dynamically from the content of m_langPath
  void createLanguageCombo(void);

  Ui::StartWindow *ui;
  QTranslator m_translator; // contains the translations for this application
  QTranslator m_translatorQt; // contains the translations for qt
  QString m_currLang; // contains the currently loaded language
  QString m_langPath; // Path of language files. This is always fixed to /languages.

  void loadAllQuizes();
  QString  m_QizPath;
};

#endif // STARTWINDOW_H
