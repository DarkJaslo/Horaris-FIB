#include "MyForm.h"

MyForm::MyForm(QWidget* parent) : QWidget(parent)
{
  ui.setupUi(this);
  ui.appWidget->getSemesters();
  ui.appWidget->getData();
}