#ifndef SPRITETOOL_H
#define SPRITETOOL_H

#include <QWidget>

namespace Ui {
class SpriteTool;
}

class SpriteTool : public QWidget
{
    Q_OBJECT

public:
    explicit SpriteTool(QWidget *parent = 0);
    ~SpriteTool();

private:
    Ui::SpriteTool *ui;
};

#endif // SPRITETOOL_H
