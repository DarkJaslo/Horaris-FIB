#include "MyForm.h"

MyForm::MyForm(QWidget* parent) : QWidget(parent)
{
  ui.setupUi(this);

  ui.appWidget->receiveListWidget(ui.listWidget);
  ui.appWidget->receiveSelectionLists(ui.listInclude, ui.listAlways);
  ui.appWidget->receiveStudiesBox(ui.studiesComboBox);
  //ui.listWidget->setSortingEnabled(true);
  ui.listWidget->setFocusPolicy(Qt::FocusPolicy::ClickFocus);
  ui.listInclude->setFocusPolicy(Qt::FocusPolicy::ClickFocus);
  ui.listAlways->setFocusPolicy(Qt::FocusPolicy::ClickFocus);
  
  std::cout << "setup ui" << std::endl;
  ui.appWidget->getSemesters();
  std::cout << "got semesters" << std::endl;
  ui.appWidget->getData();
  std::cout << "got data" << std::endl;

  
}