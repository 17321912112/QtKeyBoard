#include "abstractlayout.h"


AbstractLayout::AbstractLayout(QWidget *parent)
    : QWidget(parent)
{

}

void AbstractLayout::showEvent(QShowEvent *event)
{
    QWidget::showEvent(event);
}

void AbstractLayout::hideEvent(QHideEvent *event)
{
    QWidget::hideEvent(event);
}

void AbstractLayout::keyReleaseEvent(QKeyEvent *event)
{
    QWidget::keyReleaseEvent(event);
}

void AbstractLayout::keyPressEvent(QKeyEvent *event)
{
    QWidget::keyPressEvent(event);
}
