#ifndef ADDDIALOG_H
#define ADDDIALOG_H

#include "TableModel/tablemodel.h"
#include "ui_adddialog.h"
#include <QDialog>
#include <QGridLayout>
#include <QPushButton>
#include <QTextStream>

namespace Ui {
class AddDialog;
}

class AddDialog : public QDialog
{
    Q_OBJECT

public:
    explicit AddDialog(QWidget *parent, TableModel& mod);
    ~AddDialog();
    void setId(QVariant id);

public slots:
    void okButton();
    void cancelButton();
    void checkDob(QString date);

private:
    Ui::AddDialog *ui;
    TableModel& model;

    void connectAll();
    void addAll();
    static bool checkRegEx(QString& text);
};

#endif // ADDDIALOG_H
