#ifndef APPLICATION_H
#define APPLICATION_H

#include <QMainWindow>
#include "../TableModel/tablemodel.h"
#include "../DialogWindow/dialog.h"
#include "../CreateRowDialog/adddialog.h"
#include <QTextStream>
#include <QSortFilterProxyModel>

namespace Ui {
class Application;
}

class Application : public QMainWindow
{
    Q_OBJECT

public:
    explicit Application(QWidget *parent = nullptr);
    ~Application();
    void issueHelp();

public slots:
    void loadFromFile();
    void saveDataToFile();
    void addPlayer();
    void deletePlayer();
    void addToMyTeam();
    void filterMyTeam(int mode);
    void filterSearch(QString searchText);

    void showDetails(QModelIndex idx);
    void hideDetailColumns();

private:
    Ui::Application *ui;
    void connectAll();

    TableModel *fifaModel;
    QSortFilterProxyModel *proxyModel;
    QSortFilterProxyModel *proxyFilt;
    QSortFilterProxyModel *proxySearch;

    int wrapLayers();

    // Help menubar section
    Dialog *dialog  = new Dialog(this);
    QMenuBar *bar;
    QMenu *fileMenu;
    QAction *actionAbout;
    void addHelp();

    // Add player dialog
    AddDialog* addDialog;
};

#endif // APPLICATION_H
