#include "MyForm.h"

MyForm::MyForm(QWidget* parent) : QWidget(parent)
{
  ui.setupUi(this);
  
  std::cout << "setup ui" << std::endl;
  ui.appWidget->getSemesters();
  std::cout << "got semesters" << std::endl;
  ui.appWidget->getData();
  std::cout << "got data" << std::endl;
}