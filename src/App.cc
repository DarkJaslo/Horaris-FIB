#include "App.hh"
using namespace jaslo;

App::App(QWidget* parent) : QWidget(parent)
{
  setFocusPolicy(Qt::StrongFocus); //Can get keyboard events
}