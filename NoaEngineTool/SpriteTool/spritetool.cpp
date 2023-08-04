#include "spritetool.h"
#include "ui_spritetool.h"
#include <graphics.h>

SpriteTool::SpriteTool(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SpriteTool)
{
    ui->setupUi(this);
}

SpriteTool::~SpriteTool()
{
    delete ui;
}
