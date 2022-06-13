#include "application.h"
#include "ui_application.h"

#include <QMenuBar>
#include <QFileDialog>
#include <QMessageBox>
#include <QSortFilterProxyModel>

Application::Application(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Application)
{
    ui->setupUi(this);
    addHelp();
    fifaModel = new TableModel(this);
    proxyModel = new QSortFilterProxyModel(this);
    proxyFilt = new QSortFilterProxyModel(this);
    proxySearch = new QSortFilterProxyModel(this);
    proxyModel->setSourceModel(fifaModel);
    proxyFilt->setSourceModel(proxyModel);
    proxySearch->setSourceModel(proxyFilt);
    ui->tableView->setModel(proxySearch);
    ui->tableView->setSortingEnabled(true);
    proxyModel->setFilterKeyColumn(8);
    proxyFilt->setFilterKeyColumn(2);
    connectAll();
    hideDetailColumns();
}

Application::~Application()
{
    delete ui;
}

void Application::connectAll()
// On_name slots break these, careful!
{
    connect(
                ui->loadButton,
                SIGNAL(clicked()),
                this,
                SLOT(loadFromFile())
                );
    connect(
                ui->saveButton,
                SIGNAL(clicked()),
                this,
                SLOT(saveDataToFile())
                );
    connect(
                ui->addButton,
                SIGNAL(clicked()),
                this,
                SLOT(addPlayer())
                );
    connect(
                ui->deleteButton,
                SIGNAL(clicked()),
                this,
                SLOT(deletePlayer())
                );
    connect(
                ui->myteamButton,
                SIGNAL(clicked()),
                this,
                SLOT(addToMyTeam())
                );
    connect(
                ui->tableView,
                SIGNAL(clicked(QModelIndex)),
                this,
                SLOT(showDetails(QModelIndex))
                );
    connect(
                ui->myTeamOnly,
                SIGNAL(stateChanged(int)),
                this,
                SLOT(filterMyTeam(int))
                );
    connect(
                ui->lineEdit,
                SIGNAL(textChanged(const QString &)),
                this,
                SLOT(filterSearch(QString))
                );
}

void Application::issueHelp()
{
    dialog->show();
}

void Application::addHelp()
// Do not name Help, About on MACOS
{
    ui->menuBar->setNativeMenuBar(true);
    fileMenu = ui->menuBar->addMenu("Help Menu");
    actionAbout = new QAction("Show info", this);
    QObject::connect(actionAbout, &QAction::triggered, this, &Application::issueHelp);
    fileMenu->addAction(actionAbout);
    actionAbout->setShortcut(tr("F1"));
}

void Application::loadFromFile()
// Function for opening file with dialog
{
    QString fileName = QFileDialog::getOpenFileName(this, "Open File",
                                                        "../data",
                                                        "Data (*.csv)");
    if (!fileName.isNull())
    {
        delete fifaModel;
        delete proxyModel;
        delete proxyFilt;
        delete proxySearch;
        fifaModel = new TableModel(this);
        proxyModel = new QSortFilterProxyModel(this);
        proxyFilt = new QSortFilterProxyModel(this);
        proxySearch = new QSortFilterProxyModel(this);

        try
        {
            fifaModel->fillDataFromTable(fileName);
        }
        catch (int count)
        {
            QMessageBox::warning(this, "Warning", "Incorrect header");
            return;
        }
        catch (QString incorrectCollumn)
        {
            QMessageBox::warning(this, "Warning", "Incorrect header: " +
                                                      incorrectCollumn);
            return;
        }
        catch (long long amount)
        {
            QMessageBox::warning(this, "Warning",
                                 QVariant(amount + 1).toString() + " columns"
                                 + "\nExpected " + fifaModel->getHeaderCount().toString());
            return;
        }

        proxyModel->setSourceModel(fifaModel);
        proxyFilt->setSourceModel(proxyModel);
        proxySearch->setSourceModel(proxyFilt);
        ui->tableView->setModel(proxySearch);
        ui->tableView->setSortingEnabled(true);
        proxyModel->setFilterKeyColumn(8);
        proxyFilt->setFilterKeyColumn(2);       
        return;
    }
    QMessageBox::warning(this, "Warning", "File was not loaded!");
}

void Application::saveDataToFile()
{
    QString fileName = QFileDialog::getSaveFileName(this,

                                                      "Save file",
                                                      "../data",
                                                      "Data (*.csv)");
    // Default prefix
    if (!fileName.endsWith(".csv"))
    {
        fileName += ".csv";
    }

    QFile file(fileName);
    // Rewrites the file, not append!
    if (file.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        QTextStream stream(&file);
        fifaModel->saveDataToStream(stream);
        file.close();
        return;
    }
    QMessageBox::warning(this, "Warning", "File was not saved or written!");
}

void Application::addPlayer()
{
    addDialog = new AddDialog(this, *fifaModel);
    addDialog->show();
    addDialog->setId(fifaModel->rowCount());
}

int Application::wrapLayers()
{
    QModelIndex idx1 = ui->tableView->currentIndex();
    QModelIndex idx2 = proxySearch->mapToSource(idx1);
    QModelIndex idx3 = proxyFilt->mapToSource(idx2);
    QModelIndex idx4 = proxyModel->mapToSource(idx3);
    int rowIdx = idx4.row();
    return rowIdx;
}

void Application::deletePlayer()
{
    int rowIdx = wrapLayers();

    if (rowIdx < 0) // Unselected index
    {
        QMessageBox::warning(this, "Warning", "Invalid index for deleting!");
        return;
    }

    fifaModel->removeRow(rowIdx);
}

void Application::addToMyTeam()
{
    int rowIdx = wrapLayers();

    if (rowIdx < 0) // Unselected index
    {
        QMessageBox::warning(this, "Warning", "Invalid index for adding to team!");
        return;
    }
    fifaModel->changeMyTeam(rowIdx);
}

void Application::showDetails(QModelIndex idx)
{
   int rowIdx = wrapLayers();
   QList<QLabel*> fields{
       ui->long_name, ui->age, ui->dob, ui->height_cm, ui->weight_kg
   };
   QList<QVariant> values = fifaModel->getHidden(rowIdx);

   for (int i = 0; i < values.count(); ++i)
   {
       fields[i]->setText(values[i].toString());
   }
}

void Application::hideDetailColumns()
{
    for (int col:fifaModel->getHIDDEN_HEADERS_IDX())
        ui->tableView->hideColumn(col);
}

void Application::filterMyTeam(int mode)
{
    switch (mode)
    {
    case Qt::Unchecked:
        proxyModel->setFilterRegularExpression(QRegularExpression(""));
        return;
    case Qt::Checked:
        proxyModel->setFilterRegularExpression(QRegularExpression("true"));
        return;
    }
}

void Application::filterSearch(QString searchText)
{
    proxyFilt->setFilterRegularExpression(
                QRegularExpression(searchText,
                                   QRegularExpression::CaseInsensitiveOption));
}
