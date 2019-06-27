#ifndef STARTWINDOW_H
#define STARTWINDOW_H

#include <QMainWindow>
#include <QTranslator>
#include <QDir>

namespace Ui {
class StartWindow;
}

class StartWindow : public QMainWindow
{
    Q_OBJECT
  protected:
  // this event is called, when a new translator is loaded or the system language is changed
  void changeEvent(QEvent*);

 protected slots:
  // this slot is called by the language menu actions
  void slotLanguageChanged(QAction* action);

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

public:
    explicit StartWindow(QWidget *parent = nullptr);
    ~StartWindow();

};

#endif // STARTWINDOW_H
