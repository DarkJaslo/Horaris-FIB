#ifndef MyComboBox_hh
#define MyComboBox_hh

#include <QtWidgets>

class MyComboBox : public QComboBox
{
  Q_OBJECT

public:
  MyComboBox(QWidget *parent);

public slots:
  void addOption(const QString& s);

};

#endif