#include "mainapptoolbar.h"

MainAppToolbar::MainAppToolbar(QWidget *parent) : QToolBar(parent)
{
   // setStyleSheet("background: #4f4f4f; color: #ffffff; font-size: 12pt");
    setMovable(false);
    setFloatable(false);
}
