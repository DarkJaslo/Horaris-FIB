#include "MyComboBox.hh"
#include <iostream>

MyComboBox::MyComboBox(QWidget* parent = 0) : QComboBox(parent){}

void MyComboBox::addOption(const QString& s)
{
  this->addItem(s);
}