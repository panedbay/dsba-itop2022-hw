#ifndef TABLEMODEL_H
#define TABLEMODEL_H

#include <QAbstractTableModel>
#include <QFile>
#include <QTextStream>

class TableModel : public QAbstractTableModel
{
    Q_OBJECT

public:
    explicit TableModel(QObject *parent = nullptr);

    // Header:
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;

    bool setHeaderData(int section, Qt::Orientation orientation, const QVariant &value, int role = Qt::EditRole) override;

    // Basic functionality:
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

    // Editable:
    bool setData(const QModelIndex &index, const QVariant &value,
                 int role = Qt::EditRole) override;

    Qt::ItemFlags flags(const QModelIndex& index) const override;

    void appendRow(const QList<QVariant> &row);
    void removeRow(int idx);

    void fillDataFromTable(QString& path);
    void saveDataToStream(QTextStream& stream);

    QVariant getHeaderCount();
    QList<int> getHIDDEN_HEADERS_IDX();
    QList<QVariant> getHidden(int idx);

    void changeMyTeam(int row);
private:
    const int HEADER_COUNT = 9;
    QList<QList<QVariant>> database;
    QList<QString> headers
        {
        "id",
        "short_name",
        "long_name",
        "overall",
        "age",
        "dob",
        "height_cm",
        "weight_kg",
        "my_team"
         };
    QList<int> HIDDEN_HEADERS_IDX{2, 4, 5, 6, 7};
    void checkFirstLine(QString& line);
};

#endif // TABLEMODEL_H
