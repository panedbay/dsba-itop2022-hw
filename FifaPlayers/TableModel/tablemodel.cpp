#include "tablemodel.h"

#include <QDialog>
#include <QMessageBox>

TableModel::TableModel(QObject *parent)
    : QAbstractTableModel(parent)
{
}

QVariant TableModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (orientation == Qt::Horizontal && role == Qt::DisplayRole)
    {
        return headers[section];
    }
    return QVariant();
}

bool TableModel::setHeaderData(int section, Qt::Orientation orientation, const QVariant &value, int role)
{
    if (value != headerData(section, orientation, role)) {
        // FIXME: Implement me!
        emit headerDataChanged(orientation, section, section);
        return true;
    }
    return false;
}


int TableModel::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return 0;

    return database.length();
}

int TableModel::columnCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return 0;

    return headers.length();
}

QVariant TableModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    if (role  == Qt::DisplayRole || role == Qt::EditRole)
    {
        return database[index.row()][index.column()];
    }
    return QVariant();
}

bool TableModel::setData(const QModelIndex &index, const QVariant &value, int role)
// Disabled
{
    if (data(index, role) != value)
    {
        QString strValue = value.toString();
        if (value == "")
        {
            return false;
        }
        database[index.row()][index.column()] = strValue;
        emit dataChanged(index, index, QVector<int>() << role);
        return true;
    }
    return false;
}

Qt::ItemFlags TableModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return Qt::NoItemFlags;

    // Items should not be edited
    // return QAbstractItemModel::flags(index) | Qt::ItemIsEditable;
    return QAbstractItemModel::flags(index);
}

void TableModel::appendRow(const QList<QVariant> &row)
{
    size_t newRowNumber = this->rowCount();
    beginInsertRows(QModelIndex(), newRowNumber, newRowNumber);
    database.append(row);
    endInsertRows();
}

void TableModel::fillDataFromTable(QString& path)
{
    QFile inputFile(path);
    inputFile.open(QFile::ReadOnly | QFile::Text);
    QTextStream inputStream(&inputFile);

    QString firstline = inputStream.readLine();

    checkFirstLine(firstline);

    while(!inputStream.atEnd())
    {
        QString line = inputStream.readLine();

        if (line.count(",") != HEADER_COUNT - 1)
        {
            QMessageBox::warning(nullptr, "Warning", "Bad row encountered!");
            return;
        }

        QList<QVariant> dataRow{};
        QList<QString> fileRow = line.split(",");
        QList<int> integerIndexes{0, 3, 4 ,6, 7};

        for (int i = 0; i < HEADER_COUNT; ++i)
        {
            if (integerIndexes.count(i) == 1)
            {
                dataRow.append(fileRow[i].toInt());
                continue;
            }
            dataRow.append(fileRow[i]);
        }
        database.append(dataRow);
    }
    inputFile.close();
}

void TableModel::checkFirstLine(QString &line) // check header line when loading file
{
    if (line.count(",") != HEADER_COUNT - 1)
    {
        throw line.count(",");
    }

    QList<QString> columns = line.split(",");
    // check for headers
    for (int i = 0; i < columns.count(); ++i)
    {
        if (columns[i] != headers[i])
        {
            throw columns[i] + " instead of " + headers[i];
        }
    }
}

void TableModel::saveDataToStream(QTextStream& stream)
{
    QString headerLine("");
    for (int i = 0; i < headers.count(); ++i)
    {
        headerLine += headers[i];
        if (i + 1 != headers.count())
        {
            headerLine += ",";
        }
    }
    stream << headerLine << '\n';

    for (int row = 0; row < database.count(); ++row)
    {
        QString finalRow("");
        for (int column = 0; column < headers.count(); ++column)
        {
            finalRow.append((database[row][column]).toString());
            if (column + 1 != headers.count())
            {
                finalRow.append(",");
            }
        }
        stream << finalRow << '\n';
    }
}

void TableModel::removeRow(int idx)
{
    if (rowCount() == 0)
    {
        QMessageBox::warning(nullptr, "Warning", "Nothing to erase");
        return;
    }
    beginRemoveRows(QModelIndex(), idx, idx);
    database.removeAt(idx);
    endRemoveRows();
}

QVariant TableModel::getHeaderCount()
{
    return HEADER_COUNT;
}

void TableModel::changeMyTeam(int row)
// Switcher of my_team
{
    database[row][8] = !database[row][8].toBool();
    QModelIndex idx = this->createIndex(row, 8);
    emit dataChanged(idx, idx);
}

QList<QVariant> TableModel::getHidden(int idx)
{
    QList<QVariant> result;
    for (int col:HIDDEN_HEADERS_IDX)
        result.append(database[idx][col]);
    return result;
}

QList<int> TableModel::getHIDDEN_HEADERS_IDX()
{
    return HIDDEN_HEADERS_IDX;
}
