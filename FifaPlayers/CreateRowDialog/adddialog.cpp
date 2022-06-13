#include "adddialog.h"
#include "ui_adddialog.h"

#include "../TableModel/tablemodel.h"
#include <QMessageBox>
#include <QRegularExpression>


AddDialog::AddDialog(QWidget *parent, TableModel& mod) :
    QDialog(parent),
    ui(new Ui::AddDialog),
    model(mod)
{
    ui->setupUi(this);
    connectAll();
}

AddDialog::~AddDialog()
{
    delete ui;
}

void AddDialog::setId(QVariant id)
{
    ui->id->setText(id.toString());
}

void AddDialog::okButton()
{
    addAll();
}

void AddDialog::addAll()
{

    QList<QVariant> row{ui->id->text(), ui->short_name->text(),
                       ui->long_name->text(), ui->overall->text(),
                       ui->age->value(), ui->dob->text(),
                       ui->height_cm->text(), ui->weight_kg->text(),
                       ui->checkBox->isChecked()};


    model.appendRow(row);
    this->accept();
}

void AddDialog::checkDob(QString date)
{
    if (!checkRegEx(date))
    {
        ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(false);
        return;
    }
    ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(true);
}

bool AddDialog::checkRegEx(QString& text) // yyyy-mm-dd
{
    QRegularExpression reDate("[0-9]{0,4}\-(0[1-9]|1[0-2])\-(0[1-9]|[12][0-9]|3[01])");
    QRegularExpressionMatch match = reDate.match(text);
    bool result =  match.hasMatch();
    return result;
}

void AddDialog::cancelButton()
{
    this->reject();
}

void AddDialog::connectAll()
{
    connect(
                ui->buttonBox,
                &QDialogButtonBox::accepted,
                this,
                &AddDialog::okButton
                );
    connect(
                ui->buttonBox,
                &QDialogButtonBox::rejected,
                this,
                &AddDialog::cancelButton
                );
    connect(
                ui->dob,
                SIGNAL(textChanged(const QString&)),
                this,
                SLOT(checkDob(QString))
                );
}
