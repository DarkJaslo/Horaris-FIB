#include "ui_MyForm.h"
#include "Parser.hh"
#include "Data.hh"
#include <set>

class MyForm : public QWidget
{
  Q_OBJECT
  public:
    MyForm(QWidget* parent = 0);

  private:
    Ui::MyForm ui;
};