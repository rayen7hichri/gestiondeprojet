#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDateTime>
#include <QVector>
#include <QString>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

// Structure to represent a client request
struct ClientRequest {
    QDateTime date;
    QString clientName;
    QString serviceType; // "Broadcast", "Live", "YouTube Ads"
    QString status;      // "En cours", "Terminé", "Annulé"
    double amount;       // Montant en euros
    int durationMinutes; // Durée en minutes
    
    ClientRequest() : amount(0.0), durationMinutes(0) {}
    
    ClientRequest(const QDateTime& dt, const QString& client, const QString& service, 
                 const QString& stat, double amt, int duration)
        : date(dt), clientName(client), serviceType(service), status(stat), 
          amount(amt), durationMinutes(duration) {}
};

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void onPeriodChanged();
    void onRefreshClicked();
    void onExportStatsClicked();

    void updateAccueilTable();

private:
    Ui::MainWindow *ui;
    QVector<ClientRequest> clientRequests;
    
    // Private methods for statistics functionality
    void initializeStatistics();
    void loadSampleData();
    void updateStatisticsDisplay();
    void updateSummaryCards();
    void updateStatisticsTable();
    void filterRequestsByPeriod(const QString& period, QVector<ClientRequest>& filteredRequests);
    int getRequestCountByService(const QVector<ClientRequest>& requests, const QString& serviceType);
};
#endif // MAINWINDOW_H
