#pragma once
#include <QMainWindow>
#include <QTabWidget>
#include <QLineEdit>
#include <QToolBar>
#include <QWebEngineProfile>
#include <QWebEngineView>
#include <QProgressBar>
#include <QLabel>
#include <QEvent>
#include "Interceptor.h"
#include "FingerprintSpoofer.h"
#include "UpdateChecker.h"

class PrivateWindow : public QMainWindow {
    Q_OBJECT
public:
    explicit PrivateWindow(QWidget *parent = nullptr);
    ~PrivateWindow();

protected:
    void closeEvent(QCloseEvent *event) override;
    bool eventFilter(QObject *obj, QEvent *event) override;

private slots:
    void newTab(const QUrl &url = QUrl("about:blank"));
    void closeTab(int index);
    void navigateTo(const QString &input);
    void onUrlChanged(const QUrl &url);
    void onLoadStarted();
    void onLoadFinished(bool ok);

private:
    QWebEngineProfile *m_profile     = nullptr;
    Interceptor       *m_interceptor = nullptr;
    UpdateChecker     *m_updater     = nullptr;

    QTabWidget    *m_tabs        = nullptr;
    QLineEdit     *m_urlBar      = nullptr;
    QToolBar      *m_toolbar     = nullptr;
    QProgressBar  *m_progress    = nullptr;
    QLabel        *m_loadingDots = nullptr;

    void purge();
    void setupToolbar();
    void applyTheme();
    QWebEngineView *currentView() const;
    QUrl resolveInput(const QString &input) const;
};