#include "MyForm.h"

MyForm::MyForm(QWidget* parent) : QWidget(parent)
{
  ui.setupUi(this);

  ui.appWidget->receiveListWidget(ui.listWidget);
  //ui.listWidget->setSortingEnabled(true);
  
  std::cout << "setup ui" << std::endl;
  ui.appWidget->getSemesters();
  std::cout << "got semesters" << std::endl;
  ui.appWidget->getData();
  std::cout << "got data" << std::endl;

  
}