#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QHeaderView>
#include <QTableWidgetItem>
#include <QMessageBox>
#include <QFileDialog>
#include <QTextStream>
#include <QDate>
#include <QWidget>
#include <QHBoxLayout>
#include <QPushButton>
#include <QIcon>
#include <Qt>

void MainWindow::updateAccueilTable()
{
    ui->tableWidget_2->setRowCount(5);
    // Ici, on ne met plus de boutons d'action, juste les lignes vides ou les QTableWidgetItem classiques
    // Exemple :
    for (int i = 0; i < 5; ++i) {
        for (int col = 0; col < 7; ++col) {
            QTableWidgetItem* item = new QTableWidgetItem("");
            ui->tableWidget_2->setItem(i, col, item);
        }
    }
}

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    initializeStatistics();
    updateAccueilTable();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::initializeStatistics()
{
    // Connect signals
    connect(ui->comboBox_period, SIGNAL(currentTextChanged(QString)), this, SLOT(onPeriodChanged()));
   // connect(ui->toolButton_refresh, SIGNAL(clicked()), this, SLOT(onRefreshClicked()));
    //connect(ui->toolButton_export_stats, SIGNAL(clicked()), this, SLOT(onExportStatsClicked()));
    
    // Configure table
    ui->tableWidget_2->setAlternatingRowColors(true);
    ui->tableWidget_2->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableWidget_2->horizontalHeader()->setStretchLastSection(true);
    ui->tableWidget_2->verticalHeader()->setVisible(false);
    
    // Set column widths
    ui->tableWidget_2->setColumnWidth(0, 120); // Date
    ui->tableWidget_2->setColumnWidth(1, 150); // Client
    ui->tableWidget_2->setColumnWidth(2, 130); // Service
    ui->tableWidget_2->setColumnWidth(3, 100); // Statut
    ui->tableWidget_2->setColumnWidth(4, 100); // Montant
    ui->tableWidget_2->setColumnWidth(5, 100); // Durée
    
    // Load sample data and update display
    loadSampleData();
    updateStatisticsDisplay();
}

void MainWindow::loadSampleData()
{
    clientRequests.clear();
    
    // Current week data
    QDateTime currentDate = QDateTime::currentDateTime();
    
    // Sample data for broadcast requests
    clientRequests.append(ClientRequest(currentDate.addDays(-2), "Entreprise ABC", "Broadcast", "Terminé", 1500.0, 30));
    clientRequests.append(ClientRequest(currentDate.addDays(-1), "Studio XYZ", "Broadcast", "En cours", 2200.0, 45));
    clientRequests.append(ClientRequest(currentDate.addDays(-5), "Media Corp", "Broadcast", "Terminé", 1800.0, 25));
    
    // Sample data for live streaming requests
    clientRequests.append(ClientRequest(currentDate.addDays(-1), "Event Pro", "Live", "En cours", 3500.0, 120));
    clientRequests.append(ClientRequest(currentDate.addDays(-3), "Conference Ltd", "Live", "Terminé", 2800.0, 90));
    clientRequests.append(ClientRequest(currentDate.addDays(-4), "Wedding Studio", "Live", "Terminé", 1200.0, 180));
    clientRequests.append(ClientRequest(currentDate.addDays(-6), "Sports Club", "Live", "Annulé", 4000.0, 240));
    
    // Sample data for YouTube ads requests
    clientRequests.append(ClientRequest(currentDate.addDays(-1), "Fashion Brand", "YouTube Ads", "Terminé", 800.0, 60));
    clientRequests.append(ClientRequest(currentDate.addDays(-2), "Tech Startup", "YouTube Ads", "En cours", 1200.0, 30));
    clientRequests.append(ClientRequest(currentDate.addDays(-4), "Restaurant Chain", "YouTube Ads", "Terminé", 900.0, 45));
    clientRequests.append(ClientRequest(currentDate.addDays(-5), "Fitness Center", "YouTube Ads", "Terminé", 650.0, 20));
    clientRequests.append(ClientRequest(currentDate.addDays(-7), "Travel Agency", "YouTube Ads", "Annulé", 1100.0, 40));
    
    // Add some older data for monthly view
    clientRequests.append(ClientRequest(currentDate.addDays(-10), "Old Client 1", "Broadcast", "Terminé", 1600.0, 35));
    clientRequests.append(ClientRequest(currentDate.addDays(-15), "Old Client 2", "Live", "Terminé", 2500.0, 150));
    clientRequests.append(ClientRequest(currentDate.addDays(-20), "Old Client 3", "YouTube Ads", "Terminé", 750.0, 25));
}

void MainWindow::updateStatisticsDisplay()
{
    updateSummaryCards();
    updateStatisticsTable();
}

void MainWindow::updateSummaryCards()
{
    QVector<ClientRequest> filteredRequests;
    filterRequestsByPeriod(ui->comboBox_period->currentText(), filteredRequests);
    
    int broadcastCount = getRequestCountByService(filteredRequests, "Broadcast");
    int liveCount = getRequestCountByService(filteredRequests, "Live");
    int youtubeCount = getRequestCountByService(filteredRequests, "YouTube Ads");
    int totalCount = broadcastCount + liveCount + youtubeCount;
    
    // Update summary cards
    ui->label_broadcast_count->setText(QString("<html><head/><body><p align=\"center\"><span style=\" font-size:24pt; font-weight:700; color:#4CAF50;\">%1</span></p></body></html>").arg(broadcastCount));
    ui->label_live_count->setText(QString("<html><head/><body><p align=\"center\"><span style=\" font-size:24pt; font-weight:700; color:#FF9800;\">%1</span></p></body></html>").arg(liveCount));
    ui->label_youtube_count->setText(QString("<html><head/><body><p align=\"center\"><span style=\" font-size:24pt; font-weight:700; color:#F44336;\">%1</span></p></body></html>").arg(youtubeCount));
    ui->label_total_count->setText(QString("<html><head/><body><p align=\"center\"><span style=\" font-size:24pt; font-weight:700; color:#9C27B0;\">%1</span></p></body></html>").arg(totalCount));
}

void MainWindow::updateStatisticsTable()
{
    QVector<ClientRequest> filteredRequests;
    filterRequestsByPeriod(ui->comboBox_period->currentText(), filteredRequests);
    
    ui->tableWidget_2->setRowCount(filteredRequests.size());
    
    for (int i = 0; i < filteredRequests.size(); ++i) {
        const ClientRequest& request = filteredRequests[i];
        
        // Date
        QTableWidgetItem* dateItem = new QTableWidgetItem(request.date.toString("dd/MM/yyyy"));
        ui->tableWidget_2->setItem(i, 0, dateItem);
        
        // Client
        QTableWidgetItem* clientItem = new QTableWidgetItem(request.clientName);
        ui->tableWidget_2->setItem(i, 1, clientItem);
        
        // Service
        QTableWidgetItem* serviceItem = new QTableWidgetItem(request.serviceType);
        ui->tableWidget_2->setItem(i, 2, serviceItem);
        
        // Status with color coding
        QTableWidgetItem* statusItem = new QTableWidgetItem(request.status);
        if (request.status == "Terminé") {
            statusItem->setBackground(QColor(200, 255, 200)); // Light green
        } else if (request.status == "En cours") {
            statusItem->setBackground(QColor(255, 255, 200)); // Light yellow
        } else if (request.status == "Annulé") {
            statusItem->setBackground(QColor(255, 200, 200)); // Light red
        }
        ui->tableWidget_2->setItem(i, 3, statusItem);
        
        // Amount
        QTableWidgetItem* amountItem = new QTableWidgetItem(QString("%1 €").arg(request.amount, 0, 'f', 2));
        amountItem->setTextAlignment(Qt::AlignRight | Qt::AlignVCenter);
        ui->tableWidget_2->setItem(i, 4, amountItem);
        
        // Duration
        QTableWidgetItem* durationItem = new QTableWidgetItem(QString("%1 min").arg(request.durationMinutes));
        durationItem->setTextAlignment(Qt::AlignRight | Qt::AlignVCenter);
        ui->tableWidget_2->setItem(i, 5, durationItem);
        
        // Actions (Edit & Delete icons)
        QWidget* actionWidget = new QWidget();
        QHBoxLayout* layout = new QHBoxLayout(actionWidget);
        layout->setContentsMargins(0, 0, 0, 0);
        QPushButton* editBtn = new QPushButton();
        QPushButton* deleteBtn = new QPushButton();
        editBtn->setToolTip("Modifier");
        deleteBtn->setToolTip("Supprimer");
        deleteBtn->setIcon(QIcon(":/icons/trash.png"));
        editBtn->setFlat(true);
        deleteBtn->setFlat(true);
        editBtn->setIconSize(QSize(24, 24));
        deleteBtn->setIconSize(QSize(24, 24));
        editBtn->setFixedSize(32, 32);
        deleteBtn->setFixedSize(32, 32);
        layout->addWidget(editBtn);
        layout->addWidget(deleteBtn);
        layout->setAlignment(Qt::AlignCenter);
        actionWidget->setLayout(layout);
        ui->tableWidget_2->setCellWidget(i, 6, actionWidget);
    }
}

void MainWindow::filterRequestsByPeriod(const QString& period, QVector<ClientRequest>& filteredRequests)
{
    filteredRequests.clear();
    QDateTime now = QDateTime::currentDateTime();
    QDateTime startDate;
    
    if (period == "Cette Semaine") {
        // This week (Monday to Sunday)
        int daysSinceMonday = now.date().dayOfWeek() - 1;
        startDate = now.addDays(-daysSinceMonday);
        startDate.setTime(QTime(0, 0, 0));
    } else if (period == "Ce Mois") {
        // This month
        startDate = QDateTime(QDate(now.date().year(), now.date().month(), 1), QTime(0, 0, 0));
    } else if (period == "Semaine Dernière") {
        // Last week
        int daysSinceMonday = now.date().dayOfWeek() - 1;
        QDateTime thisWeekStart = now.addDays(-daysSinceMonday);
        startDate = thisWeekStart.addDays(-7);
        startDate.setTime(QTime(0, 0, 0));
        QDateTime endDate = thisWeekStart.addDays(-1);
        endDate.setTime(QTime(23, 59, 59));
        
        for (const ClientRequest& request : clientRequests) {
            if (request.date >= startDate && request.date <= endDate) {
                filteredRequests.append(request);
            }
        }
        return;
    } else if (period == "Mois Dernier") {
        // Last month
        QDate lastMonth = now.date().addMonths(-1);
        startDate = QDateTime(QDate(lastMonth.year(), lastMonth.month(), 1), QTime(0, 0, 0));
        QDateTime endDate = QDateTime(QDate(now.date().year(), now.date().month(), 1), QTime(0, 0, 0)).addDays(-1);
        endDate.setTime(QTime(23, 59, 59));
        
        for (const ClientRequest& request : clientRequests) {
            if (request.date >= startDate && request.date <= endDate) {
                filteredRequests.append(request);
            }
        }
        return;
    }
    
    // For "Cette Semaine" and "Ce Mois"
    for (const ClientRequest& request : clientRequests) {
        if (request.date >= startDate) {
            filteredRequests.append(request);
        }
    }
}

int MainWindow::getRequestCountByService(const QVector<ClientRequest>& requests, const QString& serviceType)
{
    int count = 0;
    for (const ClientRequest& request : requests) {
        if (request.serviceType == serviceType) {
            count++;
        }
    }
    return count;
}

void MainWindow::onPeriodChanged()
{
    updateStatisticsDisplay();
}

void MainWindow::onRefreshClicked()
{
    // In a real application, this would reload data from database
    loadSampleData();
    updateStatisticsDisplay();
    
    QMessageBox::information(this, "Actualisation", 
                           "Les statistiques ont été actualisées avec succès!");
}

void MainWindow::onExportStatsClicked()
{
    QString fileName = QFileDialog::getSaveFileName(this, 
                                                   "Exporter les statistiques", 
                                                   QString("statistiques_%1.csv").arg(QDate::currentDate().toString("yyyy-MM-dd")),
                                                   "CSV Files (*.csv)");
    
    if (!fileName.isEmpty()) {
        QFile file(fileName);
        if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
            QTextStream stream(&file);
            
            // Header
            stream << "Date,Client,Service,Statut,Montant,Durée (min)\n";
            
            // Data
            QVector<ClientRequest> filteredRequests;
            filterRequestsByPeriod(ui->comboBox_period->currentText(), filteredRequests);
            
            for (const ClientRequest& request : filteredRequests) {
                stream << request.date.toString("dd/MM/yyyy") << ","
                       << request.clientName << ","
                       << request.serviceType << ","
                       << request.status << ","
                       << request.amount << ","
                       << request.durationMinutes << "\n";
            }
            
            file.close();
            QMessageBox::information(this, "Export réussi", 
                                   QString("Les statistiques ont été exportées vers:\n%1").arg(fileName));
        } else {
            QMessageBox::warning(this, "Erreur d'export", 
                               "Impossible d'écrire dans le fichier sélectionné.");
        }
    }
}
